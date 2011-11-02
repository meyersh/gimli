/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: session.cpp
 * Purpose: Handle user profiles & session data.
 * This provides many helper functions for both.
 *
 * All user data is accessed through userProfile objects which are
 * written and read by function. This lets our business logic 
 * worry about the methods and not the underlying storage 
 * mechanisms.
 *
 * If I were to use this again, I'd objectify it. In fact,
 * I've basically already done this but 85% of my Final Project
 * code was written so I've kept this in. As far as I can tell,
 * the session_objectified object is ready and working.
 *
 */

using namespace std;

#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>

/* For stat()... I use file mod time to check a sessions freshness. */
#include <sys/stat.h>
#include <ctime>
#include <utime.h>

#include <mysql.h>
#include "database.hpp"

#define DB_PASSWORD "forwhomtbt"
#define DB_USER "meyersh"
#define DB_HOST "localhost"
#define DB_DATABASE "dbmeyersh"
#define TBL_PREFIX "270_social_"

#include "session.hpp"
#include "randomStr.hpp"

#define SESSION_SUFFIX ".session"
#define SESSION_PATH "/home/staff/meyersh/private/"
#define SESSION_FILE "/home/staff/meyersh/private/sessions.txt"

#define REGISTRATION_FILE "/home/staff/meyersh/private/user_registrations.txt"
#define USER_SUFFIX ".user"
#define USER_PATH "/home/staff/meyersh/private/"


/*
 * Read a given user profile, setting it to the given
 * userProfile structure. Returns non-zero on error.
 */

int 
readProfile(string username, userProfile& profile)
{
  /*
    select name,password,first_name,last_name,email,shoe_size,
    favorite_movie,general_statement from 270_social_user where name='meyersh';
  */

  /* Connect to MYSQL Database */
  MYSQL_RES *res;
  MYSQL_ROW row;

  bool retcode = 0;

  /* perform database query */
  char sql[255];
  sprintf(sql,
	  "SELECT name,password,first_name,last_name,email,shoe_size,favorite_movie,general_statement FROM 270_social_user WHERE name='%s'", username.c_str());
  
  res = result(sql, conn());

  /*
   * simple validation of results: 
   */
  if (res == NULL)
    { 
      return 1;
    }

  /* if everything went ok, store our values in the profile */
  
  if (mysql_num_rows(res) == 1)
    {
      row = mysql_fetch_row(res);
      profile.username       = row[0]; 
      profile.password       = row[1];
      profile.firstname      = row[2];
      profile.lastname       = row[3];
      profile.email          = row[4];
      profile.shoe_size      = atof(row[5]);
      profile.favorite_movie = (row[6]? row[6] : "");
      profile.statement      = (row[7]? row[7] : "");
    }
  else {
    retcode = 1;
    }

  /*
   * Clean up
   */

  mysql_free_result(res);
 
  return retcode;
}

string 
readSession(string seskey)
{
  /*
    SELECT user from 270_social_session WHERE skey='<skey>';
  */

  char sql[255];
  sprintf(sql, "SELECT user FROM 270_social_session WHERE skey='%s'", 
	  seskey.c_str());

  MYSQL_RES *res = result(sql);
  MYSQL_ROW row;

  if (mysql_num_rows(res) == 1)
    {
      row = mysql_fetch_row(res);
      mysql_free_result(res);
      return row[0]; 
    }
  else
    {
      return ""; /* Either no session, or something weird happening! */
    }

}

/*
 * Write the contents of a given userProfile structure to 
 * ascii-flat file.
 */

int
writeProfile(userProfile& profile)
{
  /*
    replace into 270_social_user 
    (name,password,first_name,last_name,email,shoe_size,general_statement) 
    values("meyersh","forwhomtbt","Shaun","Meyer","meyersh@morningside.edu", 
    10.5, "To be, or not to be...");
  */
  char sql[512];
  sprintf(sql, "REPLACE INTO 270_social_user (name,password,first_name,last_name,email,shoe_size,favorite_movie,general_statement) values('%s','%s','%s','%s','%s',%f,'%s','%s')", 
	  profile.username.c_str(), profile.password.c_str(), 
	  profile.firstname.c_str(), profile.lastname.c_str(),
	  profile.email.c_str(), profile.shoe_size, 
	  profile.favorite_movie.c_str(), profile.statement.c_str());

  MYSQL *connection = conn();
  MYSQL_RES *res = result(sql, connection, false);
  int affected_rows = mysql_affected_rows(connection);
  mysql_free_result(res);
  mysql_close(connection);
  
  if (affected_rows > 0)
    {
      return 0;
    }
  else 
    {
      return 1;
    }

}

/* 
 * start a new session for "username". This does not do any checking on
 * passwords or etc, just tries to create the session and returns the session
 * ID.
 */

string
newSession(string username)
{
  string random_string;
  randomString(random_string, 25);

  /*
   INSERT INTO 270_social_session (user,skey) values("username","skey");
  */
  
  char sql[255];
  sprintf(sql,
	  "INSERT INTO 270_social_session (user,skey) values('%s','%s')",
	  username.c_str(), random_string.c_str());

  /* above doesn't check for key collision. */

  mysql_free_result(result(sql));
  return random_string;
}

/*
 * Validate session. Will return true if session is OK,
 * under age and what have you, false if does not exist or
 * too old.
 */

bool 
checkSession(string sessionkey, int maxage)
{
  /* MySQL is responsible for the age-checking 
   * in this case. Basically, if we get results
   * true, if we get nil, false. 
   *

   SELECT skey FROM 270_social_session WHERE time > ADDTIME(NOW(),-300) AND skey='%s';

  */

  char sql[255];

  sprintf(sql, 
	  "SELECT skey FROM 270_social_session WHERE time > ADDTIME(NOW(),-%d) AND skey='%s'", 
	  maxage, sessionkey.c_str());

  /*
   * touch this session (if it exists)
   * remove depracted sessions (if they exist)
   */

  MYSQL_RES *res = result(sql);

  if (mysql_num_rows(res) == 1)
    {
      touchSession(sessionkey);
      mysql_free_result(res);
      return true;
    }
  else 
    {
      mysql_free_result(res);
      return false;
    }

}

/*
 * Remove a session. Works by copying every OTHER session into
 * a temporary file and then moving the temporary file into the
 * sessionfile. 
 * also removes the .session file.
 */

void
rmSession(string sessionkey)
{
  /*
   DELETE FROM 270_social_session WHERE skey='whatever';
  */


  char sql[255];
  sprintf(sql,
	  "DELETE FROM 270_social_session WHERE skey='%s';",
	  sessionkey.c_str());
  
  mysql_free_result(result(sql));

}


/*
 * Return the session age in seconds. 
 * On error: returns -1
 * ... THIS is only used by checksession.cgi...
 */

int
getSessionAge(string sessionkey)
{
  string filename = SESSION_PATH + sessionkey + SESSION_SUFFIX;
  struct stat foo;
  int mtime;

  if ( stat(filename.c_str(), &foo) )
    return -1;

  mtime = (int)foo.st_mtime; /* seconds since epoch */

  return time(NULL) - mtime; //  - mtime;

}


/*
 * housekeeping - iterate through all sessions and clean up.
 */

void 
checkAllSessions(int maxAge)
{
  /*
    SELECT skey FROM 270_social_session WHERE time > ADDTIME(NOW(),-300);
  */

  char sql[255];
  sprintf(sql,
	  "DELETE FROM 270_social_session WHERE time < ADDTIME(NOW(), -%d)",
	  maxAge);

  mysql_free_result(result(sql));

}

/*
 * touch a given session, set its age to 0 seconds. 
 */

void 
touchSession(string sessionkey)
{

  /*
    UPDATE 270_social_session SET time=NOW() WHERE skey = '123aABSd2';
  */

  char sql[255];
  sprintf(sql,
	  "UPDATE 270_social_session SET time=NOW() WHERE skey='%s'",
	  sessionkey.c_str());

  mysql_free_result(result(sql));
}

string
listUsers(bool newline) /* setting this to true will 
				   return all users \n separated. */
{
  stringstream output;

  /*
    SELECT user from 270_social_user;
  */

  char sql[255] = "SELECT name from 270_social_user";
  MYSQL_RES *res = result(sql);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(res)) != NULL)
    {
      if (newline)
	output << row[0] << endl;
      else
	output << "<a onClick='showProfile(\"" << row[0] << "\");' href='#'>" << row[0] << "</a>, ";
    }

  return output.str();
}

string
loggedInUsers(bool newline)
{
  stringstream output;

  /* 
   * SELECT user from 270_social_session;
   */

  char sql[255] = "SELECT user from 270_social_session";
  MYSQL_RES *res = result(sql);
  MYSQL_ROW row;

  while ((row = mysql_fetch_row(res)) != NULL)
    {
      if (newline)
	output << row[0] << endl;
      else
	output << row[0] << ',';
    }
  return output.str();
}

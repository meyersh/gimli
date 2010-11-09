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

/* For stat()... I use file mod time to check a sessions freshness. */
#include <sys/stat.h>
#include <ctime>
#include <utime.h>


#include "session.hpp"
#include "randomStr.hpp"

#define SESSION_SUFFIX ".session"
#define SESSION_PATH "/home/meyersh/private/"
#define SESSION_FILE "/home/meyersh/private/sessions.txt"

#define REGISTRATION_FILE "/home/meyersh/private/user_registrations.txt"
#define USER_SUFFIX ".user"
#define USER_PATH "/home/meyersh/private/"


/*
 * Read a given user profile, setting it to the given
 * userProfile structure. Returns non-zero on error.
 */

int 
readProfile(string username, userProfile& profile)
{
  ifstream userfile;
  string filename = USER_PATH + username + USER_SUFFIX;

  userfile.open( filename.c_str());
  if(!userfile.is_open())
    return -1;
  
  profile.username = username;
  std::getline(userfile, profile.password);
  std::getline(userfile, profile.realname);
  std::getline(userfile, profile.email);
  std::getline(userfile, profile.favorite_movie);
  string shoesize;
  std::getline(userfile, shoesize);
  profile.shoe_size = atof( shoesize.c_str() );
  std::getline(userfile, profile.whiteboard);
  
  userfile.close();

  return 0;
      
}

string 
readSession(string seskey)
{
  ifstream sesfile;
  string filename = SESSION_PATH + seskey + SESSION_SUFFIX;
  sesfile.open( filename.c_str() );

  string username;
  std::getline(sesfile, username);
  
  return username;
}

/*
 * Write the contents of a given userProfile structure to 
 * ascii-flat file.
 */

int
writeProfile(userProfile& profile)
{
  ofstream userfile;
  string filename = USER_PATH + profile.username + USER_SUFFIX;

  userfile.open( filename.c_str());
  if (!userfile.is_open())
    return -1;

  userfile << profile.password << endl
	   << profile.realname << endl
	   << profile.email << endl
	   << profile.favorite_movie << endl
	   << profile.shoe_size << endl
	   << profile.whiteboard << endl;

  userfile.close();

  return 0;
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
  randomString(random_string, 128);

  /* above doesn't check for key collision. */

  // write username to key.
  ofstream f;
  string filename(SESSION_PATH + random_string + SESSION_SUFFIX);

  ofstream sessionfile;
  sessionfile.open(SESSION_FILE, ios::app);
  sessionfile << random_string << endl;

  f.open(filename.c_str());
  f << username << endl;
  f.close();
  sessionfile.close();
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
  ifstream allsessions;
  string ses; 
  allsessions.open(SESSION_FILE);
  while ( std::getline(allsessions, ses) )
    {
      if ( sessionkey == ses )
	if (getSessionAge(sessionkey) < maxage)
	  {
	    touchSession( sessionkey); 
	    return true;
	  }
	else
	  {
	    rmSession( sessionkey );
	    break;
	  }
    }
  return false;
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
  string filename, session;
  filename = SESSION_PATH + sessionkey + SESSION_SUFFIX;
  remove( filename.c_str() );

  /* remove the session from SESSION_FILE */
  string altsesfilename = SESSION_FILE;
  altsesfilename += ".tmp";

  ifstream sesfile;
  ofstream altsesfile;
  
  sesfile.open( SESSION_FILE );
  altsesfile.open( altsesfilename.c_str() );

  std::getline(sesfile, session, '\n');
  while ( std::getline(sesfile, session, '\n') )
    {
      if (session != sessionkey)
	{
	  altsesfile << session << endl;
	}
    }

  /* Now move in the new file... */
  rename( altsesfilename.c_str(), SESSION_FILE );

}


/*
 * Return the session age in seconds. 
 * On error: returns -1
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
  ifstream sesfile;
  sesfile.open(SESSION_FILE);
  string altsesfilename = SESSION_FILE;
  altsesfilename += ".tmp";

  ofstream altsesfile;
  altsesfile.open(altsesfilename.c_str());

  string session;
  string filename;
  while (std::getline(sesfile, session, '\n'))
    {
      int age = getSessionAge(session);

      if (age < maxAge)
	altsesfile << session << endl;
      else
	{
	  filename = SESSION_PATH + session + SESSION_SUFFIX;
	  remove( filename.c_str() );
	}
	  
    }
  altsesfile.close();
  rename(altsesfilename.c_str(), SESSION_FILE);
}

/*
 * touch a given session, set its age to 0 seconds. 
 */

void 
touchSession(string sessionkey)
{
  string filename = SESSION_PATH + sessionkey + SESSION_SUFFIX;

  struct utimbuf new_times;
  new_times.modtime = time(NULL);
  utime(filename.c_str(), &new_times);

}


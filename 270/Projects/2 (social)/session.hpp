/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: session.hpp
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

#ifndef SESSION
#define SESSION

#define MAX_SESSION_AGE 5*60 /* 5 minutes */

struct userProfile {
  string username;
  string firstname;
  string lastname;
  string password;
  string email;
  string favorite_movie;
  double shoe_size;
  string statement;
};

/* Prototypes */

int readProfile(string username, userProfile& profile);
int writeProfile(userProfile& profile);

string readSession(string seskey);

/* Create a new session for a given username */
string newSession(string username);

/* retrieve the age of a given session in seconds */

int getSessionAge(string sessionkey);

/* check ALL known sessions for being overage */
void checkAllSessions(int maxAge = MAX_SESSION_AGE);

/* check a given session for overage. 
 * If the age is too high, removes the session, otherwise 
 * touches it. */
bool checkSession(string sessionkey, int maxage = MAX_SESSION_AGE);

/* remove a given session */ 
void rmSession(string sessionkey);

/* update the timestamp on a given session, marking it as "current" */
void touchSession(string sessionkey);

string listUsers(bool newline = false);
string loggedInUsers(bool newline);

#endif

#ifndef SESSION_OBJ
#define SESSION_OBJ

#define MAX_SESSION_AGE 5*60 /* 5 minutes */

#include <string>
#include "randomStr.hpp"

using namespace std;

class userProfile {

private:
  bool valid;

public:
  string username;
  string realname;
  string password;
  string email;
  string favorite_movie;
  double shoe_size;
  userProfile(string username = "");
  int readProfile(string username);
  int writeProfile();
};

/* Prototypes */


/* int userProfile::readProfile(string username);
   int userProfile::writeProfile(); */

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

#endif

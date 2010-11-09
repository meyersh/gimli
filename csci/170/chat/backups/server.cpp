/* 
 * A message server system
 * Shaun Meyer - Feb, 2010
 */

/* 
 * TODO:
 *   - Add Client/Server hooks so that server-daemon 
 *     can stay running and maintain a persistent message
 *     database in memory while Clients access this to add
 *     messages. (CGI "script" will be the Client.)
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>

#define MAX_MESSAGES        5       // Maximum messages kept in buffer. 
#define MESSAGE_DELIMITER   '\t'    // Message fields are delimited how?
#define USER_MAX_AGE		600		// Maximum seconds inactive for a user

using namespace std;

/*
 * Our chatmsg structure...
 */
struct chatmsg {
    string username;
    string recipient; // Should default to "ALL"
    string message;
    } foo ;

/* Prototypes */
vector<string> split(string, char);
bool inRoom(string, map<string, long> *);
void updateUser(string, map<string, long>*, long );
int checkUserList(map<string, long>*, long);
int checkMessageBuffer(multimap<long, chatmsg>*);
void updateUser(string, map<string, long>*, long);



/*
 * Split a given string on char delimiter
 */
vector<string>
split(string line, char delimiter)
{
    istringstream linestream(line); // convert line to a stream
    string token; // A given thing between delimiters
    vector<string> output; // Our output vector

    while ( getline(linestream, token, delimiter) )
    {
        output.push_back(token);
    }

    return output;
}

/*
 * I know it's probably in poor taste to mix
 * '(*ptr).method()' and ptr->method() but I'm
 * too inexperienced to know which is which. Once I
 * understand, fully, how they work I'll choose one.
 */

bool
inRoom(string username, map<string, long> *userlist)
{
  if ( (*userlist).size() )
	{
	  map<string, long>::iterator iter = userlist->begin();
	  for (iter; iter != (*userlist).end(); iter++)
		{
		  if ( (*iter).first == username)
			return true;
		}
	}
  else
	return false;
}

/*
 * `touch` a user, updating their timestamp-- or if they're
 * not in the list-- inserting them.
 */
void
updateUser(string username, map<string, long>* userlist, long now)
{
  if ( inRoom(username, userlist) )
	{
	  (*userlist)[username] = now;	  
	}
  else // They're not in the room, we have to add them...
	(*userlist)[username] = now;
}

/*
 * Process the user list. If any users' last activity
 * is beyond USER_MAX_AGE seconds ago, remove them from
 * the list.
 * RETURNS: Number of users removed.
 */

int
checkUserList(map<string, long>* userlist, long now)
{
  map<string, long>::iterator iter = userlist->begin();
  int i = 0; // number of entries removed.
  for (iter; iter != userlist->end(); iter++)
	{
	  if ( (now - iter->second) > USER_MAX_AGE )
		{
		  userlist->erase(iter);
		  i++;
		}
	}
  return i;
}

int
checkMessageBuffer(multimap<long, chatmsg> *messages)
{
  multimap<long, chatmsg>::iterator it;
  int i = 0; // Return value. non-zero if messages deleted.
  if (messages->size() > MAX_MESSAGES)
	{
	  it = messages->begin();
	  messages->erase(it);
	  cout << "Message buffer exceded, popping oldest.\n";
	  i++;
	}
  return i;
}


int 
main()
{
    chatmsg foo;
    string line;
    vector<string> tmp; 
    multimap<long, chatmsg> messages; // all messages
    multimap<long, chatmsg>::iterator it;
	map<string, long> userlist; // username:lastactivity_timestamp
	long now = time(NULL); // Our global 'now' timestamp for each run-through
    
    while (1) {
	  std::getline(cin, line, '\n');
	  now = time(NULL);

	  if (line[0] == 'Q' || line == "") // this catches ctrl-D, bonus!
		break;

	  tmp = split(line, MESSAGE_DELIMITER);
	  if ( tmp.size() == 3 )
		{
		  foo.username = tmp[0];
		  foo.recipient = tmp[1];
		  foo.message = tmp[2];
		}
	  else // invalid line, go ahead and skip the rest.
		continue;
	  
	  // update the users activity timestamp
	  updateUser(foo.username, &userlist, now);

	  // check to see if we have to remove any users
	  checkUserList(&userlist, now);

	  // add new message(s)
	  messages.insert(std::make_pair(time(NULL), foo));

	  // check MessageBuffer (for # of messages)
	  checkMessageBuffer(&messages);
	}
	  
    
    cout << messages.size() << " messages!\n";
	cout << userlist.size() << " users!\n";
	

	it = messages.begin();
    // Run off a list of our messages!
    while (it != messages.end())
	  {
        cout << (*it).first << " > "
			 << (*it).second.username << ": "
			 << (*it).second.recipient << " | "
			 << (*it).second.message  
			 << endl;   
        it++;
	  }

	map<string,long>::iterator mit = userlist.begin();
	while (mit != userlist.end())
	  {
		cout << mit->first << endl;
		mit++;
	  }
    return 0;
}

    

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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for inet_pton()
#include <cstdlib> // sizeof
#include <errno.h>
#include <cctype> // for toupper

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <algorithm> // for transform()
#define MAX_MESSAGES        5       // Maximum messages kept in buffer. 
#define MESSAGE_DELIMITER   '\t'    // Message fields are delimited how?
#define USER_MAX_AGE		600		// Maximum seconds inactive for a user
#define BUFFER_SIZE			1024	// TCP max buffer size

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
string getMessages(multimap<long, chatmsg>*, string username, long last=0);


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
  if ( userlist->size() )
	{
	  map<string, long>::iterator iter = userlist->begin();
	  for (iter; iter != userlist->end(); iter++)
		{
		  if ( iter->first == username)
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
 * checkUserList(&userlist, now)
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

/*
 * checkMessageBuffer(&messages)
 * Performs housekeeping on the message buffer, currently
 * limited to deleting old messages.
 */

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

/*
 * Returns a MESSAGE_DELIMITER delimited list starting with the
 * number of users followed by all users.
 */

string
userList(map<string, long> *userlist)
{
  string reply;
  if (!userlist->size())
	return "";
  else
	{
	  map<string, long>::iterator mit = userlist->begin();
	  int len = userlist->size();
	  int i;
	  
	  for (mit; mit != userlist->end(); mit++)
		{
		  if ( i == len - 1 )
			reply += mit->first;
		  else
			reply += mit->first + MESSAGE_DELIMITER;
		  i++;
		}
	  return reply;
	}
}

/*
 * getSocketInput(&client)
 * Return the recv() buffer as a string object
 */

string
getSocketInput(int* client)
{
  char buffer[BUFFER_SIZE];
  string inpt;
  
  recv(*client, (void*)buffer, 1024, 0);
  inpt = buffer;
  if (inpt.find_first_of('\r') != string::npos)
	inpt.erase(inpt.find_first_of('\r'));
  return inpt;
}

/*
 * Simple wrapper around send() that accepts
 * a string-type in argument.
 */

int
giveSocketReply(int* client, string reply)
{
  send(*client, (void*)reply.c_str(), reply.length(), 0);
}

/*
 * getMessages(&messages, username, <last>)
 * returns a newline delimited list of messages, each are
 * MESSAGE_DELIMITER delimited
 * <timestamp, username, recipient, message>
 */

string
getMessages(multimap<long, chatmsg>* messages, string username, long last)
{
  stringstream reply;
  multimap<long, chatmsg>::iterator it;

  for (it = messages->begin(); it != messages->end(); it++)
	{
	  if ( it->second.recipient == "ALL"
		   || it->second.recipient == username
		   || it->second.username == username )
		{
		  if ( it->first > last )
			{
			  reply << it->first << MESSAGE_DELIMITER
					<< it->second.username << MESSAGE_DELIMITER
					<< it->second.recipient << MESSAGE_DELIMITER
					<< it->second.message << endl;
			}
		}
	}
  return reply.str();
}

/*
 * This is it, the REAL DEAL begins HERE...
 */

int 
main(int argc, char* argv[])
{
  chatmsg foo;
  string line;
  stringstream buf;
  vector<string> tmp, args; 
  multimap<long, chatmsg> messages; // all messages
  multimap<long, chatmsg>::iterator it;
  map<string, long> userlist; // username:lastactivity_timestamp
  long now = time(NULL); // Our global 'now' timestamp for each run-through
  
  /*
   * Start Socket-y things
   */
  
  struct sockaddr_in ip4addr;
  ip4addr.sin_family = AF_INET;
  ip4addr.sin_port = htons(9000);
  //ip4addr.sin_addr.s_addr = INADDR_ANY;
  inet_pton(AF_INET, "127.0.0.1", &ip4addr.sin_addr);
  
  int sock;   
  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	  perror("Socket");
	  exit(errno);
	}
  
  if ( bind(sock, (struct sockaddr*)&ip4addr, sizeof ip4addr) != 0 )
	{
	  perror("socket--bind");
	  exit(errno);
	}
  
  if ( listen(sock, 20) != 0 )
	{
	  perror("socket--listen");
	  exit(errno);
	}
  
  string fromclient, command;
  char buffer[1024];
  ssize_t bufferlen;
  istringstream cppbuffer;
  cout << "Listening...\n";

  /* Forever listen loop */
  
  while(1) {
	now = time(NULL);
	int client;
	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);
	
	// accept a connection...
	client = accept(sock, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

	printf("%s:%d connected\n",
		   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	
	/*
	 * BEGIN COMMAND SOCKET HANDLING
	 */
	
	// HANDLE `COMMAND` Input -> char buffer to string command
	//buffer[0] = '\0';
	command = "";
	args.resize(0); // clear the args.
	
	args = split(getSocketInput(&client), MESSAGE_DELIMITER);
	command = args[0];

	cout << "COMMAND: [" << command << "]" << endl;
	
	/*
	 * Command processing ( I may move this later )
	 */

	if ( command == "WHO" )
	  {
		buf.str("");
		buf << userlist.size();
		buf << MESSAGE_DELIMITER;
		buf << userList(&userlist);
		buf << endl;
		 
		giveSocketReply(&client, buf.str());
		close(client);
		continue;
	  } // end "WHO"
	else if (command == "WHAT" )
	  {
		if (args.size() == 2)
		  giveSocketReply(&client, getMessages(&messages,
											   args[1]));
		else if (args.size() == 3)
		  giveSocketReply(&client,
						  getMessages(&messages,
									  args[1],
									  atoi(args[2].c_str())));
		close(client);
		continue;
	  } // end "WHAT"
	else
	  giveSocketReply(&client, command+"+ACK\n");
	
	/*
	 * Message processing ( only if command is non-blocking ) 
	 */

	// Clear buffer for message
	//	buffer[0] = '\0';
	cppbuffer.str("");	
	//	cppbuffer.str( getSocketInput(&client) );
	
	//std::getline(cppbuffer, line);

	//line = split( getSocketInput(&client), MESSAGE_DELIMITER);
	
	if (line.find_first_of('\r') != string::npos)
	  line.erase(line.find_first_of('\r'));

	tmp = split( getSocketInput(&client), MESSAGE_DELIMITER );
	
	cout << "BUFFER:  [" << tmp[2] << "]" << endl;
	
	// close connection, we're not exchanging any more info this session.
	close(client);
		
	//if (line[0] == 'Q' || line == "") // this catches ctrl-D, bonus!
		//break;
	
	//tmp = split(line, MESSAGE_DELIMITER);
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
  close(sock);
  cout << "Closing...\n";
  return 0;
}

    

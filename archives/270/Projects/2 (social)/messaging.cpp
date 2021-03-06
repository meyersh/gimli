/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: messaging.cpp
 * Purpose: Handle messaging & blacklisting
 *
 * How: Two general purpose classes - message & blacklist
 * 
 * The messaging class is basically a thin wrapper around a 
 * map<int, message> object (message being a struct)
 *
 * I've written a few worker member-functions for this and
 * I think most of them are pretty intuitive. They are also
 * complete enough that the odd-ball permissions (for instance,
 * private 'under-the-door' messaging) can be handled entirely
 * with these methods without direct access to the msgList map.
 * 
 * When a new message is added this datastructure checks the last
 * used ID and automatically increments by one. 
 *
 * Blacklist is a pretty straightforward function, also. On
 * construction it attemps to read a file (which is named based
 * on username and entity) and puts these names into a vector.
 * it then provides a few functions to manipulate this blacklist
 * table. It then writes the file again on destruction.
 *
 * At any rate, I'm pretty proud of this one.
 *
 */



#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <cstdlib>
#include <vector>
#include <time.h>

#include <mysql.h>
#include <mysql_time.h>
#include "database.hpp"

using namespace std;

/*
 * On disk layout of messages:
 * <url-encoded>
   <message id>,<in response-to>,<author>,<time>,<content>\n

 */

#define MSG_PATH "/home/staff/meyersh/private/messages/"
#define MSG_SUFFIX ".messages"

class message {
 private:
 public:
  string sender, body;
  string timestamp;
  int id, inResponseTo;
  vector<int> childMessages;
  message()
  {
    id=0;
  }
};

/* A thin wrapper around a map object... */
/*
 * USAGE:
 Read public messages:
 msgs ms(inbox_owner)
 messages=urlEncode( ms.listMessages() )
 
 Write a new message:
 message m;
 //... populate message ...//
 msgs ms(to_user)
 ms.insert(m);
 
 Delete a message:
 msg ms(username);
 if (ms.hasMessage(id)) ms.remove(id, 0); 
 
 Do something with a PRIVATE message:
 msg ms(inbox_owner, "private");
*/

class msgs {
private:
  bool DEBUG;
public: /* I'm not proud of this. */
  map<int, message> msgList; /* msgList[id] = <message> */
private:
  string username, entity;
  
  string msgfile()
  {
    string filepath  = MSG_PATH;
    if ( entity == "" )
      filepath += username + MSG_SUFFIX;
    else
      filepath += username + "." + entity + MSG_SUFFIX;

    return filepath;
  }

 

  bool tryOpen()
  {
    /*
    string filename = "";
    ifstream msgFile;
    filename = msgfile();

    msgFile.open( filename.c_str() );
    
    if (!msgFile.is_open())
      return false;

    msgFile.close();
    */
    return true;
  }

 public:

  /* 
   * CONSTRUCTOR 
   */

  msgs(string username, string entity="", bool DEBUG=false)
  {
    this->entity = entity;
    this->DEBUG = DEBUG;
    this->username = username;
    /* can we open the messages file? is it new? */
    if (tryOpen())
      readMessages();
  }

  /*
   * DESTRUCTOR!
   */

  ~msgs()
  {
    writeMessages();
  }

  int lastMessage()
  {
    map<int, message>::iterator it;
    int i=0;
    for (it = msgList.begin(); it != msgList.end(); it++)
      if ( it->first > i )
	i = it->first;
    return i;
  } 


  /* 
   * Do we have a message ID in our msgList?
   */

  bool hasMessage( int id )
  {
    if ( msgList.find( id ) != msgList.end() )
      return true;
    return false;
  }

  /*
   * subscript overloading 
   */

  message& operator[](const int idex)
  {
    return msgList[idex];
    
  }
  /*
   * readMessages( string username )
   */

  int readMessages( )
  {
    msgList.clear();
    string line, msgFileName;

    char sql[255]; 
    sprintf(sql,
	    "SELECT id, in_response_to, sender, sent, message \
             FROM 270_social_message \
             WHERE recipient = '%s' AND entity = '%s'",
	    username.c_str(), this->entity.c_str());

    MYSQL_RES *res = result(sql);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res)) != NULL) /* foreach message: */
      {

	message m;

	/* m.id */
	m.id = atoi(row[0]);

	/* m.inResponseTo */
	m.inResponseTo = atoi(row[1]);

	/* Keep threading up to date */
	if ( hasMessage( m.inResponseTo ) )
	  msgList[ m.inResponseTo ].childMessages.push_back( m.id );

	/* m.sender */
	m.sender = row[2];
	
	/* m.timestamp */
	m.timestamp = row[3];

	/* m.body */
	m.body = row[4];

	/* insert message into our msg list data structure. */
	msgList.insert( std::pair<int, message> (m.id, m) );
      }
    
    return 0;
  }

  /*
   * writeMessages(string msgFileName="")
   * This is antiquated now that insert() does a save.
   */

  int writeMessages()  { return 0; }

  /*
   * remove( int message-id, bool removeChildren )
   */

  int remove (int id, bool removeChildren=1)
  {
    if (!hasMessage(id))
      return 0;

    int parentId = msgList[ id ].inResponseTo;

    /* Do they have child messages? */
    if ( msgList[id].childMessages.size() > 0)
      {
	vector<int>::iterator child;
	for (child = msgList[id].childMessages.begin(); 
	     child != msgList[id].childMessages.end(); 
	     child++) {
	  if ( removeChildren )
	    remove(*child);
	  else
	    msgList[*child].inResponseTo = parentId;
	}
      }

    /* finally, remove our id from msgList && sql database */
    msgList.erase(id);

    char sql[255];
    sprintf(sql,
	    "DELETE FROM 270_social_message WHERE id=%d",
	    id);

    mysql_free_result(result(sql));

    return 0;
  }

  /*
   * insert( message m )
   * Add a message to our SQL database & our data structure...
   */

  void insert( message m )
  {
    /*
    insert into 270_social_message 
    (sender,recipient,private,message,entity) 
    values("meyersh", "meyersh", "N", "How are you?", "underdoor");
    */
    
    char sql[255];
    sprintf(sql,
	    "INSERT INTO 270_social_message \
             (sender,recipient,in_response_to,message,entity) \
             values('%s', '%s', '%d', '%s', '%s')",
	    m.sender.c_str(), this->username.c_str(), 
	    m.inResponseTo, m.body.c_str(), this->entity.c_str());

    MYSQL *connection = conn();
    mysql_free_result(result(sql, connection, false));
    m.id = mysql_insert_id(connection);
    mysql_close(connection);

    msgList.insert( std::pair<int, message> (m.id, m) );
  }

  /*
   * size()
   */

  int size()
  {
    return msgList.size();
  }

  /* Print out the messages! */
  string listMessage(message m, int indent = 0, bool showReply=true, 
		     bool showDelete=true)
  {
    stringstream output;
    for (int i = 0; i < indent; i++)
      output << " ";

    /* Message output: */
    if ( indent )
      output << "<li>";

    output << "<div id=\"" << m.id << "\">\n";

    if ( showReply )
      output << "  [ <a href=\"#\" onClick=\"replyPost(this);\">reply</a> ] \n";
    if ( showDelete )
      output << "  [ <a href=\"#\" onClick=\"delPost(this)\">delete</a> ] \n";

    output << "  <b>" << m.sender << " says:</b> " << m.body << endl;

    /* hidden reply output: */
    output << "  <div class=\"form-row\" id=\"inResponseTo" 
	   << m.id << "\" style=\"display:none\";> \n"
	   << "    <label for=\"" << m.id  <<"\">Your response:</label> \n"
	   << "    <input type=\"text\" id=\"responseText" 
	   << m.id << "\" maxlength=65 size=60/> \n"
	   << "    [ <a href=\"#\" onClick=\"submitReply(this)\">submit</a> ] \n"
	   << "    [ <a href=\"#\" onClick=\"cancelReply(this)\">cancel</a> ] \n"
	   << "  </div> \n"
	   << "</div> \n"; /* close the div we opened for Message Output above */

    if ( indent )
      output << "</li>\n";

    /* output << m.sender << ": " << m.body 
       << " children: " << m.childMessages.size() << endl; */

    return output.str();
  }

  /* 
   * listMessages(id, indent=0)
   * When id=0, list ALL messages... 
   * otherwise, list a given message and all of its children
   */
  string listMessages(int id = 0, int indent=0, 
		      bool showReply=true, bool showDelete=true)
  {
    stringstream output;
    map<int,message>::iterator it;
    vector<int>::iterator child;
    message* m;

    if ( id == 0) // doing ALL messages that respond to 0
      { // do this in reverse...
	for (it = msgList.begin(); it != msgList.end(); it++)
	  if ( it->second.inResponseTo == 0 )
	    output << listMessages( it->first, indent, showReply, showDelete );
      }
    else // doing a SPECIFIC message
      {
	m = &msgList[ id ];

	output << "<!-- message " << id << " -->\n"
	       << listMessage(*m, indent, showReply, showDelete);

	/* Process message children */
	if ( m->childMessages.size() )
	  {
	    output << "<ul><!-- opening for " << id << "--> \n";    
	    
	    for (child = m->childMessages.begin(); 
		 child != m->childMessages.end(); 
		 child++)
	      output << listMessages( *child, indent+1, showReply, showDelete );
	    
	    output << "</ul> <!-- closing for " << id << "--> \n";
	  }

      }
    
    return output.str();
  }
};

/*****************************************************************************
 * Permissions Stuff
 ****************************************************************************/

/*
 * This interface may be used for any of the message box entity's belonging
 * to a given username. 
 * special entities are ALL & OWNER.  Default permissions are loaded from 
 * perms.default and over-ridden by specific permissions
 *
 * It is our job to make sure all permissions are defined in the defaults
 * file. Those are used _unless_ the user overrides them manually.
 */

#define DEFAULT_PERMISSIONS_FILE "./default.perms"
#define PERMISSIONS_PATH "./"
#define PERMISSIONS_SUFFIX ".perms"

class messagePermissions {
private:
  string owner,
    entity;
  char pflags;

  /* 
   * read our on-file permissions...
   * returns the permissions for a given entity for requestor,
   * OWNER if unspecified. */

  char read(string entity="", string requestor="")
  {
    char defaultPermission = 0;
    string fentity, permission;

    if ( entity == "" )
      entity = this->entity;

    ifstream ifl;
    
    /*
     * read defaults first
     */
    
    ifl.open( DEFAULT_PERMISSIONS_FILE );
    
    if (!ifl.is_open())
      return -1;
    
    // handle ALL: and OWNER: defaults correctly.
    string searchName;
    if ( requestor == this->owner )
      searchName = "OWNER";
    else
      searchName = "ALL";

    while ( !ifl.eof() )
      {
	string line, username;
	std::getline(ifl, line);
	stringstream ss(line);

	/*
	 * ondisk format:
	 USER:ENTITY:PERMISSION\n
	*/

	std::getline(ss, username, ':');
	std::getline(ss, fentity, ':');
	std::getline(ss, permission);

	if ( fentity == entity && username == searchName )
	  {
	    // we've found it!
	    defaultPermission = permission[0];
	    break;
	  }
      }

    ifl.close();

    // permissions for an entity with no requestor are for the owner.
    if ( requestor == "" )
      requestor = this->owner;
    
    /*
     * Read the owners file now
     */

    string filename = PERMISSIONS_PATH + this->owner + PERMISSIONS_SUFFIX;
    ifl.open( filename.c_str() );

    if ( !ifl.is_open() )
      {
	// for now, just give the default...
	return defaultPermission;
      }

    if ( owner != requestor )
      searchName = requestor;

    while (!ifl.eof())
      {
	string line,username;
	fentity = "";
	std::getline(ifl, line);
	stringstream ss(line);

	std::getline(ss, username, ':');
	std::getline(ss, fentity, ':');
	std::getline(ss, permission);
	
	if (fentity == entity && username == searchName)
	  { // we've found it!
	    ifl.close();
	    return permission[0];
	  }
      }
    ifl.close();

    /* If we make it this far, no permissions. */
    return defaultPermission;


  }

public:
  char READ;
  char WRITE;
  char EXECUTE;
  /*
   * CONSTUCTOR - this username is the OWNER of the entity in question
   */
  messagePermissions(string username, string entity)
  {
    READ    = 0x04;
    WRITE   = 0x02;
    EXECUTE = 0x01;
    
    this->owner = username;
    this->entity   = entity;
  }
  /* 
   * REQUESTOR is whoever is trying to do something, not always the owner
   */
  void setPermission(char pflags,string requestor="", string entity="")
  {
    if ( requestor == "" ) 
      requestor = this->owner;
    if ( entity == "" )
      entity = this->entity;

    cout << "User: " << requestor << " Perm: " << (int)get(requestor, entity);
  } 
  /* 
   * REQUESTOR is whoever is trying to do an action
   */
  char get(string requestor="", string entity="")
  {
    if ( requestor == "" )
      requestor = this->owner;
    if ( entity == "" )
      entity = this->entity;

    return this->EXECUTE | this->WRITE | this->READ;
  }
  char set(char pflags, string requestor="", string entity="")
  {
    if ( requestor == "" )
      requestor = this->owner;
    if ( entity == "" ) 
      entity = this->entity;

    return get(entity, requestor);
  }
};

/*
  managePermissions perms("shaun", "guestbook"); // owner, entity

  perms.get("mike"); // requestor for entity (shaun's guestbook)

  // returns hex flag that can be compared with perms.READ and perms.WRITE
  if ( perms.get("mike") & perms.WRITE )
    cout << " You can write! \n";
  else 
    cout << " No permission! \n";
*/


/*******************************************************************************
 * BLACKLIST!
 ******************************************************************************/

class blacklist
{
private:

  vector<string> bl, userlist;
  string owner, entity;

  /******** 
   * readBlacklist();
   *******/

  int readBlacklist()
  {
    /* 
       this->owner && this->entity are defined for us at this point.
    */

    /*
      SELECT affects 
      FROM 270_social_message_blacklist 
      WHERE entity='public' 
        AND owner = 'meyersh' 
	AND affects IN ('ALL', 'shaun');
    */

    return 0;
  }

  /********
   * writeBlacklist();
   *******/

  int writeBlacklist()
  {
    return 0;
  }

public:
  /********
   * clear() - erase all banned users from this entity...
   *******/
  void clear()
  {

    /*
      DELETE FROM 270_social_message_blacklist
      WHERE owner = 'meyersh' 
        AND entity = 'public';
    */

    char sql[255];
    sprintf(sql,
	    "DELETE FROM 270_social_message_blacklist \
             WHERE owner = '%s' AND entity = '%s'",
	    this->owner.c_str(), this->entity.c_str());

    mysql_free_result(result(sql));

  }

  /********
   * add( string ) - add a user to the blacklist
   *******/

  void add( string username )
  {
    /*
      REPLACE INTO 270_social_message_blacklist
      (owner, entity, affects)
      values('meyersh', 'public', 'username');
    */
    
    char sql[255];
    sprintf(sql, 
	    "REPLACE INTO 270_social_message_blacklist \
             (owner, entity, affects) \
             values('%s', '%s', '%s')",
	    this->owner.c_str(), this->entity.c_str(),
	    username.c_str());

    mysql_free_result(result(sql));

    
  }

  /********
   * pop( string ) - remove a user from blacklist
   *******/

  void pop( string username )
  {
    /*
      DELETE FROM 270_social_message_blacklist
      WHERE owner = 'owner' 
        AND entity = 'entity'
	AND affects = 'username';
    */

    char sql[255];
    sprintf(sql, 
	    "DELETE FROM 270_social_message_blacklist \
             WHERE owner = '%s' \
               AND entity = '%s' \
               AND affects = '%s'",
	    this->owner.c_str(), this->entity.c_str(),
	    username.c_str());

    mysql_free_result(result(sql));

  }

  /********
   * size()
   *******/

  int size()
  {
    /*
      SELECT COUNT(*) FROM 270_social_message_blacklist
      WHERE owner = 'owner' and entity = 'entity';
    */
    char sql[255];
    sprintf(sql,
	    "SELECT * FROM 270_social_message_blacklist \
             WHERE owner = '%s' and entity = '%s'",
	    this->owner.c_str(), this->entity.c_str());

    MYSQL_RES *res = result(sql);
    int size = mysql_num_rows(res);
    mysql_free_result(res);

    return size;
  }

  /********
   * Subscript overloading
   *******/

  string& operator[](const int idex)
  {
    string foo = "LEGACY";
    return foo;
  }

  /********
   * find( string ) - return the index of result, or -1 if NULL
   *******/

  int find(string search)
  {
    return -1;
  }

  /********
   * has( string ) - return true if string is a member
   *******/

  bool has(string search)
  {

    /*
      SELECT affects 
      FROM 270_social_message_blacklist 
      WHERE entity='public' 
        AND owner = 'meyersh' 
	AND affects IN ('ALL', 'shaun');
    */

    char sql[255];
    sprintf(sql,
	    "SELECT affects \
             FROM 270_social_message_blacklist \
             WHERE entity='%s' \
              AND owner = '%s' \
              AND affects IN ('ALL', '%s')",
	    this->entity.c_str(), this->owner.c_str(),
	    search.c_str());

    MYSQL_RES *res = result(sql);
    int num_results = mysql_num_rows(res);
    mysql_free_result(res);
    if (num_results)
      return true;
    else
      return false;
	
  }


  /********
   * renderCSV() - return a csv of our blacklist vector
   *******/

  string renderCSV()
  {
    stringstream ss;
    
    /*
      SELECT affects 
      FROM 270_social_message_blacklist 
      WHERE entity='public' 
        AND owner = 'meyersh' 
	AND affects IN ('ALL', 'shaun');
    */

    char sql[255];
    sprintf(sql,
	    "SELECT affects \
             FROM 270_social_message_blacklist \
             WHERE entity='%s' \
              AND owner = '%s'",
	    this->entity.c_str(), this->owner.c_str());

    MYSQL_RES *res = result(sql);
    MYSQL_ROW row;

    bool first = true;


    while ((row = mysql_fetch_row(res)) != NULL)
      {
	if (row[0] == NULL)
	  continue;
	
	if (first != true)
	  ss << ',';
	else
	  first = false;

	ss << row[0];
      }
              

    return ss.str();
    
  }
  /********
   * CONSTRUCTOR
   *******/

  blacklist(string owner, string entity) 
  {
    this->owner = owner;
    this->entity = entity;
    readBlacklist();
  }

  /********
   * DESTRUCTOR
   *******/

  ~blacklist()
  {
    writeBlacklist();
  }

};


/*******************************************************************************
 * DRIVER
 ******************************************************************************/

#ifdef DRIVER

int
main()
{
  /*  message m;
  m.id = 2;
  m.inResponseTo = 1;
  m.sender = "shaun";
  m.timestamp = 23323223;
  m.body = "I agree!!"; */

  msgs ms("shaun");
  //cout << ms.size() << endl;
  //ms.insert( m );
  //  cout << "Number of Messages: " << ms.size() << endl;

  //cout << ms.listMessages() << endl;
  //cout << ms[7].sender << endl;

  blacklist bl("shaun", "whiteboard");
  bl.clear();
  bl.add("husmann");
  bl.add("shaun");
  bl.add("mike");

  cout << bl.renderCSV() << endl;
  //bl.pop("husmann");
  return 0;
}

#endif

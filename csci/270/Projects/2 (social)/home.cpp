/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * Updated for Project 2 in CSCI-270-001
 * Shaun Meyer, Oct 2010
 *
 * File: home.cpp 
 * Purpose: Handle all AJAX / JS Requests
 *
 * How:
 * All authentication & session handling is done by helper modules
 * cookies.hpp and session.hpp which free's us to merely process 
 * requests here.
 *
 * All requests are POSTed in key=value sets with one record per line.
 * When the need arises these POSTs may be url-encoded to prevent errors
 * from newlines or delimiters in the `value` field.
 *
 * An expected attribute to be in every REQUEST object ( from cookies.hpp )
 * is `action` which conveys what needs to happen and basically causes a function
 * here to be executed.
 * 
 * all data to be returned is appended to the 'output' stringstream object 
 * until the end of the program when it outputed to stdout with appropriate headers.
 *
 */
 
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <cstdlib> // for atof
using namespace std;

#include "cookies.hpp"
#include "session.hpp"

#include "shaunscgi.hpp"
#include "messaging.cpp"

#define REGISTRATION_FILE "/home/meyersh/private/user_registrations.txt"

int
main()
{
  /* Initialize our reponse & request objects */

  HttpResponse response;
  response.contentType = "text/plain";

  HttpRequest request;
  setHttpRequestMeta(request);
  getCookies(request);

  stringstream output;

  /*****************************************************************
   * Populate `sessionkey` with our session key from either POST or
   * GET methods for later on 
   ****************************************************************/

  string sessionkey = "" ;

  if ( hasCookie( request, "ShaunSession" ) 
       && checkSession( request.cookies["ShaunSession"] )
       )
    sessionkey = request.cookies["ShaunSession"];
  else if ( request.GET.find("ShaunSession") != request.GET.end() 
	    && checkSession( request.GET["ShaunSession"] ) )
    sessionkey = request.GET["ShaunSession"];
  
  /* Now, when sessionkey == "" it is invalid. */



  /**********************************************
   * REQUEST_METHOD == "POST"
   *********************************************/

  if ( request.META["REQUEST_METHOD"] == "POST" )
    {
  
      if ( sessionkey.length() == 0 )
	{
	  output << "error=Invalid session key. Please " 
		 << urlEncode(" [ <a href=\"login.cgi\">login</a> ]")<< "\n";
	  cout << renderHttpHeaders(response);
	  cout << output.str();
	  return 0;
	}
      

      /******************************************************************
       * At this point in the POST only valid sessionkey's are available
       * in 'sessionkey'. We've died, otherwise.
       *****************************************************************/
  
  
      /* Only respond to requests if they are logged in users... */
      if ( request.META["REQUEST_METHOD"] == "POST" 
	   || (request.hasGET("action")
	       && request.hasGET("ShaunSession") 
	       ) 
	   )
	{
	  
	  // Perform a bit of switching so we have the right action variable...
	  string *action = &request.POST["action"];
	  
	  /*
	   * Helper: 
	   * Now, down below, if username is SPECIFIED by POST it will be set
	   * to that value, otherwise it is set to the currently logged in 
	   * user. Always assume they're talking about themselves unless 
	   * they EXPLICITLY tell us otherwise.

	   * Furthermore, we're setting the string loggedOnUser to represent
	   * what you'd expect. This will save some redundancy down below.
	   *
	   */

	  string username = "";
	  string loggedOnUser = readSession( sessionkey );

	  if (request.hasPOST("username"))
	    username = request.POST["username"];

	  if ( username == "" )
	    username = loggedOnUser;

	  /*********************************************************************
	   * START OF `action=` processing block.
	   ********************************************************************/

	  /*********************************************************************
	   * `action=loadprofile`
	   * this will also work on people other than the logged user.
	   * in that case, password is not returned (for obvious reasons!)
	   */
	  
	  if ( *action == "loadprofile" )
	    {
	      userProfile profile;
	      bool same_user = false;
	      string loggedInUser = readSession ( sessionkey );
	      
	      
	      if (username == loggedInUser)
		readProfile( loggedInUser, profile );
	      else
		readProfile( username, profile );
	      
	      blacklist bl(username, "whiteboard");
	      msgs ms( username, "whiteboard");

	      output << "username=" << profile.username << endl
		     << "realname=" << profile.realname << endl
		     << "email=" << profile.email << endl
		     << "shoe_size=" << profile.shoe_size << endl
		     << "favorite_movie=" << profile.favorite_movie << endl
		     << "userlist=" << urlEncode(listUsers()) << endl
		     << "sessionkey=" << sessionkey << endl;

	      if ( bl.has(loggedInUser) )
		output << "whiteboard=" << username 
		       << " is PREVENTING you from viewing his "
		       << "or her whiteboard.\n";
	      else
		output << "whiteboard=" << urlEncode(ms[ms.size()].body) << endl;
	      
	      if (loggedInUser == username)
		{
		  output << "password=" << profile.password << endl;
		  output << "editable=true\n";
		}
	      else
		{
		  output << "password=secret\n";
		  output << "editable=false\n";
		}
	      
	    }

	  /*******************************************************************
	   * `action=listusersCSV`
	   */

	  else if ( *action == "listUsersCSV" )
	    {
	      output << "userlist=" << urlEncode(listUsers(true)) << endl;
	    }

	  /*******************************************************************
	   * `action=update`
	   * update a profile with information from webform
	   */
	  
	  else if ( *action == "update" )
	    {
	      if ( username != readSession( sessionkey ) )
		{
		  output << "error=You may only edit your own profile!\n";
		}
	      else
		{
		  userProfile profile;
		  if (readProfile(username, profile) == 0)
		    {

		      /* This is a relic from when I temporarily had to handle this
		       * via GET method (my original design was POST-only.) */

		      map<string,string> method;
		      if ( request.META["REQUEST_METHOD"] == "POST" )
			method.insert(request.POST.begin(), request.POST.end());
		      else
			method.insert(request.GET.begin(), request.GET.end());
		      
		      /*
		       * A lot of ifs, but allows us to 
		       * selectively update our profile piecemeal 
		       * if we please.
		       */

		      if ( request.hasPOST("password") )
			profile.password = method["password"];
		      if ( request.hasPOST("email") )
			profile.email = method["email"];
		      if ( request.hasPOST("realname") )
			profile.realname = method["realname"];
		      if ( request.hasPOST("favorite_movie") )
			profile.favorite_movie = method["favorite_movie"];
		      if ( request.hasPOST("shoe_size") ) 
			profile.shoe_size = atof( method["shoe_size"].c_str() );
		      if ( request.hasPOST("whiteboard") )
			profile.whiteboard = method["whiteboard"];
		      
		      
		      if (writeProfile(profile))
			output << "error=Error writing profile!\n";
		      else
			output << "notice=Profile succesfully updated.\n";
		      
		    }
		  else
		    {
		      output << "error=Unable to read profile for user (" 
			     << username << ")\n";
		    }
		}
	    }

	  /************************************************************ 
	   * `action=readPublicMessages`
	   * read out the HTML for our public messages body...
	   */

	  else if ( *action == "readPublicMessages" )
	    {
	      if ( username == "" )
		username = readSession( sessionkey );
	      msgs ms(username, "public");
	      output << "messages=" << urlEncode( ms.listMessages() ) << endl;
	      output << "username=" << username << endl
		     << "userlist=" << urlEncode(listUsers()) << endl;
	    }

	  /************************************************************
	   * `action=writePublicMessage`
	   * write on a persons public board
	   */

	  else if ( *action == "writePublicMessage" )
	    {
	      // TODO: ADD ERROR CHECKING!!! 

	      /* This requires a 
	       * Valid username to write to
	       * valid in-response-to
	       */

	      message m;
	      m.inResponseTo = atoi( request.POST["inResponseTo"].c_str() );
	      m.sender = readSession( sessionkey );
	      m.body = request.POST["body"];
	      msgs ms(request.POST["toUsername"], "public");
	      ms.insert( m );
	      ms.writeMessages();
	      
	    }

	  /************************************************************ 
	   * `action=deletePublicMessage
	   * Delete any message from your own door
	   * or delete any message you've posted on any door.
	   */

	  else if ( *action == "deletePublicMessage" )
	    {
	      string loggedInUser = readSession( sessionkey );

	      msgs ms( username, "public" );
	      int id = atoi ( request.POST["messageId"].c_str() );

	      if ( ms.hasMessage( id ) )  
		{
		  // message exists. do permission stuff.
		  if ( username == loggedInUser )
		    ms.remove( id, 0 );
		  else if ( loggedInUser == ms[id].sender )
		    ms.remove( id, 0 ); // non-recursively remove THIS ONE message.
		}
	      ms.writeMessages();
	      output << "notice=Deleting post " << id << ".\n";
	      
	    }

	  /*************************************************************
	   * `action=readPrivateMessages`
	   * display all private messages we have access to...
	   */

	  else if ( *action == "readPrivateMessages" )
	    {
	      stringstream ss;
	      string loggedOnUser = readSession( sessionkey );
	      if ( username == "" )
		username = loggedOnUser;

	      msgs ms(username, "private" );
	      for (int i=0; i < ms.size(); i++)
		{
		  /* if ( !ms.hasMessage(i) )
		     continue; // no garantees the ID's are contiguous */
		   if ( ms[i].inResponseTo == 0 && 
		       (ms[i].sender == loggedOnUser 
			|| username == loggedOnUser) 
		       ) 
		    ss << ms.listMessages(i);
		}
	      output << "messages=" << urlEncode( ss.str() ) << endl;
	      output << "username=" << username << endl
		     << "userlist=" << urlEncode(listUsers()) << endl;
	    }

	  /*************************************************************
	   * `action=writePrivateMessage` 
	   * Write a private message to someone...
	   */

	  else if ( *action == "writePrivateMessage" )
	    {
	      // TODO: ADD ERROR CHECKING!!! 

	      /* This requires a 
	       * Valid username to write to
	       * valid in-response-to
	       */
	      
	      message m;
	      m.inResponseTo = atoi( request.POST["inResponseTo"].c_str() );
	      m.sender = readSession( sessionkey );
	      m.body = request.POST["body"];
	      msgs ms(request.POST["toUsername"], "private");
	      ms.insert( m );
	      ms.writeMessages();
	    }

	  /**************************************************************
	   * `action=deletePrivateMessage`
	   * Delete any private message from your door, or your 
	   * message(s) from another persons door.
	   */
	  else if ( *action == "deletePrivateMessage" )
	    {
	      string loggedInUser = readSession( sessionkey );

	      msgs ms( username, "private" );
	      int id = atoi ( request.POST["messageId"].c_str() );

	      if ( ms.hasMessage( id ) )  
		{
		  // message exists. do permission stuff.
		  if ( username == loggedInUser )
		    ms.remove( id, 0 );
		  else if ( loggedInUser == ms[id].sender )
		    ms.remove( id, 0 ); // non-recursively remove THIS ONE message.
		}
	      ms.writeMessages();
	      output << "notice=Deleting post " << id << ".\n";
	    }
	  
	  /***************************************************************
	   * `action=getWhiteboard`
	   */

	  else if ( *action == "getWhiteboard" )
	    {
	      /* This has to be in reverse order (so newest first)
	       * and we don't need reply functionality so we'll only show
	       * parent messages. easy? */
	      string loggedInUser = readSession ( sessionkey );
	      if ( username == "" )
		username = loggedInUser;

	      blacklist bl(username, "whiteboard");
	      msgs ms( username, "whiteboard");

	      if ( bl.has( loggedInUser ) )
		output << "error=You're not allowed to see this whiteboard!\n";
	      else
		{
		  output << "messages=";
		  for ( int i = ms.size(); i > 0; i--)
		    {
		      if ( username == loggedInUser ) // show [ delete ] button
			output << urlEncode( ms.listMessages( i, 0, false, true ) );
		      else // hide [delete button]
			output << urlEncode( ms.listMessages( i, 0, false, false ) );
		    }
		  output << endl;
		}
	    }

	  /************************************************************* 
	   * `action=writeWhiteboard`
	   */

	  else if ( *action == "writeWhiteboard" )
	    {
	      string loggedInUser = readSession( sessionkey );

	      /* default to whoever we're logged in as */
	      if ( username == "" )
		username = loggedInUser;
	      
	      msgs ms( username, "whiteboard");
	      message m;

	      m.inResponseTo = atoi( request.POST["inResponseTo"].c_str() );
	      m.sender = readSession( sessionkey );
	      m.body = request.POST["body"];
	      if ( username == loggedInUser )
		ms.insert( m );

	    }

	  /*********************************************************** 
	   * `action=deleteWhiteboardMessage`
	   */
	  else if ( *action == "deleteWhiteboardMessage" )
	    {

	      string loggedInUser = readSession( sessionkey );

	      msgs ms( username, "whiteboard" );
	      int id = atoi ( request.POST["messageId"].c_str() );

	      if ( ms.hasMessage( id ) )  
		ms.remove( id, 0 ); 
	      
	      output << "notice=Deleting post " << id << ".\n";

	    }

	  /*********************************************************** 
	   * `action=getBanned`
	   * return a list of banned users and total users...
	   */

	  else if ( *action == "getBannedWhiteboard" )
	    {
	      string loggedInUser = readSession( sessionkey );
	      blacklist bl(loggedInUser, "whiteboard");

	      output << "userlist=" << urlEncode(listUsers(true)) << endl;
	      output << "bannedusers=" << bl.renderCSV() << endl;
	      
	    }

	  /***********************************************************
	   * `action=banUsers`
	   * ban provided list of banned users
	   */

	  else if ( *action == "banUsers" )
	    {

	      string loggedInUser = readSession ( sessionkey );
	      blacklist bl(loggedInUser, "whiteboard");
	      vector<string> bannedFromPage;


	      vector<string>::iterator it;

	      bl.clear();  /* unban everyone in there since I don't know
			      which have changed from before */

	      if ( request.hasPOST("bannedusers") 
		   && request.POST["bannedusers"] != "1" )
		{

		  // Split the line on Commas

		  stringstream ss(request.POST["bannedusers"]);
		  string user;

		  while (std::getline(ss, user, ','))
		    {
		      if ( user != "" )
			bannedFromPage.push_back(user);
		    }

		  it = bannedFromPage.begin();

		  for ( it; it != bannedFromPage.end(); it++ )
			bl.add( *it );

		  output << "error=Settings saved. " 
			 << bl.size() << " users banned." << endl;
		}
	      else output << "error=No one is currently banned.";

	    }
	}
    } /* END OF POST BLOCK */

  /*******************************************************************
   * if the CGI is requested via `GET` return them to the *.html page.
   */
  
  else if ( request.META["REQUEST_METHOD"] == "GET" )
    {
      cout << "location:/~meyersh/270/social/profile.html\n\n";
      //output << "You're not logged in!\n";
    }
  
  /* 
   * if they POST with out a valid session, return an error that the
   * AJAX/JavaScript will display
   */

  else if ( request.META["REQUEST_METHOD"] == "POST" )
    {
      output << "error=" 
	     << urlEncode("You're not logged in! [<a href='login.cgi'>login</a>]")
	     << endl;
    }

  else {
    // DRIVER MODE
    cout << "Driver mode detected!\n\n";

    stringstream ss;
    string loggedOnUser = "meyersh";
    string username = "meyersh";
    
    msgs ms("meyersh", "private" );
    
    bool author = false;
    bool owner = false;
    
    if (loggedOnUser == username)
      owner = true;


    for (int i=0; i < ms.size(); i++)
      {
	/* if ( !ms.hasMessage(i) )
	   continue; // no garantees the ID's are contiguous */
	if (ms[i].sender == loggedOnUser)
	  author = true;
	else
	  author = false;

	if ( ms[i].inResponseTo == 0 && 
	     (author || owner) 
	     ) 
	  ss << ms.listMessages(i);
      }
    output << "messages=" << urlEncode( ss.str() ) << endl;
    output << "username=" << username << endl
	   << "userlist=" << urlEncode(listUsers()) << endl;
  }
  /**************************************
   * FINALLY - Return any output we've collected
   *************************************/

  cout << renderHttpHeaders(response);
  cout << output.str();

  ofstream logfile ("/home/meyersh/error_log.txt");
  logfile << renderHttpHeaders(response);
  logfile << output.str() << endl;
  logfile.close();

  return 0;
}

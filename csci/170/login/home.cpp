
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

string
listUsers()
{
  stringstream output;
  ifstream usersfile;
  usersfile.open("/home/meyersh/private/user_registrations.txt");

  string user;

  while ( std::getline(usersfile, user) )
	{
	  output << "<a onClick='showProfile(\"" << user << "\");' href='#'>" << user << "</a>, ";
	}

  return output.str();
}

int
main()
{
  
  HttpResponse response;
  response.contentType = "text/plain";

  HttpRequest request;
  setHttpRequestMeta(request);
  getCookies(request);

  stringstream output;

  /* If I read closer, I'd know that we're to handle GET session keys...
   * Since I didn't, I'm writing this to support both GET & POST
   * flavors...
   */

  string sessionkey;
  
  if ( hasCookie( request, "ShaunSession")
	   && checkSession( request.cookies["ShaunSession"] )
	   )
	sessionkey = request.cookies["ShaunSession"];
  else if ( request.GET.find("ShaunSession") != request.GET.end()
			&& checkSession( request.GET["ShaunSession"] )
			)
	{
	  sessionkey = request.GET["ShaunSession"];
	  /* output << "error=" << checkSession( sessionkey ) << endl;
	  cout << renderHttpHeaders(response);
	  cout << output.str();
	  return 0; */
	}
  else
	sessionkey = "";

  /* Now we can catch empty / invalid session keys */

  if ( sessionkey.length() == 0 )
	{
	  output << "error=Invalid session key.\n";
	  cout << renderHttpHeaders(response);
	  cout << output.str();
	  return 0;
	}

  /******************************************************************
   * At this point in the game only valid sessionkey's are available
   * in 'sessionkey'.
   *****************************************************************/
  
  
  /* Only respond to requests if they are logged in users... */
  if ( request.META["REQUEST_METHOD"] == "POST" 
       || (request.hasGET("action")
	   && request.hasGET("ShaunSession") 
	   ) 
       )
    {

      // Perform a bit of switching so we have the right action variable...
      string action;
      if ( request.META["REQUEST_METHOD"] == "POST" )
	action = request.POST["action"];
      else
	action = request.GET["action"];

      string username;
      if (request.hasPOST("username"))
	username = request.POST["username"];
      else if (request.hasGET("username"))
	username = request.GET["username"];
      else
	username = "";

      /* 
       * `action=loadprofile`
       * this will also work on people other than the logged user.
       * in that case, password is not returned (for obvious reasons!)
       */

	  if ( action == "loadprofile" )
		{
		  userProfile profile;
		  bool same_user = false;

		  if ( username != "" ) 
		    {
		      if ( username == readSession( sessionkey ) )
			same_user = true;
		      else
			same_user = false;
		    }
		  else if ( readSession( sessionkey ).size() ) // no username 
		    {
		      same_user = true;
		    }

		    if (same_user)
		      readProfile( readSession( sessionkey ), profile );
		    else
		      readProfile( username, profile );
			

		  /* 
		  if ( readSession( request.cookies["ShaunSession"] ).size() ) 
		       && readSession(request.cookies["ShaunSession"]) != request.POST["username"] ) // Logged on user
		    {
		      readProfile( readSession( request.cookies["ShaunSession"] ), profile );
		      same_user = true;
		    }
		  else
		    readProfile( request.POST["username"], profile );

		  */

		  output << "username=" << profile.username << endl
			 << "realname=" << profile.realname << endl
			 << "email=" << profile.email << endl
			 << "shoe_size=" << profile.shoe_size << endl
			 << "favorite_movie=" << profile.favorite_movie << endl
			 << "userlist=" << urlEncode(listUsers()) << endl
			 << "sessionkey=" << sessionkey << endl;

		  if (same_user)
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

	  /* 
	   * `action=update`
	   * update a profile with information from webform
	   */

	  else if ( action == "update" )
	    {
	      if ( username != readSession( sessionkey ) )
		{
		  output << "error=You may only edit your own profile!\n";
		}
	      else
		{
		  userProfile profile;
		  if (!readProfile(username, profile))
		    {
		      
		      map<string,string> method;
		      if ( request.META["REQUEST_METHOD"] == "POST" )
			method.insert(request.POST.begin(), request.POST.end());
		      else
			method.insert(request.GET.begin(), request.GET.end());


		      profile.password = method["password"];
		      profile.email = method["email"];
		      profile.realname = method["realname"];
		      profile.favorite_movie = method["favorite_movie"];
		      profile.shoe_size = atof( method["shoe_size"].c_str() );
		      
		      
		      if (writeProfile(profile))
			output << "error=Error writing profile!\n";
		      else
			output << "notice=Profile succesfully updated.\n";
		      
		    }
		  else
		    {
		      output << "error=Unable to read profile for user (" << username << ")\n";
		    }
		}
	    }
	}

  /*
   * if the CGI is requested via `GET` return them to the *.html page.
   */

  else if ( request.META["REQUEST_METHOD"] == "GET" && checkSession(sessionkey) )
    {
      cout << "location:/~meyersh/profile.html?ShaunSession=" << sessionkey << "\n\n";
      //output << "You're not logged in!\n";
    }

  /* 
   * if they POST with oht a valid session
   */

  else
    {
      output << "error=" << urlEncode("You're not logged in! [<a href='login.cgi'>login</a>]") << endl;
    }

  cout << renderHttpHeaders(response);
  cout << output.str();

  return 0;
}

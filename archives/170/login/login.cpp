#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>

/* for randomString() */
#include "randomStr.hpp"

/* Super-Awesome Session/User Framework codes */
#include "cookies.hpp"
#include "session.hpp"

#define REGISTRATION_FILE "/home/staff/meyersh/private/user_registrations.txt"
#define USER_SUFFIX ".user"
#define USER_PATH "/home/staff/meyersh/private/"

/*
 * Forcibly set the password for "username"
 */

void setPassword(string username, string password)
{
  userProfile profile;

  if (!readProfile(username, profile))
    {
      /* No error reading profile, continue */
      profile.password = password;
      writeProfile(profile);
    }
}

/* 
 * Compare the username / password combo with what we have 
 * on file.
 */

bool checkPassword(string username, string password)
{
  if ( password == "" )
    return false; // Sorry RMS

  userProfile profile;
  readProfile(username, profile);

  if ( password == profile.password)
    return true;
  else
    return false;

}

/* 
 * returns True if a given username is already registered. 
 */

bool 
isRegistered(string username)
{
  string user;
  bool retval = false;

  ifstream r;
  r.open(REGISTRATION_FILE);

  while (std::getline(r, user))
    {
      if (username == user)
	{
	  retval = true;
	  break;
	}
    }
  r.close();
  return retval;
}

/*
 * Add a user to the registration Database
 */

void registerUser(string username, string password)
{
  ofstream regfile;
  string filepath = USER_PATH + username + USER_SUFFIX;
  
  regfile.open(REGISTRATION_FILE, ios::app);
  setPassword(username, password); /* This will be removed soon */
  regfile << username << endl;
  regfile.close();
}

string 
returnHtmlFile(string filename)
{
  ifstream html;
  string line, page;
  html.open(filename.c_str());
  while ( std::getline(html, line, '\n') )
    {
      page += line + "\n";
    }
  html.close();
  return page;
}

int
main()
{
  // string REQUEST_METHOD = getenv("REQUEST_METHOD");


  /* put together the request data */
  struct HttpRequest request;

  setHttpRequestMeta(request);
  getCookies(request); // render our cookies


  /* How will we be responding? */
  struct HttpResponse response;
  response.contentType = "text/html";

  stringstream output;

  // renderHttpHeaders(response);

  if ( request.META["REQUEST_METHOD"] == "GET" )
    {
      if ( hasCookie(request, "ShaunSession" ) && 
	   checkSession( request.cookies["ShaunSession"] ) )
	/* if we have a `next' in our GET, redirect to that URL */
	if ( request.GET.find("next") != request.GET.end() )
	  cout << "Location: " << request.GET["next"] << "\n\n";
	else
	  cout << "Location: " << "https://gimli.morningside.edu/~meyersh/home.cgi" << "\n\n";
      else
	output << returnHtmlFile("/home/staff/meyersh/public_html/login.html");
    }
  else if ( request.META["REQUEST_METHOD"] == "POST" )
    {
      if (request.POST["action"] == "register")
	{
	  if (isRegistered( request.POST["username"] ))
	    output << "error=That name is already registered.\n";
	  else
	    {
	      if ( request.POST["password"].size() > 7 )
		{
		  /* password OK, username not taken. lets register them! */
		  registerUser( request.POST["username"], request.POST["password"] );
		
		  /* A little tom-foolery to write the profile we recieved... */
		  userProfile profile;
		  profile.realname = request.POST["realname"];
		  profile.username = request.POST["username"];
		  profile.email = request.POST["email"];
		  profile.favorite_movie = request.POST["favorite_movie"];
		  profile.shoe_size = atof( request.POST["shoe_size"].c_str() );
		  profile.password = request.POST["password"];
		  
		  if ( writeProfile( profile ) )
		    output << "error=Error Writing Profile!!\n";
		  else /* reg. successful, send them to the login page. */
		    {
		      //setCookie(response, "ShaunSession", newSession(request.POST["username"]) );
		      output << "action=refresh\n";
		    }
		}
	      else
		output << "error=Password too short!\n";
	    }
	}
      else if (request.POST["action"] == "login")
	{
	  if (checkPassword( request.POST["username"], request.POST["password"] ) )
	    {
	      //output << "error=Valid Password\n";
	      setCookie(response, "ShaunSession", newSession(request.POST["username"]) );
	      output << "session=" << response.cookies["ShaunSession"] << endl;

	      /* Forward to "?next=" if available, otherwise refresh. */
	      if ( request.GET.find("next") != request.GET.end() )
		output << "redirect=" << request.GET["next"] << "\n";
	      else
		output << "action=refresh\n";

	    }
	  else
	    output << "error=Invalid Password\n";
	}
      else
	{
	  output << "error=Invalid action POSTed...\n";
	}
    }
  else
    {
      cout << "In DRIVER mode, you must be at the command-line...\n\n";
      //newSession("shaun");
      //cout << getSessionAge("noM4SxWdbfPxxgpa6KeeuDCCcpUYFVGLzkqwQPoCfjoBPIfxTI14ZyxKSlaIu61tvChdujo1Jf745Tyz2bjnpZLGicTauMWsYGxyZrpOApS65KeBW1m6kyKKtwSZdn2F") << endl << time(NULL) << endl;
      //checkAllSessions();
      // setCookie(response, "ShaunSession", newSession("meyersh"));
      // output << "session=" << response.cookies["ShaunSession"] << "\n";

      userProfile shaun;

      cout << readProfile("shaun", shaun) << endl
	   << "Username:   " << shaun.username << endl
	   << "Real Name:  " << shaun.realname << endl
	   << "E-Mail:     " << shaun.email << endl
	   << "Shoe Size:  " << shaun.shoe_size << endl
	   << "Fav. Movie: " << shaun.favorite_movie << endl;

      setPassword("shaun", "password");
      cout << "Check password: ";

      if ( checkPassword(shaun.username, "no") )
	cout << "OK\n";
      else
	cout << "FAIL.\n";
    }
  
  cout << renderHttpHeaders(response);
  cout << output.str();

  return 0;
}

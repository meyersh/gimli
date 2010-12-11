/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: login.cpp
 * Purpose: Handle login & registrations.
 *
 * How:
 * All authentication and user profile business is handled
 * by my modules, session.cpp and cookies.cpp. This just adds
 * a few functions to manipulate user files.
 */

#include <iostream>
#include <string>
#include <fstream>

#include <cstdlib>

/* for randomString() */
#include "randomStr.hpp"

/* Super-Awesome Session/User Framework codes */
#include "cookies.hpp"
#include "session.hpp"

#include "database.hpp"

#define REGISTRATION_FILE "/home/meyersh/private/user_registrations.txt"
#define USER_SUFFIX ".user"
#define USER_PATH "/home/meyersh/private/"

/*
 * Forcibly set the password for "username"
 */

void setPassword(string username, string password)
{
  userProfile profile;

  if (readProfile(username, profile) == 0)
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
  userProfile p;
  if (readProfile(username, p))
    return false;
  return true;
}

/*
 * Add a user to the registration Database
 * (no longer valid since we're using a db.)
 */

void registerUser(string username, string password)
{
  return;
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
	  cout << "Location: " 
	       << "https://gimli.morningside.edu/~meyersh/270/social/home.cgi" 
	       << "\n\n";
      else
	output << returnHtmlFile("/home/meyersh/public_html/270/social/login.html");
    }
  else if ( request.META["REQUEST_METHOD"] == "POST" )
    {
      if (request.POST["action"] == "register")
	{
	  if (isRegistered( request.POST["username"] ))
	    output << "error=That name is already registered.\n";
	  else if ( request.POST["password"].size() > 7 )
	    {
	      /* A little tom-foolery to write the profile we recieved... */
	      userProfile profile;
	      profile.username = request.POST["username"];
	      profile.firstname = request.POST["firstname"];
	      profile.lastname = request.POST["lastname"];
	      profile.email = request.POST["email"];
	      profile.favorite_movie = request.POST["favorite_movie"];
	      profile.shoe_size = atof( request.POST["shoe_size"].c_str() );
	      profile.password = request.POST["password"];
	      profile.statement = "";
	      
	      if ( writeProfile( profile ) )
		output << "error=Error Writing Profile!!\n";
	      else /* reg. successful, send them to the login page. */
		{
		  /* Randy wanted register to make people login(again):
		     setCookie(response, "ShaunSession", 
		     newSession(request.POST["username"]) ); 
		  */
		  output << "action=refresh\n";
		}
	    }
	  else
	    output << "error=Password too short!\n";
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
	  else /* password not valid */
	    output << "error=Invalid Password\n";
	}
      else /* Invalid action= posted. */
	{
	  output << "error=Invalid action POSTed...\n";
	}
    }
  else /* DRIVER Mode (no REQUEST_METHOD) */
    {
      cout << "In DRIVER mode, you must be at the command-line...\n\n";
      //newSession("shaun");
      //cout << getSessionAge("noM4SxWdbfPxxgpa6KeeuDCCcpUYFVGLzkqwQPoCfjoBPIfxTI14ZyxKSlaIu61tvChdujo1Jf745Tyz2bjnpZLGicTauMWsYGxyZrpOApS65KeBW1m6kyKKtwSZdn2F") << endl << time(NULL) << endl;
      //checkAllSessions();
      // setCookie(response, "ShaunSession", newSession("meyersh"));
      // output << "session=" << response.cookies["ShaunSession"] << "\n";

      userProfile shaun;
      cout << "Reading profile..\n";
      cout << readProfile("meyersh", shaun) << endl
	   << "Username:   " << shaun.username << endl
	   << "First Name:  " << shaun.firstname << endl
	   << "Last Name:  " << shaun.lastname << endl
	   << "E-Mail:     " << shaun.email << endl
	   << "Shoe Size:  " << shaun.shoe_size << endl
	   << "Fav. Movie: " << shaun.favorite_movie << endl
	   << "Statement: " << shaun.statement << endl;

      cout << "Testing password reset...\n";
      string password = shaun.password;

      setPassword("meyersh", "password"); 

      /* put password back the way it was...: */
      setPassword("meyersh", password);

      cout << "checking password... ";
      if ( checkPassword(shaun.username, shaun.password) )
	cout << "OK\n";
      else
	cout << "FAIL.\n";

      cout << "Testing writeProfile()...\n";
      cout << writeProfile(shaun) << endl;
    }
  
  cout << renderHttpHeaders(response);
  cout << output.str();

  return 0;
}

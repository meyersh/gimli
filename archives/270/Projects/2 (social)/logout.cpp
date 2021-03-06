/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: logout.cpp
 * Purpose: Remove a users session when they visit this *.cgi page
 *
 * How: Using the session & cookies frameworks, merely call the 
 * remove-session function.
 */

#include <string>
#include <iostream>
#include <map>

using namespace std;

#include "cookies.hpp"
#include "session.hpp"

int
main()
{
  /* Begin the RESPONSE stuff */
  HttpResponse response;

  /* Get the REQUEST stuff */
  HttpRequest request;
  setHttpRequestMeta(request);
  getCookies(request);

  stringstream output;

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
	}

  else
	sessionkey = "";


  if (checkSession( sessionkey ) )
    rmSession( sessionkey );
  
  output << "<html><body>\n" 
	 << "Logged out. <a href='login.cgi'>Login again</a> <a href='checksession.cgi'>Verify session</a>\n"
	 << "<br>" << sessionkey << endl
	 << "</body></html>\n";

  cout << renderHttpHeaders(response);
  cout << output.str();

  return 0;
}

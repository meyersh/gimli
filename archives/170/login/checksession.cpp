#include <string>
#include <iostream>
#include <map>

using namespace std;

#include "cookies.hpp"
#include "session.hpp"

int
main()
{
  
  HttpResponse response;

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

  /* if (hasCookie( request, "ShaunSession") )
     checkSession( request.cookies["ShaunSession"] ); */

  output << "<html><body>\n";
  if ( checkSession(sessionkey) )
    {
      userProfile profile;
      readProfile( readSession( request.cookies["ShaunSession"] ), profile );
      output << "You have a session! <a href='logout.cgi'>[logout]</a><br>\n"
	     << "<small>" << request.cookies["ShaunSession"] << "</small><br>\n"
	     << "Session age: " << getSessionAge( request.cookies["ShaunSession"] ) << "<br>\n";

      output << "hasGET: " << request.hasGET("ShaunSession") << " <br/>\n";
      output << "<p>Profile stuff:<br>\n"
	     << profile.realname << "<br>\n"
	     << profile.email << "<br>\n"
	     << profile.shoe_size << "<br>\n"
	     << profile.favorite_movie << "<br>\n"
	     << profile.username << "<br>\n</p>";
    }
  else 
    output << "Sorry, no session here. <a href='login.cgi'>[login]</a>";

  output << "</body></html>\n";

  cout << renderHttpHeaders(response);
  cout << output.str();

  return 0;
}

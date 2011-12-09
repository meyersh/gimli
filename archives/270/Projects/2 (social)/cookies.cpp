/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: cookies.cpp
 *
 * I wrote this when I was still learning. If I were to use it again in the 
 * future I was objectify it so that a lot of this gets done when I 
 * construct an object.
 */

#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include <cstdlib> // for getenv
#include <unistd.h> // for environmental variable struct
#include "cookies.hpp"

using namespace std;

/* parse and seperate key=value pairs on the equal sign. */
pair<string,string> getKeyValue(string mypair)
{
  string key,val;
  stringstream ss(mypair);
  std::getline(ss, key, '=');
  std::getline(ss, val, '\0');
  if (val == "")
    val = "1";
  pair <string,string> p(key,val);
  return p;
}

void 
setHttpRequestMeta(HttpRequest &request)
{
  char **ep;
  string key,val;
  extern char** environ;

  for (ep = environ; *ep; ep++)
    {
      stringstream ss(*ep);
      std::getline(ss, key, '=');
      std::getline(ss, val, '\0');
      request.META.insert( pair<string,string>(key, val) );
    }
  
  /* unpack POST data. In my case, I send it as key=val pairs. */
  if (request.META["REQUEST_METHOD"] == "POST")
    {
      string line;
      while (std::getline(cin, line, '\n'))
		{
		  request.POST.insert(getKeyValue(line));
		}
    }

  /* unpack GET data. This will be key=val pairs so we can put
     it in a map */

  if (request.META["QUERY_STRING"].size())
    { // pair<string,string> getKeyValue(string mypair); .insert(pair);
      string tok; 
      stringstream getstream(request.META["QUERY_STRING"]);

      while (std::getline(getstream, tok, '&'))
		{
		  request.GET.insert( getKeyValue( tok ) );
		}
    }

}

void 
setCookie(HttpResponse& response, string key, string val)
{
  pair<string,string> cookie(key,val);
  response.cookies.insert( cookie );
}

string 
returnCookieHeaders(HttpResponse& response)
{
  // we ASSUME we have cookies since it's checked for elsewhere...
  stringstream output;

  output << "Set-Cookie: ";
  map<string, string>::iterator it;
  for (it = response.cookies.begin(); it != response.cookies.end(); it++)
    {
      output << (*it).first << "=" << (*it).second << "; ";
    }
  output << "\n\n";
  return output.str();
}

/*
 * Parse and separate out the QUERY_STRING environmental
 * variable into a map. 
 *
 * ALGORITHM:
 * Step one: split key=value pairs at the '&'
 * Step two: split *that* at the '=' sign.
 * Step three: urlDecode what we've got. 
 */

void
getGET(HttpRequest& request)
{
  cout << "NULL";
}

string
renderHttpHeaders(HttpResponse& response)
{
  stringstream output;

  if ( response.headers.size() )
    {
      map<string,string>::iterator it;
      for (it = response.headers.begin(); it != response.headers.end(); it++)
	{
	  output << it->first << ": " << it->second << "\r\n";
	}
    }

  // this will soon be moved into the above MAP. 
  output << "Content-Type: " << response.contentType << '\n';

  if ( response.cookies.size() == 0 )
    output << '\n';
  else
    output << returnCookieHeaders(response) << "\n\n";

  return output.str();
}


/*
 * For debugging purposes, prints out all KEY=ATTR pairs in the map.
 */

void
printCookies(map<string, string> cookies)
{
  cout << "Content-Type: text/html\n";
  cout << "Set-Cookie: SES=XZY123\n\n";
  map<string, string>::iterator it;
  for (it=cookies.begin(); it != cookies.end(); it++)
    {
      cout << (*it).first << "=" << (*it).second << endl;
    }
}

/*
 * HTTP_COOKIE looks something like "KEY=VALUE; KEY=VALUE;...; ..."
 */

void
getCookies(HttpRequest& request)
{
  //map<string,string> cookies;

  /* do we have any cookies? */
  //HTTP_COOKIE = getenv("HTTP_COOKIE");
  
  if (request.META["HTTP_COOKIE"].size() != 0  )
    {

      string cookie, key, attr;
      stringstream ss;
      ss.str(request.META["HTTP_COOKIE"]);
      while (std::getline(ss, cookie, ';'))
	{
	  stringstream cookiestream;
	  /* Sometimes there is a leading space... */
	  if (cookie[0] == ' ')
	    cookie.erase(0, 1);
	  cookiestream.str(cookie);
	  std::getline(cookiestream, key, '=');
	  std::getline(cookiestream, attr, '=');
	  request.cookies[key] = attr;
	}
    }
 }

/*
 * Check for a given key in our map of cookies...
 */

bool hasCookie(HttpRequest &request, string cookie) 
{
  if (request.cookies.find(cookie) == request.cookies.end())
    return false;
  else
    return true;
}


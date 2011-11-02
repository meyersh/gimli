/*******************************************************************************
 * Final Project for CSCI-170-001
 * Shaun Meyer, Apr 2010
 *
 * File: cookies.hpp
 *
 * I wrote this when I was still learning. If I were to use it again in the 
 * future I was objectify it so that a lot of this gets done when I 
 * construct an object.
 */



#ifndef COOKIES
#define COOKIES

#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include <cstdlib> // for getenv

using namespace std;


/* Structures */

struct HttpResponse {
  map<string, string> cookies;
  string contentType;
  map<string, string> headers;
};

struct HttpRequest {
  map<string, string> cookies;
  map<string, string> META; /* this will hold our env variables later */
  map<string, string> POST; /* All POST data in key / value pairs. */
  map<string,string> GET; /* All GET data in key / value pairs. */
  map<string,string> session; /* Our session middleware! */

  bool hasGET(string search)
  {
    if ( GET.find(search) != GET.end() )
      return true;
    return false;
  }

  bool hasPOST(string search)
  {
    if (POST.find(search) != POST.end() )
      return true;
    return false;
  }

  bool hasCookie(string search)
  {
    if (cookies.find(search) != cookies.end() )
      return true;
    return false;
  }
};


/* Prototypes */

void setHttpRequestMeta(HttpRequest &request);
void setCookie(HttpResponse &Response, string key, string val);
string returnCookieHeaders(HttpResponse& response);
string renderHttpHeaders(HttpResponse& response);
void printCookies(map<string, string> cookies);
void getCookies(HttpRequest &request);
bool hasCookie(HttpRequest &cookies, string cookie);

#endif

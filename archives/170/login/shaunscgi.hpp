#ifndef SHAUNCGIHPP
#define SHAUNCGIHPP

#include <iostream>
#include <string>
#include <sstream>

#include <cctype>
#include <iomanip>
#include <cstdlib> // for atoi
using namespace std;

/* Prototypes */
string urlDecode(string);
string urlEncode(string);
char hexToChar(string);

/* hexToChar(string hexstring) - Convert something like "FF" to it ascii
   character equivalent. */

char hexToChar(string hexstring)
{
  int x;
  stringstream ss;
  ss << std::hex << hexstring;
  ss >> x;
  return (char)x;
}

string
urlDecode(string urlencoded)
{
  /* URL Encoding seems to be %<hex> so all we should have
   * to do is replace %<hex> with &#x<hex>; to output these
   * characters in HTML.
   */
  string output;
  string x;
  
  for (int i = 0; i < urlencoded.length(); i++)
	{
	  if (urlencoded[i] == '%')
		{

		  // grab the character from the two-digit HEX in the the string
		  x = hexToChar(urlencoded.substr(i+1, 2));
			
		  // ERASE the %01 characters...
		  urlencoded.erase(i, 3);

		  // INSERT the character literal that we're replacing
		  urlencoded.insert(i, x);
		  
		}
	  else if (urlencoded[i] == '+')
		{
		  urlencoded[i] = ' ';
		}

	}
  return urlencoded;
  
}

string
urlEncode(string url)
{
  /*
   * URL Encoding...
   * 
   * A-Z, a-z, 0-9, '.', '-', '~', and '_' are left as-is
   * SPACE becomes '+'
   * ALL Other characters become %FF for their Hex representation of
   * the ascii codes.
   */

  string::iterator it;
  stringstream replacement, buffer;

  for (it = url.begin(); it != url.end(); it++)
    {
      if ( isalnum( *it ) || *it == '.' || *it == '-' || 
	   *it == '~' || *it == '_' )
	{
	  buffer << *it;
	}
      else if ( *it == ' ' )
	{
	  buffer << '+';
	}
      else /* for all else, replace with '%FF'-hex (rfc1738 says 
			* lower-case is OK. ) */
	{
	  replacement.str("");
	  replacement << "%" << std::hex << static_cast<int>(*it);
	  buffer << replacement.str();
	}
    }
  return buffer.str();
      
}

#endif

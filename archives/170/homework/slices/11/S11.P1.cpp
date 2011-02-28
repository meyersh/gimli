/*
 * Slice 11, Project 1
 * Shaun Meyer, Mar 2010
 * 
 * Take HTML POST data and dump what recieved, outputting non-printable
 * characters (space included) as HEX <b>[AF]</b>
 *
 * Escape HTML-significant characters < > for instance with &lt; and
 * &gt; etc etc... (which in practice, we refer to them by their HEX/Ascii
 * codes... &#xFF;)
 *
 */

#include <cctype>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include "shaunscgi.h" // urlDecode() urlEncode()
#include "randyficator.h" // parseInput()
using namespace std;

#define MAX_LINE_LENGTH 75
#define HTML_TITLE "S11.P1 - POST DUMP TOOL"

/* Prototypes */
void htmlHeader();
void htmlFooter();
void displayEnv();

const char* methods[] = {
  "AUTH_TYPE",
  "CONTENT_LENGTH",
  "CONTENT_TYPE",
  "DOCUMENT_ROOT",
  "GATEWAY_INTERFACE",
  "PATH_INFO",
  "PATH_TRANSLATED",
  "QUERY_STRING",
  "REMOTE_ADDR",
  "REMOTE_HOST",
  "REMOTE_IDENT",
  "REQUEST_METHOD",
  "SCRIPT_NAME",
  "SERVER_NAME",
  "SERVER_PORT",
  "SERVER_PROTOCOL",
  "SERVER_SOFTWARE",
  "HTTP_ACCEPT",
  "HTTP_ACCEPT_CHARSET",
  "HTTP_ACCEPT_ENCODING",
  "HTTP_ACCEPT_LANGUAGE",
  "HTTP_COOKIE",
  "HTTP_FROM",
  "HTTP_HOST",
  "HTTP_REFERER",
  "HTTP_USER_AGENT",
  "HTTPS",
  '\0'};

/*
 * INPUT: Any POST from a webpage. (std::getline)
 *
 * ALGORITHM:
 * Break lines at 75 lines (but finish hex codes)
 * If ( (non-printable) || (blank) || (newline) )
 *   return HEX-CODE
 * ElIf ( (isalpha) || (isdigit) )
 *   return CHARACTER
 * ElIf ( is_special_html)
 *   return &asciiCode;
 * 
 * OUTPUT: 
 * Dump from ALGORITHM ABOVE.
 *
 */

int
main()
{
  char me;
  int linelen = 0;
  string input, output; // Our POST or GET input and our formatted output.
  //cin.get(me); // prime `me' character

  htmlHeader();
  
  string REQUEST_METHOD = getenv("REQUEST_METHOD");
  
  if ( REQUEST_METHOD == "POST") 
    {

	  cout << "<h3>POST METHOD!</h3>\n";
	  cout << hex;
	  getline(cin, input, '\0'); // get all characters until NULL.
	  cout << parseInput(input); // Return nice output
      cout << "<hr>";
      displayEnv();
    }
  else if ( REQUEST_METHOD == "GET" )
    {
      cout << "<h3>GET METHOD!</h3>\n";
      string QUERY_STRING = getenv("QUERY_STRING");

      if (QUERY_STRING.length())
		cout << "<h4>urlDecoded:</h4>\n"
			 << parseInput( urlDecode(QUERY_STRING) )
			 << "<h4>urlEncoded:</h4>\n"
			 << parseInput( QUERY_STRING );
	  else
		cout << "No Query string here!";

	  cout << "<hr>";
	  displayEnv();
    }
  else {
    cout << "Whoa, what happened?!?";
  }

  htmlFooter();
  return 0;
}

void 
htmlHeader() 
{
  cout << "Content-Type: text/html\n\n"
       << "<html>\n"
       << "  <head>\n"
       << "    <title>" << HTML_TITLE << "</title>\n"
       << "  </head>\n"
       << "<body>\n"
       << "<tt>\n\n";
}

void
htmlFooter()
{
  cout << "</tt>\n</body>\n</html>\n";
}

void
displayEnv()
{
  for (int i = 0; methods[i] != NULL; i++)
	{
	  cout << methods[i] << ": '";

	  if ( getenv(methods[i]) != NULL)
		cout << getenv( methods[i] );

	  cout << "'<br>" << endl;
	}
}

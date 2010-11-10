/*
 * Slice 10, Project 1
 * Shaun Meyer
 * 
 * Take HTML POST data and dump what recieved, outputting non-printable
 * characters (space included) as HEX <b>[AF]</b>
 *
 * Escape HTML-significant characters < > for instance with &lt; and
 * &gt; etc etc...
 *
 */

#include <cctype>
#include <iostream>
#include <iomanip>

using namespace std;

#define MAX_LINE_LENGTH 75
#define HTML_TITLE "S10.P1 - POST DUMP TOOL"

/* Prototypes */
void htmlHeader();
void htmlFooter();

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
  //cin.get(me); // prime `me' character

  htmlHeader();
  
  cout << hex;
  for ( cin.get(me); ( !cin.eof() ) || ( !cin.fail() ); cin.get(me) )
    {
      ;
      // Do stuff
      if ( (!isprint(me)) || (isspace(me)) ) 
	{
	  cout << "<b>[" << setfill('0') << setw(2) << static_cast<int>(me) << "]</b>";
	  linelen += 4;
	}
      else if ( isalnum(me) )
	{
	  cout << me << "";
	  linelen += 1;
	}
      else 
	{
	  cout << "&#x" << static_cast<int>(me) << ";";
	  linelen += 1;
	}
      
      if (linelen >= MAX_LINE_LENGTH)
	{
	  cout << "<br/>\n";
	  linelen=0;
	}
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

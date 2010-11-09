/*****************************************************************************
 * String Splitting (S9.E1.1)
 * Split a given string on ampersand (&) marks
 * Shaun Meyer, Feb 2010
 *
 ****************************************************************************/ 

#include <iostream>
#include <string>
#include <sstream> // for linestream

using namespace std;

/*************************************
 * INPUT: A single line of input
 *
 * ALGORITHM:
 *  I tried a few ways of doing this. The current implementation uses
 * a single for loop to iterate over the string looking for the delimiter
 * character. When it is found we use the std::string method substr(begin, n)
 * to copy end-begin characters from begin point in the string.
 *
 * Other methods used were std::getline with the modified delimiter and
 * Randy's nested For-For example that seems to work especially well.
 *
 * OUTPUT:
 * each "token" from the string.
 *
 */


/* Prototypes */
void shaunsSplit_for(string line, char delimiter);
void shaunsSplit_getline(string line, char delimiter);
void randysSplit_forfor(string line, char delimiter);

int
main()
{
  string line, token;
  char delimiter='&';
  int start=0, end=0;

  //      0         10        20        30
  //      0123456789012345678901234567890123456789
  
  //line = "One potato&two potato&three potato&four";

  cout << "Content-Type: text/plain\n\n";
  cout << "notice=Processing string...\n";
  cout << "clear=output\n";
  std::getline(cin, line);
  shaunsSplit_for(line, '&');

}	  
  
void
shaunsSplit_for(string line, char delimiter)
{
  int start, end=0;
  string token;
  
  for (start = 0; end <= line.size(); end++)
	{
	  if(line[end] == '&')
		{
		  token = line.substr(start, end - start);
		  cout << "output=" <<start << ":" << token << ":" << end <<endl;
		  start = end + 1;
		}
	  else if (end == line.size()) // EOL & still no delimiter.
		{
		  token = line.substr(start, end - start);
		  cout << "output=" << start << ":" << token << ":" << end << endl;
		}
	}
}

void
shaunsSplit_getline(string line, char delimiter)
{
  istringstream linestream(line); // convert line to a stream
  string token; 
  
  while ( std::getline(linestream, token, delimiter) )
	{
	  cout << token << endl;
	  if (token.find('=') != string::npos)
		shaunsSplit_getline(token, '=');
	}
  
}

void
randysSplit_forfor(string line, char delimiter)
{
  int start, end=0;
  string token;

  for (start = 0; start <= line.size(); start = end + 1 )
	{
	  for (end = start+1;
		   end <= line.size() && line[end] != delimiter;
		   end++)
		; /* Increment until we find delimiter or EOL */
	  
	  token = line.substr(start, end-start);
	  cout << start << ":" << token << ":" << end << endl;
	}
}

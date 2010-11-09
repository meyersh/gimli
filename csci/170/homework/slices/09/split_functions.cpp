#include <iostream>
#include <string>
#include <sstream> // for linestream

using namespace std;

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
  
  line = "One potato&two potato&three potato&four";

  while (line[0] != 'Q')
	{
	  cout << "Line length: " << line.size()-1 << endl;
	  cout << "Line: " << line << endl;
	  cout << endl << "Shauns for method:" << endl;
	  shaunsSplit_for(line, delimiter);
	  cout << "\nShauns getline method:\n";
	  shaunsSplit_getline(line, delimiter);
	  cout << "\nRandys nested loop method:\n";
	  randysSplit_forfor(line, delimiter);
	  cout << endl;

	  std::getline(cin, line);
	}
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
		  cout << start << ":" << token << ":" << end <<endl;
		  start = end + 1;
		}
	  else if (end == line.size()) // EOL & still no delimiter.
		{
		  token = line.substr(start, end - start);
		  cout << start << ":" << token << ":" << end << endl;
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

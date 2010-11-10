#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

int
main(int argc, char* argv[])
{
  string file;
  if (argc > 1)
	file = argv[1];
  else
	{
	  cout << "Usage: " << argv[0] << " <filename>" << endl;
	  exit(0);
	}

  struct stat stFileInfo;
  if ( stat(file.c_str(), &stFileInfo))
	{
	  cout << "invalid file\n";
	  exit(1);
	}

  ifstream f;
  f.open(file.c_str());

  char c;
  cout << "File begins:\n";
  for (f.get(c); !f.eof(); f.get(c))
	{
	  if (c != '\n')
		cout << c;
	  else
		cout << "\\n\n";
	}
  cout << "EOF\n";
	
  return 0;
}

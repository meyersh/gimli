#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;

#define DB_BASE "/home/meyersh/db/"

/* Prototypes */
bool fileExists(string file, string base=DB_BASE);
string readKey(string key, string base=DB_BASE);
string writeKey(string key, string value, string base=DB_BASE);
deque allKeys(string base=DB_BASE);

bool fileExists(string file, string base)
{
  struct stat stFileInfo;
  bool r;
  int intStat;
  file = base+file;

  intStat = stat(file.c_str(), &stFileInfo);
  
  if (intStat == 0)
	{
	  // able to get attr, file exists.
	  r = true;
	}
  else
	{
	  // unable to get attr, nopermission or no file.
	  r = false;
	}

  return r;
}

string
readKey(string key, string base)
{
  if (!fileExists(key, base))
	return "";

  ifstream f;
  string file = base+key;
  f.open(file.c_str());
  string value;
  std::getline(f, value, '\0');
  
  return value;
}

string
writeKey(string key, string value, string base)
{
  ofstream f; 
  string file = base+key;
  f.open(file.c_str());
  f << value;
  f.close();
  return value;
}

int
main()
{
  cout << fileExists("test") << endl;
  // cout << writeKey("test", "shaun") << endl;
  cout << readKey("test") << endl;
  return 0;
}

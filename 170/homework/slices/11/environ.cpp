#include <iostream>
#include <unistd.h>

using namespace std;

extern char **environ;

int
main (int argc, char* argv[])
{
  cout << "Content-Type: text/plain\n\n";
  
  char **ep;

  for (ep = environ; *ep; ep++)
	cout << *ep << endl;
  
  return 0;
}

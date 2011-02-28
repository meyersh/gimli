#include <iostream>

using namespace std;

void printFirst(const char*[]);

int
main()
{
  const char* foo[]={
	"Shaun",
	"Meyer"
  };

  printFirst(foo);
}

void printFirst(const char* foo[])
{
  cout << foo[0] << endl;
}

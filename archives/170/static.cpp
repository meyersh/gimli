#include <iostream>

using namespace std;

int foo(int = NULL);

int
main()
{
  foo(1);
  for (int i = 0; i<10; i++)
	{
	  foo();
	}
}

int foo(int x)
{
  static int fooy;
  if (x)
	fooy += x;
  else
	fooy++;
  
  cout << fooy << endl;
  return fooy;
}

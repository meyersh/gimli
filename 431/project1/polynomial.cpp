#include <iostream>
#include <string>
#include "polynomial.hpp"

using namespace std;

int main(int argc, char** argv)
{
   if (argc != 3)
	  {
	  cout << "SYNTAX: " << argv[0] << " <n> <polynomial>" << endl;
	  exit(0);
	  }

   string polynomial = argv[2];
   int n = atoi(argv[1]);
   cout << "n: " << n << endl
		<< "p: " << polynomial << endl
		<< "=: " << p(polynomial, n) << endl;
   

}

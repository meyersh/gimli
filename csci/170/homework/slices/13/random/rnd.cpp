#include <iostream>
#include "randomStr.hpp"
#include <cstdlib>

using namespace std;

int
main(int argc, char* argv[])
{
  int len; 

  if (argc == 2)
    len = atoi(argv[1]);
  else
    {
      cout << "How long? > ";
      cin >> len;
    }
  
  string randomness;
  
  randomString(randomness, len);

  cout << randomness << endl;

  return 0;
}

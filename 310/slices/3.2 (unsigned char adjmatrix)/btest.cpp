#include <iostream>
#include "adjMatrix.hpp"

using namespace std;

int main()
{

bitarray ba(8);
ba[0] = 1;
ba[1] = 0;
ba[2] = 1;

cout << 
  "0: " << ba[0] << endl <<
  "1: " << ba[1] << endl <<
  "2: " << ba[2] << endl;

 for (int i = 0; i < 3; i++)
   {
     bool foo = ba[i];
     cout << i << ": " << foo << endl;
   }

return 0;
}

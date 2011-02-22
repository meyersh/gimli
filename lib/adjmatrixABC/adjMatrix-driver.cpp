#include <iostream>
#include "adjMatrix.hpp"

using namespace std;

int main()
{
   cout << "Begin: \n";
   unweightedDirectedLST foo(5);
   foo.setBit(0,1);
   foo.setBit(2,3);
   foo.setBit(0,2);
   foo.setBit(0,3);
   foo.setBit(0,4);
   
   foo.print();
   
   cout << "ClearAll:\n";
   foo.clearAll();
   foo.print();

   cout << "SetAll:\n";
   foo.setAll();
   foo.print();
   cout << "After checkbit (should move to front)\n";
   foo.checkBit(4, 2);
   foo.print();
   cout << "End. \n";
   return 0;
}

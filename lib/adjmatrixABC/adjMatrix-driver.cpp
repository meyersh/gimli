#include <iostream>
#include "adjMatrix.hpp"

using namespace std;

int main()
{
   unweightedDirectedLST foo(5);

   /*
    * Test setting some bits
    */
   foo.setBit(0,1);
   foo.setBit(2,3);
   foo.setBit(0,2);
   foo.setBit(0,3);
   foo.setBit(0,4);
   
   foo.print();

   /*
    * Test checkbit (based on above inputs)
    */
   if (foo.checkBit(2,3))
      cout << "2,3 is checked.\n";
   if (!foo.checkBit(3,4))
      cout << "3,4 is not checked.\n";
   

   /*
    * Test clearAll()
    */
   cout << "ClearAll:\n";
   foo.clearAll();
   foo.print();

   /* 
    * Test setAll()
    */
   cout << "SetAll:\n";
   foo.setAll();
   foo.print();

   /*
    * Check that checkbit is moving checked bits to the start
    * of the list. (0 should be in order after this is done)
    */
   cout << "After checkbit (should move to front)\n";
   foo.checkBit(0, 3);
   foo.checkBit(0, 2);
   foo.checkBit(0, 1);
   foo.checkBit(0, 0);

   foo.print();

   cout << "El fin. \n";
   return 0;
}

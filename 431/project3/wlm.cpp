#include <iostream>
#include "wlm.hpp"

using namespace std;

int main() {

   //
   // States
   //

   // May be initialized with NO length.
   State b;     
   b.insert(1);
   b.insert(4);
   b.insert(2);
   b.insert(2);

   // Or we may initialize it with some arbitrary length.
   State b2(5); 
   for (int i = 0; i < 5; i++)
	  b2[i] = i;

   // b2[5] // this would segfault!

   cout << b[1] << endl;
   cout << b.toString() << endl;

   //
   // Weights
   //

   // Weights always requires a filename.
   Weight w("weights.txt");

   /* Non-zero indicates a file couldn't be loaded. Probably means 
	  we need to initialize some values. */
   if (w.load()) 
	  {
	  w.insert(1);
	  w.insert(3);
	  w.insert(4);
	  w.insert(5);
	  }

   cout << w[1] << endl;
   cout << w.toString() << endl;

   /* We haven't saved our weights object! That's OK, the 
	  destructor calls save() for us. Whew. */
   return 0;

}


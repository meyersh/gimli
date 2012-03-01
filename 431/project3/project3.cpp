#include <iostream>
#include "wlm.hpp"

using namespace std;

int main() {

   State b;
   b.insert(1);
   b.insert(4);
   b.insert(2);
   b.insert(2);

   cout << b[1] << endl;
   cout << b.toString() << endl;

   Weight w("weights.txt");
   if (w.load())
	  {
	  w.insert(1);
	  w.insert(3);
	  w.insert(4);
	  w.insert(5);
	  }

   cout << w[1] << endl;
   cout << w.toString() << endl;

   return 0;

}


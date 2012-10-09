/* Simple tests for set class 
   Shaun Meyer, Feb 2012
*/

#include <iostream>
#include "set.hpp"

using namespace std;

int main() {

   char_set set;

   cout << "This should be false: " << set.contains('s') << endl;
   
   set.insert('s');

   cout << "This should be true:  " << set.contains('s') << endl;

   cout << "This should be 's':   " << set.string() << endl;

   return 0;

}

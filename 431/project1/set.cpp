#include <iostream>
#include "set.hpp"

using namespace std;

int main() {

   char_set set;

   cout << set.contains('l') << endl;
   
   set.insert('l');

   cout << set.contains('l') << endl;

   cout << set.string() << endl;

   return 0;

}

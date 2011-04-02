#include <cstdlib>
#include <iostream>

#include "ctt.hpp"

using namespace std;

int main()
{
   ctt<int> foo;
   foo.insert("hi", 1);
   foo.insert("hello", 2);
   int index = 0;
   int ret = 0;
   if (ret = foo.getIndex("hi", index))
      cout << "Search failed.\n" << ret << endl;
   else
      cout << index << endl;

   return ret;
}

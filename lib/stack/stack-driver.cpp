#include <iostream>
#include "stack.hpp"

using namespace std;

int main()
{
   
   stack<char> my_stack; 
   stack<char>::stack_ptr item;

   my_stack.push('N');
   my_stack.push('U');
   my_stack.push('A');
   my_stack.push('H');
   my_stack.push('S');

   while (my_stack.size())
      {
      //item = my_stack.pop();
      cout << my_stack.pop()->value << endl;
      }

   return 0;
}

#include <iostream>
#include "queue.hpp"

using namespace std;

int main()
{
   
   queue<char> my_queue; 
   stack<char>::queue_ptr item;

   my_queue.push('N');
   my_queue.push('U');
   my_queue.push('A');
   my_queue.push('H');
   my_queue.push('S');

   while (my_queue.size())
      {
      item = my_queue.pop();
      cout << item->value << endl;
      }

   return 0;
}

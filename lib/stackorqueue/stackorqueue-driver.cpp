/******************************************************************************
 * stackorqueue-driver.cpp -
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * A quick test of of stackorqueue.hpp
 *****************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include "stackorqueue.hpp"

using namespace std;

int main(int argc, char **argv)
{
   stackorqueue<char> *my_container;

   vector<string> args(argv, argv+ argc);
   if (args.size() == 2)
      {
      if (args[1] == "queue")
	 my_container = new queue<char>;
      else if (args[1] == "stack")
	 my_container = new stack<char>;
      else 
	 exit(1);
      }
   else 
      exit(1);

   
   my_container->push('N');
   my_container->push('U');
   my_container->push('A');
   my_container->push('H');
   my_container->push('S');

   cout << "Size: " << my_container->size() << endl;

   while (my_container->size())
      {
      char item = my_container->pop();
      cout << item << endl;
      }

   delete my_container;

   return 0;
}

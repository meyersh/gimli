/*
Create a standalone application that takes one parameter. 

The parameter is the name of a file that has a bunch of integers in it, one per 
line. It should stop reading numbers when it reaches the end of the file or
encounters a blank line.

Once it reads the numbers in, the program should sort them using a radix sort on
the binary representation of the numbers. You may separate negative and non-
negative numbers for purposes of sorting and then combine them later. Once they
are sorted, they should be output to the screen, one per line.
*/

#include <iostream>
#include <fstream>
#include <cstdlib> // for atoi()
#include <ctime>  // for time(NULL)
#include <vector>
#include <string>
#include <queue>

using namespace std;

int main(int argc, char **argv)
{
   vector<string> args(argv, argv+argc);

   int numrandom = 10000;
   if (args.size() == 3 && args[1] == "--random")
     {
       numrandom = atoi(args[2].c_str());
     }
   else if (args.size() != 2)
      {
      cout << "Usage: " << args[0] << " <file-to-be-sorted> " << endl;
      cout << "Random generator: " << args[0] << " --random [numbers]\n";
      return 0;
      }
   
   string filename = args[1];
   ifstream infile(filename.c_str());

   if (args[1] == "--random")
     {
       srand(time(NULL));
       for (int i = 0; i < numrandom; i++)
	 {
	   cout << rand() << endl;
	 }
       return 0;
     }
   
   if (!infile)
      {
      cout << "Can't seem to open " << filename << endl;
      return 0;
      }

   vector<unsigned int> data; 

   string line;
   while (std::getline(infile, line))
      {
      if (line == "")
	 break;

      data.push_back(atoi(line.c_str()));
      }


   /*
    * Debugging: verify we've read in the file OK.
    */
   for (int i = 0; i < data.size(); i++)
      cout << i << ": " << data[i] << endl;

   /*
    * About now, we should probably sort...
    */

   /* buckets will hold our sorting. Element 0 is the 
      items with a 0 in the current digit, Element 1 
      will be the items with a 1. */
   vector< queue<int> > buckets(2);
   
   /* for each digit of an unsigned integer */
   for (int dig = 0; 
	dig < sizeof(unsigned int)*8; 
	dig++)
     {
       /* populate our buckets from the array based
	  on the value of the bit in `dig` digit. */
       for (int element = 0;
	    element < data.size();
	    element++)
	 {
	   if (data[element] & (1<<dig))
	     {
	       buckets[1].push(data[element]);
	     }
	   else
	     {
	       buckets[0].push(data[element]);
	     }
	 }
       /* unpack our buckets back into the array so we can look
	* at the next digit */
       for (int element = 0;
	    element < data.size();
	    element++)
	 if (buckets[0].size()) 
	   {
	     data[element] = buckets[0].front();
	     buckets[0].pop();
	   }
	 else
	   {
	     data[element] = buckets[1].front();
	     buckets[1].pop();
	   }
     }

   cout << "Sorted:\n";
   for (int i = 0; i < data.size(); i++)
     cout << i << ": " << data[i] << endl;

   return 0;

}

unsigned int nth(unsigned int n)
{
  return (1 << n);
}

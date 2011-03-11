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
#include <vector>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
   vector<string> args(argv, argv+argc);

   if (args.size() != 2)
      {
      cout << "Usage: " << args[0] << " <file-to-be-sorted> " << endl;
      return 0;
      }
   
   string filename = args[1];
   ifstream infile(filename.c_str());

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

   return 0;

}

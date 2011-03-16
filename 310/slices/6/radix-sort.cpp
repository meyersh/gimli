/*
  radix-sort.cpp
  CSCI 310 S6.E1

  CREATED BY  : Shaun Meyer
  SUBMITTED BY: Shaun Meyer
  CREATED     : 15 MAR 1011
  DESCRIPTION : Fulfills two purposes. First, if a filename is specified
  on the command line the file is read for integers (expected: one per line),
  the file is sorted using a radix method, and the results are printed.

  Alternatively, the --random parameter may be passed at the command line
  and the program will return the specified number of random numbers 
  (defaults to 5).
*/

#include <iostream>
#include <fstream>
#include <cstdlib> // for atoi()
#include <ctime>  // for time(NULL)
#include <vector>
#include <string>
#include <queue>

#define DEFAULT_RANDOM_NUMBERS 5

using std::vector;
using std::queue;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  /*******************
   * Process arguments 
   ******************/
  // place all arguments into a vector<string>
  vector<string> args(argv, argv+argc);

  int numrandom = DEFAULT_RANDOM_NUMBERS; /* default qty to return. */

  if (args.size() == 3 && args[1] == "--random")
    numrandom = atoi(args[2].c_str()); // number-of-numbers was given

  else if (args.size() != 2)
    /* Invalid args, give error and die */
    {
      cout << "Usage: " << args[0] << " <file-to-be-sorted> " << endl
	   << "Random generator: " << args[0] 
	   << " --random [number-of-numbers]\n";
      return 0;
    }
   
  /****************
   * --random mode
   ***************/

  if (args[1] == "--random")
    /* Output some random numbers and die */
    {
      srand(time(NULL));
      for (int i = 0; i < numrandom; i++)
	{
	  cout << rand() << endl;
	}
      return 0;
    }
   
  /********************************************************************
   * We're in regular-mode, go ahead and process input file for sorting
   *******************************************************************/

  ifstream infile(args[1].c_str());
  if (!infile)
    {
      cout << "Can't seem to open " << args[1] << endl;
      return 0;
    }

  vector<int> data; /* all integers for sorting */

  string line; // this represents one line from the file.
  while (std::getline(infile, line))
    {

      if (line == "")
	break; /* break for empty line as-per spec */
      
      data.push_back(atoi(line.c_str()));
    }


#ifdef DEBUG

  /* show data as read from file */
  for (int i = 0; i < data.size(); i++)
    cout << data[i] << endl;

#endif


  /****************
   * Begin sorting.
   ***************/

  /* buckets[] will hold our sorting. Element 0 is a 
     queue for items with a 0 in the current digit, Element 1 
     will be the queue for items with a 1. */

  vector< queue<int> > buckets(2);

  unsigned int neg_mask = 1 << (sizeof(unsigned int)*8-1);

  /* Loop for-each bit in an unsigned int (32 or 64) */
  for (int dig = 0; /* 0 is the right-most bit. */
       dig < sizeof(unsigned int)*8; 
       dig++)
    {

      /* populate queues from array based on `dig` bit */
      for (int element = 0;
	   element < data.size();
	   element++)
	{
	  bool negb = (neg_mask & data[element]) ? 0 : 1;

	  /* negb is some arithmetic tom-foolery. If a number is 
	     negative, the leading bit is 1, the "bigger" it is
	     and the further it is from zero. 

	     We check the first bit and remember it as negb (0/1).

	     If the number is negative, the negb bit will be set 1,
	     if it is positive, negb gets 0. This is used below to 
	     select between the ones bucket (queue) and the zeros bucket.
	  */

	  if (data[element] & (1<<dig))
	    buckets[negb].push(data[element]); // "ones" bucket[1]
	  else
	    buckets[!negb].push(data[element]); // "zeros" bucket[0]

	} // end for loop


      /* Put all elements from queues back into the data vector. */
      for (int element = 0;
	   element < data.size();
	   element++)
	{
	  if (buckets[0].size()) 
	    /* unpack the 0 queue first */
	    {
	      data[element] = buckets[0].front();
	      buckets[0].pop();
	    }
	  else
	    /* the 0 queue is empty, unpack the 1's queue */
	    {
	      data[element] = buckets[1].front();
	      buckets[1].pop();
	    }
	}
    }

#ifdef DEBUG
  cout << "Sorted:\n";
#endif 

  /* Print the data vector */
  for (int i = 0; i < data.size(); i++)
    cout << data[i] << endl;

  return 0; // bye!

}

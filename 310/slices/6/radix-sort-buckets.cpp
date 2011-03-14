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

/*
 * Prototypes
 */
void sift(vector<queue<unsigned int> > &buckets, 
	  unsigned int element, unsigned int dig);



int main(int argc, char **argv)
{
  // place all arguments into a vector<string>
  vector<string> args(argv, argv+argc);

  int numrandom = 10000; // default qty of random numbers to return.

  /*
   * Determine if we're in --random mode or if invalid
   * credentials have been passed. 
   */
  if (args.size() == 3 && args[1] == "--random")
    /* Run to return a quantity of random numbers */
    {
      numrandom = atoi(args[2].c_str());
    }
  else if (args.size() != 2)
    /* Invalid credentials */
    {
      cout << "Usage: " << args[0] << " <file-to-be-sorted> " << endl;
      cout << "Random generator: " << args[0] << " --random [numbers]\n";
      return 0;
    }
   
  string filename = args[1];
  ifstream infile(filename.c_str());

  
  if (args[1] == "--random")
    /* We're in --random mode; just output random numbers and die */
    {
      srand(time(NULL));
      for (int i = 0; i < numrandom; i++)
	{
	  cout << rand() << endl;
	}
      return 0;
    }
   
  /*
   * We're in regular-mode, go ahead and process input file for sorting
   */

  if (!infile)
    {
      cout << "Can't seem to open " << filename << endl;
      return 0;
    }

  vector<unsigned int> data; /* all integers for sorting */

  string line;
  while (std::getline(infile, line))
    {
      if (line == "")
	break; /* break for empty line as-per spec */
      
      data.push_back(atoi(line.c_str()));
    }


  /*
   * Debugging: print out the contents of 
   * our data vector. 
   */
  for (int i = 0; i < data.size(); i++)
    cout << i << ": " << data[i] << endl;
  cout << endl;

  /*
   * Begin sorting.
   */

  /* buckets[] will hold our sorting. Element 0 is a 
     queue for items with a 0 in the current digit, Element 1 
     will be the queue for items with a 1. */
  vector< queue<int> > buckets(2);

  /* Loop for-each bit in an unsigned int (32 or 64) */
  for (int dig = 0; /* 0 is the right-most bit. */
       dig < sizeof(unsigned int)*8; 
       dig++)
    {
      /* populate our buckets from the array based
	 on the value of the bit in `dig` digit. */
      if (!dig) // first pass, elements come from `data`
	for (int element = 0;
	     element < data.size();
	     element++)
	  {
	    /* We maintain a queue for every possible
	     * value at a given bit. The element to be sorted
	     * in our data vector goes into the queue based on 
	     * the value in the `dig` bit. 
	     */
	    if (data[element] & (1<<dig))
	      buckets[1].push(data[element]);
	    else
	      buckets[0].push(data[element]);
	  }
      else // all other passes, data already in buckets.
	{
	  int bucket_a = buckets[0].size();
	  int bucket_b = buckets[1].size();

	  for (int sz = bucket_a; sz; sz--, buckets[0].pop()) // for a bucket
	    sift(buckets, buckets[0].front(), dig);

	  for (int sz = bucket_b; sz; sz--, buckets[1].pop())
	    sift(buckets, buckets[1].front(), dig);
	    
	   
	}
    }

  /* Buckets are now populated with all elements. We now
   * put them all back into the `data` vector, popping all 
   * of the 0's into the array followed by all of the 1's. 
   * This will prepare us for the next bit.
   */
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
      else /* the 0 queue is empty, unpack the 1's queue */
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

void sift(vector<queue<unsigned int> > &buckets, 
	  unsigned int element, unsigned int dig)
{
  if (element & (1<<dig))
    buckets[1].push(element);
  else
    buckets[0].push(element);
}

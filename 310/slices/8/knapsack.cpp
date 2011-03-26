/*
  Create a standalone application that uses the max heap like the one we created in class to solve instances of the knapsack problem. The max heap (in addition to functioning correctly) should abide by the following conventions:

  * The name of the template should be maxHeapPQ
  * At a minimum, it should have the following operations as public functions:
  o void addItem(const A &item) This adds another item to the end of the heap and then uses sift-up to place it in the proper position.
  o int readLargest(A &item) This assigns the largest item in the heap to the parameter (item) but does not change the heap. It returns 0 on success, 1 if there is nothing in the heap.
  o int removeLargest(A &item) This assigns the largest item in the heap to the parameter (item) and then removes the largest items from the heap. It returns 0 on success, 1 if there is nothing in the heap.
  o int isEmpty() returns 1 if the heap is empty, 0 otherwise.
  o clear() clears out the heap so that it is empty.

  Your program should take one parameter, which is the name of the file defining the knapsack instance. This file should have two numbers per line, the first being the value of an item and the second the weight of the item. The numbers should be separated by one or more blanks. You may assume the file is appropriately formatted so that you can use extraction (>>) and not have to check input. You may assume that your program can read until it encounters EOF.

  Your program should output the combined value and combined weight of your solution. It should also output the indexes (starting at 0) of the items placed in the knapsack.

*/

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

#define KNAPSACK_SIZE 50

struct Item {
   unsigned int value, weight, max_value, level;
   vector<bool> selection;

   Item()
   {
      value = weight = max_value = level = 0;
   }

   Item(unsigned int value, unsigned int weight, unsigned int max_value)
   {
      this->value = value;
      this->weight = weight;
      this->max_value = max_value;
   }
   
   unsigned int density() const
   {
      if (value == 0 || weight == 0)
	 return 0;
      return value/weight;
   }

   bool operator< (const Item &b) const
   {
      if (this->density() < b.density()) 
	 return true;
      return false;
   }

   bool operator> (const Item &b) const
   {
      if (this->density() > b.density())
	 return true;
      return false;
   }
   /*
   bool operator== (const Item &b) const
   {
      if (this->density() == b.density())
	 return true;
      return false;
   }
   */
   
   string selection_str()
   {
      string ret;
      for (int i = 0; i < selection.size(); i++)
	 ret += selection[i];
      return ret;
   }

};

template<class V> class maxHeapPQ {
private:
   vector<V> heap; // ptr to root of heap 

   void swap(V &a, V &b)
   {
      V t = a;
      a = b;
      b = t;
   }

   void sift_up(int i=-1)
   {
      if (i == -1)
	 i = heap.size()-1;
      int parent = (i-1)/2;

      if (parent < 0)
	 return;

      if (heap[i] > heap[parent])
	 {
	 swap(heap[i], heap[parent]);
	 sift_up(parent);
	 }
   }

   void sift_down(int i=0)
   {
      if( i*2 > heap.size()-1 || i*2+1 > heap.size()-1 )
	 return;

      int leftChild = i*2+1,
	 rightChild = i*2+2;

	 /* Which child is biggest ? */
	 int bc = leftChild; // default to left child.
      if (heap[bc] < heap[rightChild])
	 bc = rightChild; // right child is bigger.
      
      /* If heap[bc] > heap[i], 
	 swap parent with biggest child. */
      if (heap[i] < heap[bc])
	 swap(heap[i], heap[bc]);

      // try to continue sifting down.
      sift_down(bc);
   }

public:
   void addItem(V &item)
   {
      heap.push_back(item);
      sift_up();
   }

   int readlargest(V &item) {
      if (heap.size())
	 item= heap[0];
      else 
	 return -1;
	 
      return 0;
   }
      
   int removeLargest(V &item)
   {
      if (heap.size()==0)
	 return -1;
      else if (heap.size() == 1)
	 {
	 item = heap[0];
	 heap.pop_back();
	 }
      else if (heap.size() > 1)
	 {
	 item = heap[0];
	 swap(heap[0], heap[heap.size()-1]);
	 heap.pop_back();
	 sift_down();
	 } 
      return 0;
   }

   int isEmpty() {return heap.empty();}
   void clear() {heap.clear();}
};

/***********************
 * HELPER FUNCTIONS
 **********************/

std::vector<std::string> split(const std::string line, 
                               const std::string split_pattern=",")
/* PARAMS     : string thing, string separator
   RETURNS    : vector of strings
   DESCRIPTION: Splits a string into pieces on `split_pattern` and returns a
   vector of these results. */
{
   std::vector<std::string> ret;
   int start,
      end,
      len = 0;

   for (start = 0; start < line.size(); start += len + 1)
      {
      len = 0;
      end = line.find(split_pattern, start);
      if (end == std::string::npos)
         {
         ret.push_back(line.substr(start, line.size()-start));
         break;
         }

      /* If we made it this far, we've got a match. */
      len = end - start;

      if (len < split_pattern.size())
         continue;

      ret.push_back(line.substr(start, len));

      }
   return ret;
}

unsigned int sum_value(vector<Item> items, unsigned int level)
{
   unsigned int sum = 0;
   for (int i = 0; i < items.size() || i < level; i++)
      sum += items[i].value;
   return sum;
}

unsigned int sum_weight(vector<Item> items, unsigned int level)
{
   unsigned int sum = 0;
   for (int i = 0; i < items.size() || i < level; i++)
      sum += items[i].weight;
   return sum;
}

unsigned int calculate_bound(vector<Item> items, unsigned int K, 
			     Item *currNode=NULL)
/* PARAMS     : a vector of Items, the capacity of the knapsack `K'.
   RETURNS    : an unsigned int representing the maximum value.
   DESCRIPTION: Given our items and K, deduce the maximum value bounds by 
   bounds = currentValue + Sv + (K-TotalSize)*(Vk/Sk)
*/
{
   unsigned int upperBound = 0;
   unsigned int current_weight = 0;

   vector<bool> selection;

   if (currNode != NULL)
      selection = currNode->selection;

   // presume any node not already in our selection is a possibility.
   while (selection.size() < items.size())
      selection.push_back(true);
   
   int i;
   for (i = 0; 
	i < items.size();
	i++)
      {
      /* filter out nodes if we've written them off in our 
	 selection process */
      if (selection[i] == false)
	 continue;

      current_weight += items[i].weight;
      upperBound += items[i].value;

      if ((current_weight + items[i+1].weight) > K)
	 break;
      }

   i++;
   
   /*
   // * compute fractional density.
   if (i < items.size())
      upperBound += items[i].density() * ( K - current_weight);
   */
   return upperBound;
}

string selection_str(vector<bool> selection)
/* PARAMS     : a vector of bool representing a binary selection
   RETURNS    : a string showing the binary 100101
   DESCRIPTION: Iterates through a vector to make a string/binary
   representation suitable for cout'ing */
{
   string ret;
   for (int i = 0; i < selection.size(); i++)
      ret += selection[i] ? '1' : '0';

   return ret;
}

/***********************
 * MAIN
 **********************/

int main(int argc, char **argv)
{
   /* Handle user parameters */
   vector<string> args(argv, argc+argv);
   if (args.size() != 2)
      {
      cout << "Usage: " << args[0] << " <weights file>\n";
      return -1;
      }

   ifstream ifile(args[1].c_str());

   if (!ifile)
      {
      cout << "Unable to open '" << args[1] << "'.\n";
      return -1;
      }

   /* Load up the file */

   vector<Item> items;

   string line="";
   while (std::getline(ifile, line))
      {
      Item* i = new Item;

      i->value = atoi(split(line, " ")[0].c_str());
      i->weight = atoi(split(line, " ")[1].c_str());

      items.push_back(*i);
      i = NULL;
      }

   /* Process the file */

   // *sort items by density*
   std::sort(items.rbegin(), items.rend());

   /* print out our items list for debugging */
   for (int i = 0; i < items.size(); i++)
      cout << i << ": " 
	   << items[i].value << "v "
	   << items[i].weight << "w " 
	   << items[i].density() << "d\n";

   maxHeapPQ<Item> prioq; // priority queue
   int best_so_far=-1;
   unsigned int best_value = 0;
   vector<bool> best_selection; // best selection thus far
   int weight = 0;

   /* Initialize before entering while() loop */
   items[0].max_value = calculate_bound(items, KNAPSACK_SIZE);
   Item initialnode; // this is all zip except for bounds.
   initialnode.max_value = calculate_bound(items, KNAPSACK_SIZE);
   prioq.addItem(initialnode);

   cout << "Initial max_value: " << items[0].max_value << endl;

   while (!prioq.isEmpty())
      {
      /* dequeue an item to process */
      Item currNode;
      prioq.removeLargest(currNode);

      if (currNode.max_value > best_value &&
	  currNode.weight <= KNAPSACK_SIZE) 
	 {
	 Item next_added = currNode;

	 next_added.selection.push_back(true);

	 // This is where we need to add the value of the next item
	 next_added.value += items[currNode.selection.size()].value;
	 next_added.weight += items[currNode.selection.size()].weight;

	 next_added.max_value = 
	    calculate_bound(items, KNAPSACK_SIZE, &next_added);

	 if (next_added.weight <= KNAPSACK_SIZE)
	    {
	    if (next_added.value > best_value)
	       {
	       best_value = next_added.value;
	       best_selection = next_added.selection;
	       }
	    if (next_added.max_value > best_value)
	       prioq.addItem(next_added);
	    }

	 Item next_not_added = currNode; // inherit weight+value
	 next_not_added.selection.push_back(false);
	 next_not_added.max_value = 
	    calculate_bound(items, KNAPSACK_SIZE, &next_not_added);

	 if (next_not_added.max_value > best_value)
	    {
	    prioq.addItem(next_not_added);
	    //	    best_selection = next_not_added.selection;
	    }
	 }

      cout << "level: " << currNode.selection.size()
	   << " item: " << currNode.weight << "w "
	   << currNode.value << "v "
	   << currNode.max_value << "b "
	   << "(sel: " << selection_str(currNode.selection)
	   << ") (best_value: " << best_value << ") "
	   << "(best sel: " << selection_str(best_selection) << ")\n";

      }
   cout << "sel: (" << best_selection.size() << ") '" 
	<< selection_str(best_selection) << "'\n";
   cout << "Best so far: " << best_value << endl;
   return 0;
}

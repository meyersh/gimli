/*******************************************************************************
 * CSCI-270 : S4.E1.1
 * Shaun Meyer
 * Sept, 2010
 ******************************************************************************/

#include <iostream> 
#include <vector>    /* going away when dynAry is fixed */
#include <algorithm> /* for sort(); */

using namespace std;

/* 
 * DynAry is a magical object that will do 
 * dynamic memory allocation to create a never-ending
 * array (vector-like) that uses push_back(element)
 * to append to the back. 
 * 
 * size & capacity methods exist for your viewing 
 * pleasure. 
 *
 * And you may access the array (unprotected) at your leasure
 * via the ary[] method. I have not overloaded the subscript
 * operators like I probably should.
 *
 * For this assignment we have to sort a series of numbers
 * at one point so I've added an additional trick to this
 * dynAry: 
 *
 * HEAPS! If, instead of push_back(i) you use insert_heap(i)
 * the array is kept in max-heap order.
 * Which allows the print_sorted() function to do what you'd 
 * expect, very effeciently. 
 *
 */


class dynAry
{
 public:
  int * ary; // our dynamic array pointer
  int arySize;
  int capacity;

  /***
   * constructor
   ***/

  dynAry(int newCapacity=1)
  {
    capacity = newCapacity;
    arySize = 0;
    ary = new int[capacity];
    #ifdef DEBUG 
    cout << "New dynAry constructed. Capacity: " << capacity << endl; 
    #endif
  }
  /***
   * destructor
   ***/

  ~dynAry()
  {
    delete [] ary;
  }

  /***
   * size() method - returns the current population of the array.
   ***/

  int size()
  {
    return arySize;
  }

  /*** 
   * overloaded subscript operators here
   ***/

  int& operator[] (int index)
  {
    return ary[index];
  }
  
  int& operator[] (int index) const
  {
    return ary[index];
  }

  /***
   * push_back(element) - append an element to the end of the array.
   * make room if you need to. 
   ***/

  void push_back(int element)
  {
    if (arySize >= capacity-1)
      enlarge();

    ary[arySize] = element;
    arySize++;
    
  }

  /*** 
   * insert_heap(element) - like push_back, but does a sift-up after.
   ***/

  void insert_heap(int element)
  {
    push_back(element); 
    sift_up(arySize-1, ary);
  }

  /*** 
   * swap(a, b)  - swap variable values
   **/

  void swap(int &a, int &b)
  {
    int t = a;
    a = b;
    b = t;
  }
  
  /***
   * swap(* *) - overloaded swap, swaps values of two pointers.
   ***/

  void swap(int *a, int *b)
  {
    int t = *a;
    *a = *b;
    *b = t;
  }
  /*** 
   * sift_up(i) - sift up starting at `i' node.
   ***/

  void sift_up(int i, int * heap_ary)
  {
    int parent = (i-1)/2;
    if (parent < 0)
      return;

    if (heap_ary[i] > heap_ary[parent])
      {
	swap(heap_ary[i], heap_ary[parent]);
	sift_up(parent, heap_ary);
      }
  }

  /*** 
   * sift_down(i) - sift down starting at `i' node.
   ***/

  void sift_down(int i, int hsize,  int * heap_ary)
  {
    if( i*2 > hsize-1 || i*2+1 > hsize-1)
      return;

    int * leftChild = &heap_ary[i*2+1];
    int * rightChild = &heap_ary[i*2+2];
    int * parent = &heap_ary[i];

    if (*parent < *leftChild && *parent < *rightChild)
      {
	if (*leftChild > *rightChild)
	  {
	    swap(parent, leftChild);
	    sift_down(leftChild - heap_ary, hsize, heap_ary);
	  }
	else 
	  {
	    swap(parent, rightChild);
	    sift_down(rightChild - heap_ary, hsize, heap_ary);
	  }
      }
    else if (*parent < *leftChild)
      {
	swap(parent, leftChild);
	sift_down(leftChild - heap_ary, hsize, heap_ary);
      }
    else if (*parent < *rightChild)
      {
	swap(parent, rightChild);
	sift_down(rightChild - heap_ary, hsize, heap_ary);
      }
  }

  /*** 
   * enlarge(newCapacity) - enlarge our array to newCapacity size, preserving
   * all the elements.
   ***/

  void enlarge(int newCapacity = 0)
  {
    int * newAry;
    if (newCapacity == 0)
      {
	newCapacity = capacity * 2;
	newAry = new int[newCapacity];
      }
    else if (newCapacity > arySize)
      {
	newAry = new int[newCapacity];
      }
    else return; // We're not making the array SMALLER.

    #ifdef DEBUG
    cout << "Resizing array to " << newCapacity << " elements.\n";
    print();
    #endif

    /* Copy all the elements to the new array */
    for (int i = 0; i < arySize; i++)
      newAry[i] = ary[i];
    
    delete [] ary;
    ary = newAry; // ary[] is now new and big.
    newAry = NULL;
    capacity = newCapacity; // update our records to reflect the new capacity.
  }

  void print()
  {
    for (int i = 0; i < arySize; i++)
   	cout << ary[i] << " ";
   
    cout << endl;
  }

  void print_sorted()
  {
    int * sorted_ary = new int[arySize];
    int ssize = arySize; // sorted_size or how many remaining elements to sort.
    
    // copy our heap for sorting:
    for (int i = 0; i < arySize; i++)
      sorted_ary[i] = ary[i]; 

    for (ssize = arySize; ssize > 0; ssize--)
      {
	swap(sorted_ary[0], sorted_ary[ssize-1]);
	sift_down(0, ssize-2, sorted_ary);
      }

    for (int i = 0; i< arySize; i++)
      cout << sorted_ary[i] << " ";
    cout << endl;

    delete [] sorted_ary;
  }
};

int findLowest(dynAry&);
int findHighest(dynAry&);
float calcAverage(dynAry&);

/*
 * ** If there are no scores it should return an error message saying that 
 *    there is no average because there are no scores. 
 * 
 * ** If there are 1-4 scores it should return the average of those 4 scores 
 *
 * ** If there are 5 or more scores throw away the top and the bottom score 
 *    and return the average of the rest. 
 * 
 * The scores brought into the CGI script should be converted to numbers 
 * and stored in a (dynamic array).
 */


int main()
{
  dynAry ary; 
  int i;

  /* get all the numbers to populate the vector */
  while (cin >> i)
    {
      ary.insert_heap(i);
      #ifdef DEBUG
      cout << "c" << ary.capacity <<  " s" << ary.size() << ": ";
      ary.print();
      #endif
    }

  cout << "Content-Type: text/plain\n\n";
  cout << "HEAP: ";
  ary.print();
  cout << "<br>SORT: ";
  ary.print_sorted();
  cout << "<br>MAX:  " << findHighest(ary) << endl;
  cout << "<br>MIN:  " << findLowest(ary) << endl;
  
  cout << "<br>AVG:  " << calcAverage(ary) << endl;

  return 0;
}

/*******
 ** END OF MAIN(), MORE FUNCTIONS BELOW
 ******/

/* 
 * return the value of the lowest element from a vector of integers. 
 */

int findLowest(dynAry& numbers)
{
  int lowest=numbers[0];
  for (int i=1; i < numbers.size(); i++)
    if (numbers[i] < lowest)
      lowest = numbers[i];

  return lowest;
}

/*
 * return the highest element from a vector of integers.
 */

int findHighest(dynAry& numbers)
{
  int max=numbers[0];
  for (int i=1; i< numbers.size(); i++)
    if (numbers[i] > max)
      max = numbers[i];

  return max;
}

/* return a float representing the average from a vector
 * of integers with the lowest value dropped. */

float calcAverage(dynAry &numbers)
{
  int sumdropped = 0;
  int numdropped = 0;

  if (numbers.size() > 4)
    {
      numdropped = 2; // highest & lowest
      sumdropped += findLowest(numbers) + findHighest(numbers);
    }


  int sum=0;

  for (int i = 0; i < numbers.size(); i++)
    sum += numbers[i];

  return (sum-sumdropped) / (numbers.size()-numdropped);

}


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

template<class V> class maxHeapPQ {
private:
   vector<V> heap;

   void swap(V &a, V &b)
   {
      V t = a;
      a = b;
      b = t;
   }

   void sift_up(int i=heap.size()-1)
   {
      int parent = (i-1)/2;

      if (parent < 0)
	 return;

      if (heap[i] > heap[parent])
	 {
	 swap(heap[i], heap[parent]);
	 sift_up(parent);
	 }
   }

   void sift_down(int i)
   {
      if( i*2 > heap.size()-1 || i*2+1 > heap.size()-1)
	 return;

      int leftChild = i*2+1,
	 rightChild = i*2+2,

      /* Which child is biggest ? */
      int bc = leftChild; // default to left child.
      if (heap[bc] < heap_ary[rightChild])
	 bc = rightChild; // right child is bigger.
      
      /* If heap[bc] > heap[i], 
	 swap parent with biggest child. */
      if (heap[i] < heap[bc])
	 swap(heap[i], heap[bc]);

      // try to continue sifting down.
      sift_down(bc);
   }

public:
   void addItem(const V &item)
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

   int isEmpty() {return !heap.size();}
   void clear() {heap.clear();}
};

struct Node {
   int value, 
      weight,
      maxvalue;
   Node()
   {
      value = weight = maxvalue = 0;
   }
};

int main()
{
   return 0;
}

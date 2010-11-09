#include <iostream>
#include <iomanip> // for setw()

using namespace std;

template<class A>
void print_array(A ary[], int size)
{
  for (int i = 0; i < size; i++)
    {
      cout << setw(3) << ary[i] << " ";
    }
  cout << endl;
}


template<class A> 
int binarySearch(const A ary[], int size, const A &key)
{

  int min, max, mid;

  min = 0;
  max = size-1;
  mid = max+min/2;

  while (max >= min)
    {
      if (ary[mid] == key)
	  return mid;
      else if (ary[mid] > key)
	{
	  min = mid+1;
	  mid = max+min/2;
	}
      else if (ary[mid] < key)
	{
	  max = mid-1;
	  mid = max+min/2;
	}
    }

  return -1;
}

template<class A>
int linearSearch(const A ary[], int size, const A &key)
{
  for (int i = 0; i < size; i++)
    {
      if (ary[i] == key)
	return i;
    }
  return -1;
}

template<class A>
void sswap(A &a, A &b)
{
  A t;
  t = a;
  a = b;
  b = t;
}

template<class A>
int bubbleSort(A ary[], int size)
{
  int sorted = 0;
  int moves=0;
  while (sorted == 0)
    {
      for (int i = 0; i<size-1; i++)
	{
	  if (ary[i] > ary[i+1])
	    {
	      sswap(ary[i], ary[i+1]);
	      moves++;
	      print_array(ary, size);
	    }
	}
      for (int i = 0; i< size-1; i++)
	{
	  if (ary[i] <= ary[i+1])
	    sorted = 1;
	  else 
	    {
	      sorted = 0;
	      break;
	    }
	}
      
    }
  return moves;
}

template<class A>
int selectionSort(A ary[], int size)
{
  int lowest_idx = 0;
  int nsorted = 0;
  int sorted = 0;
  while (nsorted < size)
    {
      // find the lowest (index)...
      for (int i = nsorted+1; i < size; i++)
	{
	  if (ary[i] < ary[lowest_idx])
	    lowest_idx = i;
	}
      swap(ary[lowest_idx], ary[nsorted]);
      lowest_idx = ++nsorted;
      print_array(ary, size);
    }
}

int main()
{

  int ary[20] = {2,5,3,7,2,56,6,9,1,2,5,3,78,9,5,7,8,1,23,3};
  cout << "Sorting: ";
  print_array(ary, 20);

  //  cout << endl<< bubbleSort(ary, 10) << " moves." << endl << endl;
  cout << endl << selectionSort(ary, 20) << " moves." << endl << endl;


  return 0;
}

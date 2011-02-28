#include <iostream>

using namespace std;

template<class A>
void print_array(const A ary[], int size)
{
  for (int i = 0; i < size; i++)
    cout << ary[i] << " ";

  cout << endl;

}

int main()
{

  int ary[] = {1,2,3,4,5,6,7,8,9,10}
  int size = 10;

  print_array(ary, size);
}

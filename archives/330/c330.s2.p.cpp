#include <iostream>
#include <iomanip>
#include <cmath> // for pow()

using namespace std;

void binary(bool[], int, int);
void print_ary(const bool[], int);

int main()
{

  int inpt;
  bool ary[8];

  cin >> inpt;

  cout << inpt << endl;

  if (inpt < 0)
    {
      inpt *= -1; // flip it.
      inpt = inpt^0xFF; // XOR with 11111111 which inverts the bits.
      inpt++;
    }

  cout << inpt << endl;

  binary(ary, 8, inpt);

  cout << endl;
  print_ary(ary, 8);

  return 0;

}

void binary(bool ary[], int size, int number)
{
  if (number > pow(2,size))
    {
      cout << "Number will not fit in an array this size!\n";
      return;
    }

  for (int i = 0; i < size; i++)
    {
      if (pow(2, size-1-i) <= number)
	{
	  ary[i] = 1;
	  number -= pow(2,size-1-i);
	}
      else
	{
	  ary[i] = 0;
	}
      cout << number << ":" << ary[i] << endl;
    }
}

void print_ary(const bool ary[], int size)
{
  for (int i = 0; i < size; i++)
    cout << ary[i];
  cout << endl;

}

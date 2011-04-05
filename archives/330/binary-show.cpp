/* Shaun Meyer, Sept 2010
 * CSCI-330 S2.P.1
 * The first page takes an integer in the range: -2,147,483,648 to 
 * 2,147,486,647 and returns the 32-bit binary 2's complement number 
 * that corresponds to it.  There should be a space between adjacent 
 * bytes so the output would have the form: 11110000 10101010 01010101 00001111.
 */


#include <iostream>
#include <climits>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[])
{

  int inpt;
  unsigned int mask = 1;
  
  mask = mask << sizeof(int)*8-1;

  cin >> inpt;

  cout << "Content-Type: text/plain\n\n";

  for (int i = 0; i< sizeof(int)*8; i++)
    {
      if (inpt & mask)
	  cout << 1;
      else
	  cout << 0;

      mask = mask >> 1;
      if ( (i+1)%8 == 0 )
	cout << " ";
    }
  cout << endl;

}

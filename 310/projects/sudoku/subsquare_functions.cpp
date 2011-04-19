#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

template<class V>
void print_array(vector<V> vec)
{
   for (int i = 0; i < vec.size(); i++)
      cout << vec[i] << " ";
   cout << endl;
}

vector<int> neighbors(int subsquare)
{
   vector<int> ret;
   for (int row = 0; row < 3; row++)
      for (int col = 0; col < 3; col++)
	 ret.push_back(subsquare+col + 9*row);

   return ret;
}

int sub_square(int cell)
{
   int row = cell / 9;
   int col = cell % 9;

   for (int subsquare_row = 0;
	subsquare_row < 9; 
	subsquare_row += 3)
      {
      for (int subsquare_col = 0;
	   subsquare_col < 9;
	   subsquare_col += 3)
	 {
	 if (row >= subsquare_row && row < subsquare_row + 3)
	    if (col >= subsquare_col && col < subsquare_col + 3)
	       return (subsquare_row * 9) + subsquare_col;
	 }
      }
   return -1;
}
int main()
{
   int counter = 0;
   for (int i=0; i < 81; i++)
      {
      cout << setw(3) << sub_square(i);
      counter++;
      if (counter == 9)
	 {
	 counter = 0;
	 cout << endl;
	 
	 }
      }
      /*
      cout << i << ": " << (i / 9) << "r" << (i % 9) 
	   << " = " << ( i % 9 ) - ( i / 9 ) << endl;
      */

   for (int i = 0; i < 81; i++)
      {
      print_array(neighbors(sub_square(i)));
      }

}

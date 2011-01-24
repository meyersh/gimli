#include <iostream>
#include "triary.hpp"

using namespace std;

int main()
{
  triary<int> triangle(4, false, 1);
  for (int i = 0; i < triangle.rows(); i++)
    {
      for (int j = 0; j < i+1; j++)
	cout << triangle.access(i,j) << " ";
      cout << endl;
    }
  return 0;
}

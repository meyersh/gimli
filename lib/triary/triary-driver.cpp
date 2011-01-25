/*******************************************************************************
 * triary.hpp driver. Purpose: Test triary.hpp functioning.
 * CREATED BY: Shaun Meyer, Jan-2011
 ******************************************************************************/
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

  // Cool, now lets access something out-of-bounds...
  triangle.access(5,5) = 5;

  return 0;
}

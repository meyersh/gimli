#include <iostream>
#include <stdexcept>
#include "adjmatrix.hpp"

using namespace std;

int main()
{
  adjMatrix a(10);
  // test out of range...
  
  try
    {
      a.edge(11,11) = 1;
    }
  catch (std::out_of_range &e)
    {
      cout << "out_of_range: " << e.what() << endl;
    }

  try
    {
      a.edge(1, -1) = 1;
    }
  catch (std::out_of_range &e)
    {
      cout << "out_of_range: " << e.what() << endl;
    }

  cout << "Goodbye!" << endl;

}

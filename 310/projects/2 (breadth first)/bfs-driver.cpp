#include "matrix_utils.hpp"

int main()
{
   // lets try some stuff!
   /*   adjMatrix foo(5);
   for (int i = 0; i < foo.size(); i++)
      {
      foo.edge(i, i) = 1;
      }
   save_matrix("matrix.txt", foo, "SHAUNSMATRIX");
   */

   print_matrix_as_dot("matrix.txt");
}

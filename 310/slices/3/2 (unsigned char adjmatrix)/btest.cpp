#include <iostream>
#include "adjMatrix.hpp"

#define BA_SIZE 19

using namespace std;

int main()
{
   /*
     bitarray ba(BA_SIZE);
     
     for (int i = 0; i < BA_SIZE; i++)
     {
     if (i & 1)
     ba[i] = 1;
     else
     ba[i] = 0;
     }
     
     for (int i = 0; i < BA_SIZE; i++)
     {
     bool foo = ba[i];
     cout << i << "(bool): " << foo << " ... "
     << i << "(cout): " << ba[i] << endl;
     }
     
   */


   unweightedUndirected graph(5);

   cout << "Populating graph a bit...\n";
   for (int i = 0; i < 5; i++)
      {
      graph.setBit(i, i);
      graph.print();
      cout << endl;
      }

   graph.print();
   graph.setAll();
   graph.print();
   graph.clearAll();
   graph.print();


   return 0;
}

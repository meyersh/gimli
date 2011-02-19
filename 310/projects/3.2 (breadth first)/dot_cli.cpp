#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "matrix_utils.hpp"
#include "shaun.hpp"

using namespace std;

int main(int argc, char **argv)
{
  vector<string> args(argv, argc+argv);
  string graphname;
  
  if (args.size() == 2)
    graphname = args[1];
  
   if (graphname == "")
     {
       cout << "No graph specified." << endl;
       return 0;
     }

   string PATH = GRAPH_PATH + graphname;

   try { 
     print_matrix_as_dot(PATH.c_str()); 
   } catch (const char e) { 
     cout << e << endl; 
   }
}



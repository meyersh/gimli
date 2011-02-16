#include <iostream>
#include "adjmatrix/adjmatrix.hpp"
#include "matrix_utils.hpp"

using namespace std;

int main()
{

   cout << "Content-Type: text/plain\n\n";

   string filename, graph_name;
   cin >> filename; // no spaces are allowed so this is kosher.

   try {
   adjMatrix &graph = load_matrix(string(GRAPH_PATH + filename).c_str(), graph_name);
   
   
   cout << "graphname=" << graph_name << endl;
   cout << "graphsize=" << graph.size() << endl;
   
   for (int x = 0; x < graph.size(); x++)
      for (int y = 0; y < graph.size(); y++)
	 if (graph.edge(x,y))
	    cout << "checked=" << x << "," << y << endl;

   } // end of try

   catch (runtime_error e)
      {
      cout << "error=" << e.what() << endl;
      return 0;
      }
}

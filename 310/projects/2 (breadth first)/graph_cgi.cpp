/* 
 * Given a set of options, a starting and ending point, and a 
 * graph name we shall attempt to return the path we came
 * up with for the calling javascript.
 */

#include <iostream>
#include <stdexcept>
#include <vector>
#include "adjmatrix/adjmatrix.hpp"
#include "stackorqueue/stackorqueue.hpp"
#include "matrix_utils.hpp"
#include "shaun.hpp"

using namespace std;

vector<int> bfs(adjMatrix &matrix, int start_node, int end_node, bool breadth_first_mode=true);

int main()
{
   cout << "Content-Type: text/plain\n\n";

   /* EXPECTED INPUT:

      GRAPH=(graphname)
      START=(int)
      END=(int)
      TYPE=BREADTHFIRST // optional

   */

   string graph_name, line;
   vector<string> inpt;
   int start=-1, end=-1;
   bool breadth_first = true;
   
   while (std::getline(cin, line))
      {
      inpt = split(line,"=");
      if (inpt.size() != 2)
	 continue;
      if (inpt[0] == "GRAPH")
	 graph_name = inpt[1];
      else if (inpt[0] == "START")
	 start = atoi(inpt[1].c_str());
      else if (inpt[0] == "END")
	 end = atoi(inpt[1].c_str());
      else if (inpt[0] == "TYPE" && inpt[1] == "BREADTHFIRST")
	 breadth_first = true;
      else if (inpt[0] == "TYPE" && inpt[1] == "DEPTHFIRST")
	 breadth_first = false;
	  }
   
   string graph_path=string(GRAPH_PATH) + graph_name;
   string name;
   
   try 
      {
      adjMatrix &graph = load_matrix(graph_path.c_str(), name);

      if (start < 0 || start > graph.size() || end < 0 || end > graph.size())
	 throw runtime_error("start or end nodes out_of_range.");

      vector<int> path = bfs(graph, start, end, breadth_first); 
      if (path.size())
	 {
	 if (breadth_first)
	    cout << "notice=The shortest path found is: ";
	 else
	    cout << "notice=Detected the following path: ";
	 /* We found a path, lets print it out real friendly (and in the correct
	  * order [backwards, in this case */
	 for (int i = path.size()-1; i > -1; i--)
	    {
	    cout << path[i];
	    if (i == 0)
	       cout << endl;
	    else
	       cout << " -> ";
	    }
	 }
      else // path was empty
	 cout << "notice=No path was found.\n";
      }

   catch (runtime_error e)
      {
      cout << "error=" << e.what() << endl;
      return 0;
      }

   
}

vector<int> bfs(adjMatrix &matrix, int start_node, int end_node, bool breadth_first_mode)
/* Do a breadth-first search of a given matrix and return the path
 * in a vector of ints. We don't bother to reverse the path so just
 * call us with start_node and end_node reversed and life will go on. */
{
   stackorqueue<int> *q;

   if (breadth_first_mode)
      q = new queue<int>;
   else
      q = new stack<int>;

   q->push(start_node);
   int current_node = start_node;

   vector<int> from_list(matrix.size(), -1),  // where are we from?
      visited(matrix.size(), 0); // have we visited this node before?

   bool path_found = true;

   while (current_node != end_node)
      {
      current_node = q->pop();
      for (int edge = 0; edge < matrix.size(); edge++)
	 {
	 if (matrix.edge(current_node, edge) == 0)
	    continue;
	 if (visited[edge])
	    continue;
	 
	 cout << "node=" << current_node << "," << edge << endl;
	 from_list[edge] = current_node;
	 visited[edge] = true;
	 q->push(edge);
	 }

      if (!q->size() && current_node != end_node)
	 /* We've exhausted our search possibilities and still have 
	  * no full path to show. Abort. */
	 {
	 path_found = false;
	 break;
	 }
      }

   /* Now translate our from_list into a vector-path */
   int i = end_node;
   vector<int> ret;

   if (!path_found)
      return ret; // stop here if no path was discovered

   ret.push_back(i);

   while (i != start_node)
      {
      ret.push_back(from_list[i]);
      i = from_list[i];
      }

   return ret;

}

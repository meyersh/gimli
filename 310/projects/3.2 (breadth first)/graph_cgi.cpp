/******************************************************************************
 * graph_cgi.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * Given a set of options, a starting and ending point, and a 
 * graph name we shall attempt to return the path we produced
 * for our calling javascript.
 *****************************************************************************/

#include <iostream>
#include <stdexcept>
#include <vector>
#include "adjmatrix/adjmatrix.hpp"
#include "stackorqueue/stackorqueue.hpp"
#include "matrix_utils.hpp"
#include "shaun.hpp"

using namespace std;

/* 
 * Prototypes
 */


int main()
{
   cout << "Content-Type: text/plain\n\n";

   /* EXPECTED INPUT:

      GRAPH=(graphname)
      START=(int)
      END=(int)
      TYPE=[BREADTHFIRST|DEPTHFIRST] // optional

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


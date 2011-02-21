/******************************************************************************
 * matrix_utils.cpp
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * Calculate a path in a graph between two points using the breadth first 
 * algorithm. This has the happy side effect of giving us the shortest path
 * while it's at it.
 *****************************************************************************/

#include "matrix_utils.hpp"

using namespace std;

adjMatrix &load_matrix(const char *filename, std::string &name)
/* given a filename and "name" reference, we'll attempt
 * to load a graph from a file and, as a side-effect,
 * populate the name variable.
 */
{
   ifstream graph_file;
   graph_file.open(filename);

   if (!graph_file)
      throw runtime_error(std::string("Cannot open file! '") + 
			  std::string(filename) + "'");

   std::string line;
   
   /* We expect the FIRST line of the file to contain the 
    * size of the matrix, comma, the name of the matrix. */
   std::getline(graph_file, line);
   adjMatrix *loaded_matrix = new adjMatrix(atoi(split(line, ",")[0].c_str()));
   name = split(line, ",")[1];

   /* from here on out we expect comma-separated "points", 
    * e.g., 
    * 1,2
    * 2,3
    * ... for bits that should be SET. 
    */
   while (std::getline(graph_file, line))
      {
      vector<std::string> inpt = split(line, ",");
      if (inpt.size() != 2)
	 continue;
      int x = atoi( inpt[0].c_str() );
      int y = atoi( inpt[1].c_str() );

      loaded_matrix->edge(x,y) = 1;
      
      }

   graph_file.close();
   return *loaded_matrix;
}

int save_matrix(const char* filename, adjMatrix &matrix, std::string name)
/* DESCR: Save a matrix to a file 
 * PARAMS: filename, adjMatrix, stringname (filename == name, this is legacy)
 * RETURNS: 0 (could be void, we throw for errors.)
*/
{
   ofstream graph_file;
   graph_file.open(filename);

   if (!graph_file)
      throw runtime_error(std::string("Cannot open file for saving! '") + 
			  std::string(filename) + "'");
   
   graph_file << matrix.size() << "," << name << endl;

   for (int x = 0; x < matrix.size(); x++)
      for (int y = 0; y < matrix.size(); y++)
	 if (matrix.edge(x,y))
	    graph_file << x << "," << y << endl;

   graph_file << endl;
   graph_file.close();
}

vector<int> bfs_get_path(adjMatrix &matrix, int start_node, int end_node)
/* DESCR: Given a matrix and using the bfs algorithm, compute a path from start_node
 * to end_node.
 * RETURNS: A vector<int> of our path. (Backwards)
 */
{
   queue<int> q;

   /* priming read pattern */
   int current_node = -1;
   q.push(start_node);

   boost::scoped_array<bool>  visited(new bool[matrix.size()]);
   boost::scoped_array<int> from_list(new int [matrix.size()]);

   /* initialize visited & from_list */
   for (int i = 0; i < matrix.size(); i++)
      {
      visited[i] = false;
      from_list[i] = -1;
      }

   /* 
    * Calculate the breadth-first path 
    * NOTE: Doesn't look for non-path graphs yet!
    */
   while (current_node != end_node)
      {
      current_node = q.pop();

      for (int edge = 0; edge < matrix.size(); edge++)
	 {
	 if (!matrix.edge(current_node, edge))
	    continue;
	 if (visited[edge])
	    continue;

	 from_list[edge] = current_node;
	 visited[edge] = true;
	 q.push(edge);
	 }

      }

   /*
    * Now, put together a vector indicating our path!
    */
   vector<int> ret;
   current_node = end_node;
   while (current_node != start_node)
      {
      ret.push_back(current_node);
      current_node = from_list[current_node];
      }

   return ret;
}

vector<int> bfs(adjMatrix &matrix, int start_node, int end_node, 
		bool breadth_first_mode)
/* DESC: Do a breadth-first search of a given matrix
 * PARAMS: adjMatrix, start & end nodes (ints) and bool representing
 *         the search type.
 * RETURNS: the path in a vector of ints. We don't bother to reverse 
 * the path here. */
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

int print_matrix_as_dot(const char *filename, bool is_directed)
/* DESCR: Given a matrix, return its contents as a dot graph.
 * PARAMS: filename & directional status
 * RETURNS: 0 (Throws for errors, may as well be void.)
 */
{
   std::string name; // place holder for our name;
   adjMatrix &matrix = load_matrix(filename, name);

   cout << "graph " << name << " {" << endl;

   for (int x = 0; x < matrix.size(); x++)
      for (int y = 0; y < matrix.size(); y++)
	 if (matrix.edge(x,y))
	    cout << "  " <<  x << (is_directed ? " -> " : " -- ") << y << ";" 
		 << endl;

   cout << "}" << endl;
   return 0;
}

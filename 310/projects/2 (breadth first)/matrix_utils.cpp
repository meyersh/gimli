/******************************************************************************
 * BFS- BREADTH FIRST SEARCH functions
 * Written by: Shaun Meyer, Feb 2011
 * 
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
      throw runtime_error("Cannot open file! '" + filename + "'");

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
/* Save a matrix to a file */
{
   ofstream graph_file;
   graph_file.open(filename);

   if (!graph_file)
      throw runtime_error("Cannot open file for saving! '" + filename + "'");
   
   graph_file << matrix.size() << "," << name << endl;

   for (int x = 0; x < matrix.size(); x++)
      {
      for (int y = 0; y < matrix.size(); y++)
	 {
	 if (matrix.edge(x,y))
	    graph_file << x << "," << y << endl;
	 }
      }
   graph_file << endl;
   graph_file.close();
}

vector<int> bfs_get_path(adjMatrix &matrix, int start_node, int end_node)
/* Given a matrix and using the bfs algorithm, compute a path from start_node
 * to end_node and return the path in a vector. Note: This path will be 
 * backwards, so it is advisable to swap the start_node and end_node arguments 
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

int print_matrix_as_dot(const char *filename, bool is_directed)
{
   std::string name; // place holder for our name;
   adjMatrix &matrix = load_matrix(filename, name);
   cout << "graph " << name << " {" << endl;
   for (int x = 0; x < matrix.size(); x++)
      {
      for (int y = 0; y < matrix.size(); y++)
	 {
	 if (matrix.edge(x,y))
	    {
	    cout << "  " <<  x << (is_directed ? " -> " : " -- ") << y << ";" << endl;
	    }
	 }
      }
   cout << "}" << endl;
   return 0;
}


#include <iostream>
#include <fstream>
#include "adjmatrix/adjmatrix.hpp"
#include "matrix_utils.hpp"
#include "shaun.hpp"

using namespace std;

/* because of the size of the matrix, we'll never have more than
 * 50*50 + (1 header) lines. Stop reading if we get more than that.
 */
#define MAX_LINES 50*50

int main()
{
   cout << "Content-Type: text/plain\n\n";

   string line, // to hold our cin input line...
      name; // graph name
  
   /* We expect the first line to hold SIZE,NAME... */
   std::getline(cin, line);

   if (split(line,",").size() != 2)
      {
      cout << "error=Malformed POST\n";
      return 0;
      }

   adjMatrix *tmp_matrix = new adjMatrix(atoi(split(line, ",")[0].c_str()));
   name = split(line, ",")[1];

   while(std::getline(cin, line))
      {
      vector<std::string> inpt = split(line, ",");
      if (inpt.size() != 2)
         continue;
      int x = atoi( inpt[0].c_str() );
      int y = atoi( inpt[1].c_str() );

      tmp_matrix->edge(x,y) = 1;
      }

   /* We're all done making our adjMatrix. if everything is OK, we'll 
      write it out. */
   string filename;
   filename += GRAPH_PATH;
   filename += name;
   
   try
      {
      save_matrix(filename.c_str(), *tmp_matrix, name);
      delete tmp_matrix;
      }
   catch (runtime_error e)
      {
      cout << "error=" << e.what() << endl;
      }
}

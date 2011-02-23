/******************************************************************************
 * save_graph_cgi.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * This provides the necessary inteface to allow us to save graphs we've
 * created/edited from the website.
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include "adjmatrixABC/adjMatrix.hpp"
#include "matrix_utils.hpp"
#include "sysutils.hpp"
#include "shaun.hpp"

using namespace std;

/* because of the size of the matrix, we'll never have more than
 * 50*50 + (1 header) lines. Stop reading if we get more than that.
 */
#define MAX_LINES 50*50
#define MAX_GRAPHS_ON_DISK 20

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

   unweightedDirectedLST *tmp_matrix = new unweightedDirectedLST(atoi(split(line, ",")[0].c_str()));
   name = split(line, ",")[1];

   while(std::getline(cin, line))
      {
      vector<std::string> inpt = split(line, ",");
      if (inpt.size() != 2)
         continue;
      int x = atoi( inpt[0].c_str() );
      int y = atoi( inpt[1].c_str() );

      tmp_matrix->setBit(x,y);
      }

   /* We're all done making our unweightedDirectedLST. if everything is OK, we'll 
      write it out. */
   string filename;
   filename += GRAPH_PATH;
   filename += name;
   
   try
      {
      if (lsdir(GRAPH_PATH).size() > MAX_GRAPHS_ON_DISK + 2)
	 {
	 delete_oldest_file_in_directory(GRAPH_PATH);
	 cout << "notice=Deleted a graph to maintain max-of-" 
	      << MAX_GRAPHS_ON_DISK+2 << ".\n";
	 }
      save_matrix(filename.c_str(), *tmp_matrix, name);
      delete tmp_matrix;
      }
   catch (runtime_error e)
      {
      cout << "error=" << e.what() << endl;
      }
}

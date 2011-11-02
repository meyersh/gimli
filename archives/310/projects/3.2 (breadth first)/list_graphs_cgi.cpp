/******************************************************************************
 * list_graphs_cgi.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * Return a list of the graphs in our GRAPH_PATH directory.
 *****************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include "shaun.hpp"
#include "matrix_utils.hpp"
#include "sysutils.hpp"

using namespace std;

int main()
{

   cout << "Content-Type: text/plain\n\n";
   
   vector<string> files = lsdir(GRAPH_PATH);

   // ignore an empty directory. ("." and ".." don't count.)
   if (files.size() > 2)
      cout << "graphs=";
   else
      return 0;

   for (int i = 0; i < files.size(); i++)
      {
      if (files[i] != "." && files[i] != "..")
	 {
	 cout << files[i];
	 if (i < files.size()-1)
	    cout << ",";
	 else
	    cout << endl;
	 }
      }
   return 0;
}

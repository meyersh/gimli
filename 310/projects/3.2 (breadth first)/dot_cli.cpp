/******************************************************************************
 * dot_cli.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * This is a thin wrapper around the print_matrix_as_dot() function
 * and is called from the command line to return a matrixs' dot formatting
 * for processing by dot/neato to make the graph image.
 *
 * From the command-line, we expect the first (and only) argument to be a 
 * name of a valid graph-file in GRAPH_PATH.
 *****************************************************************************/

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



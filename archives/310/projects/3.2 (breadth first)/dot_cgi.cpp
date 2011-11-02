/******************************************************************************
 * dot_cgi.cpp - 
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * A tentative (and currently unused) example wrapper to return a 
 * dot representation of a graph when I was still expecting to do this
 * over http. This is now replaced by webdot.cgi & dot_cli.
 *****************************************************************************/

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "matrix_utils.hpp"
#include "shaun.hpp"

using namespace std;

int main()
{
   cout << "Content-Type: text/plain\n\n";

   char * querystrings = getenv("QUERY_STRING");

   if (querystrings == NULL)
      return 0;

   /* Split up the query string and check for GRAPH= variable. */
   vector<string> query_strings = split(querystrings, "&");
   if (query_strings.size() == 0)
      {
      cout << "No query strings specified.\n";
      return 0;
      }

   string graphname = "";
   for (int i = 0; i < query_strings.size(); i++)
      {
      vector<string> keyval = split(query_strings[i], "=");
      if (keyval.size() != 2)
	 continue;
       
      if (keyval[0] == "GRAPH")
	 graphname = keyval[1];
      }

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



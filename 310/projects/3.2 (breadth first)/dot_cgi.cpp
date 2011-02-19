#include <cstdlib>

#include "matrix_utils.hpp"
#include <iostream>
#include <vector>
#include <string>

#include "shaun.hpp"

using namespace std;

int main()
{
   cout << "Content-Type: text/plain\n\n";

   char * querystrings = getenv("QUERY_STRING");

   if (querystrings == NULL)
      return 0;

   /* Split up the query string and check for GRAPH= variable. */
   vector<string> urlbase = split(string(querystrings), "?");
   if (urlbase.size() != 2)
     return 0;

   vector<string> query_strings = split(urlbase[1], "&");

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
     return 0;

   string PATH = GRAPH_PATH + graphname;

   try 
     { 
       print_matrix_as_dot(PATH.c_str()); 
     }
   catch (const char e)
     { 
       cout << e << endl; 
     }
}



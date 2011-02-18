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

   vector<string> query_strings = split(string(querystrings));
   string PATH = GRAPH_PATH;

   if (query_strings.size() == 2)
      {
      PATH += "/" + query_strings[1];
      try 
	 { 
	 print_matrix_as_dot(PATH.c_str()); 
	 }
      catch (const char e)
	 { 
	 cout << e << endl; 
	 }
      }

}

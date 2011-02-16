#include <iostream>
#include <vector>
#include <string>
#include "shaun.hpp"
#include "matrix_utils.hpp"
#include "sysutils.hpp"

using namespace std;

int main()
{
   vector<string> files = lsdir(GRAPH_PATH);

   if (files.size() > 2)
      cout << "graphs=";

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

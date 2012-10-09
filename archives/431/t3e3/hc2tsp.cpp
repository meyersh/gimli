/*
CSCI 431 T3.E3
For this assignment you will create a standalone computer program that
transforms Hamiltonian circuit problems into Traveling Salesperson
Problems.  The specification for it is as follows:

1. The program is a standalone C++ program.

2. It will take one command­line parameter that is the name of a file
holding a description of an HC problem and a second command­line
parameter that gives the name of the file where the TSP problem will
be written.

3. The program reads the first file and writes the corresponding TSP
problem to the file given by the second parameter.  It should provide
something on the screen to indicate progress, even if it's just a
notice that data's been read in and data's being written out.  The
format for the input file that describes the HC problem is as follows:

  1. The first line contains a single number which is the number of
  vertices in the HC problem.  We assume the vertices are
  consecutively numbered, beginning with 0.

  2. Subsequent lines each contain a pair of numbers representing an
  edge.  The first digit of the first number begins the line.  Numbers
  are separated by a single colon (:).

  3. There is one blank line at the end of the file, indicating that
  all info is done.

Team: Shaun + Stephen

*/

#include <iostream>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <vector>
#include "triary.hpp" // perfect for undirected graphs.

using namespace std;

int main(int argc, char** argv) {
   
   unsigned int num_vertices = 0;
   unsigned int max_vertices = 1000; // Abort for too many vertices.
   unsigned int min_vertices = 3;
   string line; /* reusable string for file reading */
   ifstream hc_problem_file;
   ofstream tsp_problem_file;
   triary<int> adj_matrix;
   int linenum = 1; // n+1 for compensate for first line.

   
   /* 2 command line parameters. */
   if (argc != 3)
	  {
	  cout << "SYNTAX: " << argv[0] 
		   << " <HC-Problem-File> <TSP-Problem-File>\n";
	  
	  exit(1);
	  }


   /* Open our files */
   hc_problem_file.open(argv[1]);
   if (!hc_problem_file)
	  {
	  cout << "Unable to open '" << argv[1] 
		   << "' for Hamilton Circuit." << endl;
	  exit(1);
	  }

   tsp_problem_file.open(argv[2]);
   if (!tsp_problem_file.good()) 
	  {
	  cout << "Unable to open '" << argv[2]
		   << "' for Travelling sales person." << endl;

	  exit(1);
	  }


   /* Read in and validate hc_problem */
   getline(hc_problem_file, line);
   num_vertices = atoi(line.c_str());

   if (num_vertices < min_vertices) 
	  {
	  cout << "Indicated number of vertices (" << num_vertices 
		   << ") is below vertice minimum (" << min_vertices << ").\n";
	  exit(1);
	  }
   else if (num_vertices > max_vertices)
	  {
	  cout << "Indicated number of vertices (" << num_vertices
		   << ") is above vertice max (" << max_vertices << ").\n";
	  exit(1);
	  }

   adj_matrix.resize(num_vertices);

   /* zero it out... */
   for (int i = 0; i < num_vertices; i++)
	  for (int j = i+1; j < num_vertices; j++)
		 adj_matrix.access(i,j) = 0;

   /* adj_matrix is ready and we have vertices so start connecting edges 
	  until we run out. Just skip errors loudly. */
   while (getline(hc_problem_file, line))
	  {
	  linenum++;
	  char separator = ':';
	  int vertice_a = 0;
	  int vertice_b = -1;
	  int *number = &vertice_a; // point at the number currently being read.

	  for (int i = 0; i < line.size(); i++) 
		 {
		 if (line[i] == separator) 
			if (number != &vertice_b)
			   number = &vertice_b;
			else
			   {
			   cout << "Line " << linenum << ": Ignoring extra colon.\n";
			   continue;
			   }
		 

		 else if (!isdigit(line[i]))
			{
			cout << "Line " << linenum << ": Ignoring invalid character '" 
				 << line[i] << "'.\n";
			continue;
			}


		 else 
			{
			if (*number == -1) // we have found our first digit for this number.
			   *number = 0;
			
			(*number) *= 10;
			(*number) += (line[i] - '0');
			}

		 } // done with line, now validate results:
	  
	  if (number != &vertice_b /* no colon or missing numbers around colon */ 
		  || vertice_a == -1 
		  || vertice_b == -1) 
		 {
		 cout << "Line " << linenum 
			  << ": Skipped mal-formed line." << endl;
		 continue;
		 }

	  if (vertice_a >= num_vertices || vertice_b >= num_vertices)
		 {
		 cout << "Line " << linenum << 
			": Skipping out-of-range vertex index. (>= " 
			  << num_vertices << ")\n";
		 continue;
		 }

	  adj_matrix.access(vertice_a, vertice_b) = 1;
	  
	  }

   /* Convert+write problem form */

   for (int i = 0; i < num_vertices; i++)
	  for (int j = i+1; j < num_vertices; j++)
		 if (adj_matrix.access(i,j))
			tsp_problem_file << i << ":" << j << " (1) " << endl;
		 else
			tsp_problem_file << i << ":" << j << " (2) " << endl;

   /* Clean up */
   hc_problem_file.close();
   tsp_problem_file.close();

   cout << "All done.\n";

   cout << "Processed " << linenum << " lines for " 
		<< num_vertices << " vertices." << endl;


   return 0; 
   
}

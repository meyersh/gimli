#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "adjMatrix.hpp"
#include "shaun.hpp"

using namespace std;

/* 
 * prototypes
 */
void print_help(string, string);
unweightedGraph &read_graph_file(char *filename, bool is_symmetrical);

int main(int argc, char **argv)
{

   /* Process input. We expect TWO file-names at the command-line. */
   if (argc != 3)
      {
      print_help("USAGE", argv[0]);
      exit(1);
      }
   
   unweightedGraph &directed = read_graph_file(argv[1], 1);
   unweightedGraph &undirected = read_graph_file(argv[2], 0);

   cout << "Directed graph:\n";
   directed.print();
 
   cout << "\nUndirected graph:\n";
   undirected.print();

   cout << "All done!" << endl;
}

void print_help(string t="HELP", string what="")
/* print out help for t topic. `what` is used for extra info */
{
   if (t == "USAGE")
      cout << "SYNTAX: " << what 
	   << " <DirectedGraph file> <UndirectedGraph file>\n";
}

unweightedGraph &read_graph_file(char *filename, bool directed_graph)
/* Read a given file in and return a reference to a directed/undirected
 * graph object which is populated with that files' data. */
{

   cout << "Reading in " << filename << endl;
   /* Try and open files... */
   ifstream graph_file;

   graph_file.open(filename);

   if (!graph_file)
      throw "Unable to open file(s).\n";

   int expected_rows;
   string srows;
   getline(graph_file, srows);
   expected_rows = atoi(srows.c_str());

   unweightedGraph *ret_graph;
   if (directed_graph)
      ret_graph = new unweightedDirected(expected_rows);
   else
      ret_graph = new unweightedUndirected(expected_rows);

   ret_graph->clearAll();

   string sline; /* container for a given line before we split it up... */
   std::getline(graph_file, sline);
   while(!graph_file.fail() || !graph_file.eof())
      {
      vector<string> vline = split(sline, " ");
      if (vline.size() != 2)
	 {
	 continue;
	 }

      int row = atoi(vline[0].c_str()),
	 col = atoi(vline[1].c_str());

      cout << " (" << row << ", " << col << ") ";

      if (row < expected_rows || col < row)
	 {
	 ret_graph->setBit(row, col);
	 }
      else
	 cout << " out of range... skipping.. ";
      
      // Cool, now fetch another line.
      std::getline(graph_file, sline);
      }
   cout << endl;

   graph_file.close();

   return *ret_graph;
}

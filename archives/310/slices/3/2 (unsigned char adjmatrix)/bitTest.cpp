/******************************************************************************
 * bitTest.cpp - CSCI 310 S3.E2
 * SUBMITTED BY: Shaun Meyer
 * CREATED     : 11 Feb 2011
 * CREATED BY  : Shaun Meyer
 * 
 * bitTest allows one to load up one directed and one undirected graph into
 * an adjacency matrix and work with either using a simple context-based CLI.
 * 
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>

#include "adjMatrix.hpp"
#include "shaun.hpp"

using namespace std;

/* 
 * prototypes
 */
void print_help(string, string);
unweightedGraph &read_graph_file(char *filename, bool is_symmetrical);
void slprint(string, unsigned int delay=25000);

/*
 * Showtime.
 */
int main(int argc, char **argv)
{

   /* Process input. We expect TWO file-names at the command-line. */
   if (argc != 3)
      {
      print_help("USAGE", argv[0]);
      exit(1);
      }
   
   /* read in the graphs */
   unweightedGraph &directed = read_graph_file(argv[1], 1);
   unweightedGraph &undirected = read_graph_file(argv[2], 0);
   
   
   unweightedGraph *cur_graph = NULL;

   slprint("\n\nWelcome to the graphic grapher!\n");
   slprint("Type HELP for instructions.\n");

   /* Prep for command line loop */
   string prompt(" OK > "),
      inpt;

   while (1)
      {
      if (!cin.good())
	 break;

      cout << prompt;
      std::getline(cin, inpt);
      vector<string> cmd = split(inpt, " ");

      /* Process commands based on number of inputs: */
      if (cmd.size() == 1)
	 {
	 cmd[0] = uppercase(cmd[0]);
	 if (cmd[0] == "QUIT")
	    break;
	 else if (cmd[0] == "HELP")
	    {
	    slprint("bitTest uses a context base system, commands affect the\n");
	    slprint("current context, which is indicated by the prompt.\n");
	    slprint("  To enter a context, type either `DIRECTED` or `UNDIRECTED`\n");
	    slprint("(don't worry, you can switch back and forth whenever you please.)\n");
	    slprint("  From within a graph-context you may manipulate and display the\n");
	    slprint("graph data.\n");
	    slprint("  Typing a command alone without any parameters, or incorrect parameters\n");
	    slprint("will cause the program to print a description for the given command.\n");
	    slprint("   COMMANDS:\n PRINT, DIRECTED, UNDIRECTED, SET, CLEAR, ");
	    slprint("QUIT.\n");
	    }
	 else if (cmd[0] == "DIRECTED")
	    {
	    cur_graph = &directed;
	    prompt = " DIRECTED > ";
	    }
	 else if (cmd[0] == "UNDIRECTED")
	    {
	    cur_graph = &undirected;
	    prompt = " UNDIRECTED > ";
	    }
	 else if (cmd[0] == "PRINT")
	    {
	    if (cur_graph)
	       cur_graph->print();
	    else
	       {
	       slprint("(You need to select a DIRECTED or UNDIRECTED graph first.)\n");
	       continue;
	       }
	    }
	 else if (cmd[0] == "SET" || cmd[0] == "CLEAR")
	    {
	    if (cur_graph)
	       {
	       slprint("Doesn't look like the point is in the proper syntax.\n");
	       slprint("Something like `" + cmd[0] + " 2,3` or `" + 
		       cmd[0] +" ALL` would be great.\n");
	       }
	    else 
	       {
	       slprint("(You need to select a DIRECTED or UNDIRECTED graph first.)\n");
	       continue;
	       }
	       
	    }
	 }
      else if (cmd.size() == 2)
	 {
	 cmd[0] = uppercase(cmd[0]);
	 cmd[1] = uppercase(cmd[1]);
	 if (cmd[0] == "SET")
	    {
	    vector<string> pt = split(cmd[1]);

	    if (!cur_graph)
	       {
	       slprint("(You need to select a DIRECTED or UNDIRECTED graph first.)\n");
	       continue;
	       }
	    if (cmd.size() > 1 && uppercase(cmd[1]) == "ALL")
	       {
	       cur_graph->setAll();
	       cout << "Graph set...\n";
	       continue;
	       }
	    else if (pt.size() != 2)
	       {
	       slprint("Doesn't look like the point is in the proper syntax.\n");
	       slprint("Something like `SET 2,3` `SET ALL` would be great.\n");
	       continue;
	       }

	    int row = atoi(pt[0].c_str());
	    int col = atoi(pt[1].c_str());
	       
	    cout << "Setting " << row << "," << col << endl;
	    cur_graph->setBit(row,col);
	    }
	 else if (cmd[0] == "CLEAR")
	    {
	    vector<string> pt = split(cmd[1]);

	    if (!cur_graph)
	       {
	       slprint("A DIRECTED or UNDIRECTED graph must first be selected.\n");
	       continue;
	       }
	    if (cmd.size() > 1 && uppercase(cmd[1]) == "ALL")
	       {
	       cur_graph->clearAll();
	       cout << "Graph cleared...\n";
	       continue;
	       }
	    else if (pt.size() != 2)
	       {
	       slprint("Doesn't look like the point is in the proper syntax.\n");
	       slprint("Something like `CLEAR 2,3` or `CLEAR ALL` would be great.\n");
	       continue;
	       }

	    int row = atoi(pt[0].c_str());
	    int col = atoi(pt[1].c_str());
	       
	    cout << "Clearing " << row << "," << col << endl;
	    cur_graph->clearBit(row,col);
	    }
	 }
      else if (cmd.size() == 3)
	 {
	 /* I have no 3-long commands... */
	 }

      cout << "Beep!\n";
              
      }


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

void slprint(string line, unsigned int delay)
/* Slow-print. Delay each character by `delay`-usecs. */
{
   for (int i = 0; i < line.size(); i++)
      {
      cout << line[i] << flush;
      usleep(delay);
      }
}

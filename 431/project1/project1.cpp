/*
 * Determinate Turing Machine Simulator
 * Shaun Meyer, Jan 2012
 *
 * Overview:
 * Tape struct manages the tape, including all read, write, and move
 * operations.
 *
 * A transition struct is used to describe the transitions.
 *
 * The finite state table is kept in memory as a two dimensional array
 * of `transition` objects. The row represents the 'trigger' character,
 * as in what we've just read from the tape.
 *   The column represents the specific Q (state) that we are in. I.e., 
 * if we are in state Q0 and have read a '3' from the tape, we'll want
 * the ['3'][0] transition from the table which will describe what to 
 * do next.
 *
 * The main() function calls all code to read + validate the file.
 * 
 * The runprogram() function actually runs a given program against the
 * described machine and returns true for YES and false for NO.
 *
 * TODO:
 *   - 'polynomial' parser for p(n) line.
 *   - grok what the output should actually look like
 *   - ?
 * 
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib> // for exit();
#include <cctype> // for isdigit();
#include "set.hpp"
#include "dtm.hpp"
#include "clauses.hpp"
#include "polynomial.hpp"

using namespace std;

std::vector<std::string> split(const std::string line, 
                               const std::string split_pattern=" ")
/* DESCR: Split a string `line` on `split_pattern` 
 * RETURNS: a vector<string> containing the result. */
{
   std::vector<std::string> ret;
   int start,
      end,
      len = 0;

   for (start = 0; start < line.size(); start += len + 1)
      {
      len = 0;
      end = line.find(split_pattern, start);
      if (end == std::string::npos)
         {
         ret.push_back(line.substr(start, line.size()-start));
         break;
         }
      
      /* If we made it this far, we've got a match. */
      len = end - start;

      if (len < split_pattern.size())
         continue; 
      
      ret.push_back(line.substr(start, len));
            
      }
   return ret;
}

/* 
 *function prototypes
 */

bool run_program(
				 transition **state_table, 
				 const char_set &tape_symbols, 
				 Tape tape);




Stats stats; // A global struct to hold pn, n, r, and v. Defined in dtm.hpp.
/*
 * The big show!
 */

int main(int argc, char** argv)
{

   /* All file objects we may need */
   ifstream config_file(argv[1]);
   ifstream tape_file;
   ofstream tracefile(argv[2]);


   // Save the console rdbuf pointer for later. 
   streambuf *console = cout.rdbuf();

   char_set tape_symbols;
   char_set input_characters;

   string line; /* Throw away line read variable. */
   
   int states = 0; // Number of states advertised in file.
   int states_read = 0;

   stats.pn = 0; // p(n)
   stats.n = 0;
   string polynomial; 

   stats.r = 0; // number of states - 1
   stats.v = 0; // number of input characters - 1

   Tape tape; // tape object to store our data.

   /* initialize state table (2d array). */
   transition **state_table = new transition*[255]; /* 255 is abitrary, 
													   largest a CHAR can be. */
   for (int i = 0; i < 255; i++)
	  state_table[i] = new transition[states];

   stats.transition_table = state_table;

   /* Validate arguments */
   if (argc != 3)
	  {
	  cout << "Syntax: " << argv[0] << " <in file> <out file>\n";
	  exit(1);
	  }
   
   /* Validate file readable */
   if (!config_file)
	  {
	  cout << "Error opening config file '" << argv[1] << "'.\n";
	  exit(1);
	  }

   /* Validate output file */
   if (!tracefile.good())
	  {
	  cout << "No dice on that output file '" << argv[1] <<"'. Bailing out.\n";
	  exit(1);
	  }
   

   /* Get and validate tape_symbols, input_characters, and the int. */
   if (!getline(config_file, line)) 
	  {
	  cout << "Error readig tape_symbols.\n";
	  exit(1);
	  }

   for (int i = 0; i < line.size(); i++)
	  tape_symbols.insert(line[i]);
   
   tape_symbols.insert(' ');

   if (!getline(config_file, line)) 
	  {
	  cout << "Error reading input_characters.\n";
	  exit(1);
	  }

   for (int i = 0; i < line.size(); i++)
	  input_characters.insert(line[i]);
   
   if (!getline(config_file, line)) // number of states
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   states = atoi(line.c_str()); // +1 for the implied blank.
   tape_symbols.insert(' '); // add the space symbol.

   /* Validate number of states */
   if (states < 1)
	  {
	  cout << "Non-positive number of states.\n";
	  exit(1);
	  }

   stats.r = states - 1;

   /*
	* Read in the states lines
	*/

   cout << "Reading " << states << " states." << endl;

   for (states_read = 0; states_read < states; states_read++)
	  /* Read in a line for each state we're expecting */
	  {

	  if (states_read == 1) // skip QY QN fields.
		 states_read = 3;


	  if (!getline(config_file, line))
		 {
		 cout << "Error reading state!\n";
		 exit(1);
		 }
	  cout << "Readling line " << line << endl;

	  /* 
	   * Read each transition state from a line
	   */

	  int number = 0; // general field for storing numbers
	  char c = NULL;  // the current character being scanning
	  transition t;   // the present transition we're filling in

	  enum { LOOKING_FOR_Q, 
			 LOOKING_FOR_COLON, 
			 LOOKING_FOR_CHAR,
			 LOOKING_FOR_DELTA };

	  int s = LOOKING_FOR_Q; // our search state
	  int state_idx = 0; // the current state-transition we're reading.

	  char symbol = input_characters.string()[state_idx];

	  cout << "Reading for state " << states_read << endl;

	  for (int i=0 ; i < line.length() ; i++)
		 /* Parse the line for transition-markers */
		 {
		 c = line[i]; // the character being scanned

		 switch (s) 
			{
			case LOOKING_FOR_Q:

			   if (toupper(c) == 'Y' || toupper(c) == 'N') 
				  {
				  t.next_state = (toupper(c) == 'Y') ? QY : QN;
				  s = LOOKING_FOR_COLON;
				  }

			   else if (isdigit(c))
				  {
				  number *= 10;
				  number += c - '0';
				  }
			
			   else if (c == ':') 
				  {
				  s = LOOKING_FOR_CHAR;
				  t.next_state = number;
				  number = 0;
				  }
		   
			   break;

			case LOOKING_FOR_COLON:

			   if (c == ':')
				  s = LOOKING_FOR_CHAR;

			   break;
			
			case LOOKING_FOR_CHAR:

			   t.write = c;
			   s = LOOKING_FOR_DELTA;

			   break;
			

			case LOOKING_FOR_DELTA:
			
			   if (c == '+' || c == '-')
				  {
				  t.delta = (c == '+');

				  /* update our state_table */
				  state_table[symbol][states_read] = t;

				  cout << " " 
					   << state_table[symbol][states_read].next_state << ":" 
					   << state_table[symbol][states_read].write << ":" 
					   << state_table[symbol][states_read].delta
					   << ",";

				  s = LOOKING_FOR_Q;

				  state_idx++;

				  symbol = input_characters.string()[state_idx];
				  }

			   break;

			}
					 

		 }

	  cout << endl;

	  /* Validate the line just read in */
	  if (state_idx  != input_characters.length())
		 {
		 cout << "Line " << setw(2) << setfill('0')
			  << states_read + 4
			  << ": Invalid number of transitions states." << endl
			  << "         (read " << state_idx << ", expected " 
			  << input_characters.length() << ")" << endl;

		 exit(1);
		 }

	  }

   /* Validate states read matches expected number 
	  +1 because states_read is zero-indexed and -2 
	  because of implicit QY and QN. */
   if (states_read+1 < states-2)
	  {
	  cout << "Error: Read " << states_read 
		   << " states, expected " << states-1 << endl;
	  exit(1);
	  }


   /*
	* Prepare + handle tape input sources (stdin or stdin filename)
	*/

   getline(config_file,line);

   /* validate line */
   for (int i = 0; i < line.length(); i++)
	  {
	  if (!input_characters.contains(line[i]))
		 {
		 cout << "Error: invalid character '" << line[i] 
			  <<"' found on tape.\n";
		 exit(1);
		 }
	  }

   tape.set(line+" "); // Actually set the tape contents!
   
   stats.tape = tape;
   stats.v = tape.length() - 1;

   getline(config_file, polynomial); // read in the last line, the polynomial.
   

   stats.n = tape.tape_length;
   stats.pn = eval_polynomial(polynomial, n);
   
   if (stats.pn < stats.n)
	  {
	  cout << "\nError: p(n) is less than your tape length.\n";
	  exit(1);
	  }

   cout << endl 
		<< "Config summary:\n"
		<< tape_symbols.length() << " {" << tape_symbols.string() << "}" << endl
		<< states << " states.\n"
		<< "tape value: {" << tape.toString() << "}" << endl 
		<< "Polynomial P = " << polynomial << endl
		<< "P(" << stats.n << ") = " << stats.pn << endl 
		<< "v = " << stats.v << ", r = " << stats.r << endl
		<< endl;
	  
   cout << "Writing clauses to file..." << endl;

   // redirect to the tracefile
   cout.rdbuf(tracefile.rdbuf());

   pretty_print_vector(G1a(stats), "G1_a");
   pretty_print_vector(G1b(stats), "G1_b");

   pretty_print_vector(G2a(stats), "G2_a");
   pretty_print_vector(G2b(stats), "G2_b");

   pretty_print_vector(G3a(stats), "G3_a");
   pretty_print_vector(G3b(stats), "G3_b");

   pretty_print_vector(G4a(stats), "G4_a");
   pretty_print_vector(G4b(stats), "G4_b");
   pretty_print_vector(G4c(stats), "G4_c");

   pretty_print_vector(G5a(stats), "G5_a");

   pretty_print_vector(G6a(stats), "G6_a");

   pretty_print_vector(G6b(stats), "G6_b");


   cout.rdbuf(console); // restore cout.

	// end clauses

   cout << " All done. " << endl;

   tracefile.close();
		 
   return 0;
}

bool run_program(
				 transition **state_table, 
				 const char_set &tape_symbols, 
				 Tape tape)
{

   int current_state_index = 0;
   int step = 0;

   while (!(current_state_index == QY) &&
		  !(current_state_index == QN)) 
	  {
	  /* Fetch the present transition-state */
	  transition state = state_table[tape.read()][current_state_index];


	  /* Render some nice output for humans */
	  char human_next_state; /* a human-readable description of the 
								upcoming state. */

	  if (state.next_state == QY)
		 human_next_state = 'Y';
	  else if (state.next_state == QN)
		 human_next_state = 'N';
	  else 
		 human_next_state = state.next_state + '0';

	  cout << setfill('0') << setw(3) << step << ": STATE: Q" 
		   << current_state_index
		   << " HEADPOS: " << setw(2) << tape.read_head_pos 
		   << " TAPE: (r'" << tape.read() << "') (w'" << state.write << "')"
		   << " NEXT: Q" << human_next_state << " DELTA: " 
		   << (state.delta ? '+' : '-') << endl;

	  /* write tape according to our state instruction */
	  tape.write(state.write);

	  /* move the tape as instructed */
	  tape.move(state.delta);

	  /* update current_state_index */
	  current_state_index = state.next_state;

	  step++;
	  }

   return (current_state_index == QY);

}

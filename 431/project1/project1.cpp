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
 *   - ?
 * 
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib> // for exit();

#include "set.hpp"
#include "dtm.hpp"

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


/*
 * The big show!
 */

int main(int argc, char** argv)
{

   /* All file objects we may need */
   ifstream config_file(argv[1]);
   ifstream tape_file;
   ofstream tracefile;


   // Save the console rdbuf pointer for later. 
   streambuf *console = cout.rdbuf();


   char_set tape_symbols;
   char_set input_characters;

   //   string tape_symbols; // The list of all symbols we'll work with 
   //   string input_characters; // the symbols which may be initially on the tape
   string line; /* Throw away line read variable. */
   
   int states = 0; // Number of states advertised in file.
   int states_read = 0;

   Tape tape; // tape object to store our data.


   /* initialize state table (2d array). */
   transition **state_table = new transition*[255]; /* 255 is abitrary, 
													   largest a CHAR can be. */
   for (int i = 0; i < 255; i++)
	  state_table[i] = new transition[states];



   /* Validate arguments */
   if (argc != 2)
	  {
	  cout << "Syntax: " << argv[0] << " <config file>\n";
	  exit(1);
	  }
     

   /* Validate file readable */
   if (!config_file)
	  {
	  cout << "Error opening config file '" << argv[1] << "'.\n";
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

   states = atoi(line.c_str()) + 1; // +1 for the implied blank.
   tape_symbols.insert(' '); // add the space symbol.

   /* Validate number of states */
   if (states < 1)
	  {
	  cout << "Non-positive number of states.\n";
	  exit(1);
	  }

   /*
	* Read in the states lines
	*/
   printf("Input:\n");
   while (getline(config_file, line))  
	  {

	  vector<string> temp_states = split(line, "|");

	  // Validate number of states on this line 
	  if (temp_states.size() > states) 
		 {
		 cout << line << endl 
			  << "Line has too many states.\n";
		 exit(1);
		 }

	  /* 
	   * Read each state from a line
	   */
	  for (int i = 0; i < temp_states.size(); i++)
		 {
		 char symbol = tape_symbols.string()[i];
		 cout << "Reading transition for '" << symbol 
			  << "' (q" << states_read << ") ==> " << temp_states[i] << endl;

		 vector<string> state = split(temp_states[i], ":");

		 /* Validate state "write-next" field, it 
			should be a member of tape_symbols. */
		 if (!tape_symbols.contains(state[1][0]))
			{
			cout << "Invalid state: '" << temp_states[i] << "' has symbol '" 
				 << state[1][0] << "' which is not in\n"
				 << "the alphabet {" << tape_symbols.string() << "}.\n";
			exit(1);
			}

		 /* delta is true if +, false otherwise. */
		 state_table[symbol][states_read].delta = (state[2] == "+");

		 /* write value */
		 state_table[symbol][states_read].write = state[1][0];

		 /* Handle exceptions of 'Y' and 'N' */
		 if (state[0] == "Y" || state[0] == "N")
			state_table[symbol][states_read].next_state = 
			   state[0] == "Y" ? QY : QN;
		 else
			state_table[tape_symbols.string()[i]][states_read].next_state = 
			   atoi(state[0].c_str());
		 }

	  states_read++;

	  }

   config_file.close();

   /* Validate states read matches expected number */
   if (states_read < states-1)
	  {
	  cout << "Error: Read " << states_read 
		   << " states, expected " << states-1 << endl;
	  exit(1);
	  }


   /*
	* Prepare + handle tape input sources (stdin or stdin filename)
	*/

   line = "";
   cout << "\nInitial tape input? (Press enter for file)> ";
   getline(cin,line);

   if (line == "")
	  {
	  cout << "Filename: (default: " << argv[1] << ".tape)> ";
	  getline(cin,line);
	  
	  /* Offer default path option */
	  string tape_file_path;
	  if (line == "")
		 tape_file_path = string(argv[1]) + ".tape";
	  else
		 tape_file_path = line;

	  tape_file.open(tape_file_path.c_str());

	  /* Validate tape_file path */
	  if (!tape_file)
		 {
		 cout << "Unable to open tapefile '" << tape_file_path << "'\n";
		 exit(1);
		 }

	  getline(tape_file, line);
	  tape_file.close();
	  }
   
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

   /* 
	* Prepare + Handle tape output source (file/console)
	*/
   line = "";
   cout << "Tracefile? (press enter for console) Filename> ";
   getline(cin, line);

   /* 
	* This is pretty cool, all we do if a file is specified is
	* redirect cout to the file buffer. So no other code is changed.
	*/
   if (line != "") {
	  tracefile.open(line.c_str());

	  if (tracefile.good())
		 cout.rdbuf(tracefile.rdbuf());

	  }
   
   cout << endl 
		<< "Config summary:\n"
		<< tape_symbols.length() << " {" << tape_symbols.string() << "}" << endl
		<< states << " states.\n"
		<< "tape value: {" << tape.toString() << "}" << endl << endl;
	  
   cout << "Program Run:" << endl;

   /* Run the program and record the result */
   bool result = run_program(state_table, tape_symbols, tape);

   /* return cout to console */
   cout.rdbuf(console);

   /*
	* Return the results to the screen and any output file. 
	*/
   string report("\nSOLUTION: ");
   report += (result == true) ? "YES" : "NO";

   cout << report << endl;
   if (tracefile.good())
	  {
	  tracefile << "\nSOLUTION: YES" << endl;
	  tracefile.close();
	  }

		 
   return 0;
}

bool run_program(
				 transition **state_table, 
				 const char_set &tape_symbols, 
				 Tape tape)
{

   int current_state_index = 0;
   int step = 0;

   while (current_state_index >= 0) 
	  {
	  /* Fetch the present transition-state */
	  transition state = state_table[tape.read()][current_state_index];


	  /* Render some nice output for humans */
	  char human_next_state; // a human-readable description of the upcoming state.
	  if (state.next_state == QY)
		 human_next_state = 'Y';
	  else if (state.next_state == QN)
		 human_next_state = 'N';
	  else 
		 human_next_state = state.next_state + '0';

	  cout << setfill('0') << setw(3) << step << ": STATE: Q" << current_state_index
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

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

using namespace std;

#define QY -1
#define QN -2

#define FORWARD true
#define BACKWARD false



/***
 * Tape structure, handles all tape operations by using a linked-list 
 * for tape squares. This allows any arbitrary moves prev & next.
 ***/

struct Tape {
   struct Cell {
	  Cell* next;
	  Cell* prev;
	  char data;
	  int index;

	  Cell(char d)
	  {
		 data = d;
		 next = prev = NULL;
	  }

   };

   Cell *beg;
   Cell *end;
   Cell *read_head;
   int read_head_pos;
   int tape_length;

   Tape() {
	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
   }

   Tape(string tape)
   {
	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
	  for (int i = 0; i < tape.length(); i++)
		 append(tape[i]);

	  read_head = beg;
   }


   string toString()
   {
	  string ret;
	  for (Cell *i = beg; i; i = i->next)
		 ret += i->data;
	  return ret;
   }

   void append(char d=' ')
   {
	  if (end)
		 {
		 end->next = new Cell(d);
		 end->next->prev = end;
		 end = end->next;
		 end->index = end->prev->index + 1;
		 }
	  else
		 {
		 beg = end = new Cell(d);
		 end->index = 1;
		 }

	  tape_length++; 
	  
   }

   void clear() {
	  Cell *t;
	  for (t = beg; t; t = t->next)
		 delete t;

	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
   }

   void set(string t)
   {
	  clear();
	  for (int i = 0; i < t.length(); i++)
		 append(t[i]);

	  rewind();
   }

   void prepend(char d=' ')
   {
	  if (beg)
		 {
		 beg->prev = new Cell(d);
		 beg->prev->next = beg;
		 beg = beg->prev;
		 beg->index = beg->next->index - 1;
		 }
	  else
		 {
		 beg = end = new Cell(d);
		 beg->index = beg->next->index - 1;
		 }
	  
	  tape_length++;
   }
   
   char read()
   {
	  if (read_head)
		 return read_head->data;
	  else
		 return ' ';
   }

   void write(char d=' ')
   {
	  if (read_head)
		 read_head->data = d;
   }

   int move(bool delta)
   {
	  if (delta == FORWARD)
		 {
		 if (read_head == NULL || !read_head->next)
			append();

		 read_head = read_head->next;
		 read_head_pos++;

		 }
	  else if (delta == BACKWARD)
		 {

		 if (read_head == NULL || !read_head->prev)
			prepend();

		 read_head = read_head->prev;
		 read_head_pos--;
		 }

	  return read_head_pos;
   }
   
   int rewind()
   {
	  read_head = beg;
	  return read_head_pos = 0;
   }
};

/***
 * transition structure - This holds the three pieces of a state for
 * easy access later. 
 **/

struct transition {
   int next_state; // -1 = QY, -2 = QN
   char write;
   bool delta; // TRUE == +1, FALSE == -1
   transition() {
	  write = ' ';
	  delta = true;
	  next_state = 0;
   }

   transition(int tonext, int towrite, bool todelta)
   {
	  next_state = tonext;
	  write = towrite;
	  delta = todelta;
   }
};

bool haschar(string s, char c)
/* Search (string)s for instances of (char)c, returning true on the
   first match. */
{
   for (int i = 0; i < s.length(); i++)
	  if (s[i] == c)
		 return true;

   return false;
}




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
				 string tape_symbols, 
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


   string tape_symbols; // The list of all symbols we'll work with 
   string input_characters; // the symbols which may be initially on the tape
   string line; /* Throw away line read variable. */
   
   int states = 0; // Number of states advertised in file.
   int states_read = 0;

   Tape tape; // tape object to store our data.


   /* initialize state table (2d array). */
   transition **state_table = new transition*[255]; /* 255 is abitrary, largest a CHAR can be. */
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
   if (!getline(config_file, tape_symbols)
	   || !getline(config_file, input_characters) 
	   || !getline(config_file, line)) // number of states
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   states = atoi(line.c_str()) + 1; // +1 for the implied blank.
   tape_symbols += ' '; // add the space symbol.

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
		 char symbol = tape_symbols[i];
		 cout << "Reading transition for '" << symbol 
			  << "' (q" << states_read << ") ==> " << temp_states[i] << endl;

		 vector<string> state = split(temp_states[i], ":");

		 /* Validate state "write-next" field, it 
			should be a member of tape_symbols. */
		 if (!haschar(tape_symbols, state[1][0]))
			{
			cout << "Invalid state: '" << temp_states[i] << "' has symbol '" 
				 << state[1][0] << "' which is not in\n"
				 << "the alphabet {" << tape_symbols << "}.\n";
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
			state_table[tape_symbols[i]][states_read].next_state = 
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
	  if (!haschar(input_characters, line[i]))
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
		<< tape_symbols.length() << " {" << tape_symbols << "}" << endl
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
				 string tape_symbols, 
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
		   << " NEXT: Q" << human_next_state << " DELTA: " << (state.delta ? '+' : '-') << endl;

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

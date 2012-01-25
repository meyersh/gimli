#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define QY -1
#define QN -2

#define FORWARD true
#define BACKWARD false

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

bool run_program(
				 transition **state_table, 
				 string tape_symbols, 
				 Tape tape);

std::vector<std::string> split(const std::string line, 
                               const std::string split_pattern=" ")
/* DESCR: Split a string `line` on `split_pattern` 
 * RETURNS: a vector containing the result. */
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


int main(int argc, char** argv)
{

   /* Validate arguments */
   if (argc != 2)
	  {
	  printf("Syntax: %s <config file>\n", argv[0]);
	  exit(1);
	  }
   
   fstream config_file(argv[1]);

   /* Validate file readable */
   if (!config_file)
	  {
	  printf("Error opening config file '%s'.\n", argv[1]);
	  exit(1);
	  }

   string tape_symbols;
   string input_characters;
   string line; /* Throw away line read variable. */
   int states = 0;

   /* Get and validate tape_symbols, input_characters, and the int. */
   if (!getline(config_file, tape_symbols)
	   || !getline(config_file, input_characters) 
	   || !getline(config_file, line)) // number of states
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   states = atoi(line.c_str()) + 1; // +1 because we have the implied blank.
   tape_symbols += ' '; // add the space symbol.
   Tape tape(input_characters); /* convert the input_characters string into our tape object. */

   /* Validate number of states */
   if (states < 1)
	  {
	  cout << "Non-positive number of states.\n";
	  exit(1);
	  }

   /* Attempt to read in the states. */
   int states_read = 0;

   /* state_table explanation:
	  (transition):state_table[read_value][current_state_index] = a transition
	  eg: read a ' ', goto state_table[' '], current state 'Q2': state_table[' '][2]
   */ 

   /* 
	* initialize state table. 
	*/
   transition **state_table = new transition*[255]; /* 255 is abitrary, largest character. */
   for (int i = 0; i < 255; i++)
	  state_table[i] = new transition[states];

   /*
	* Read in the states lines
	*/
   printf("Input:\n");
   while (getline(config_file, line))  
	  {

	  vector<string> temp_states = split(line);
	  if (temp_states.size() > states) 
		 {
		 cout << line << endl;
		 cout << "Line has too many states.\n";
		 exit(1);
		 }

	  /* 
	   * Read each state from a line
	   */
	  for (int i = 0; i < temp_states.size(); i++)
		 {
		 char symbol = tape_symbols[i];
		 cout << "Reading symbol '" << symbol 
			  << "' (q" << states_read << ") ==> " << temp_states[i] << endl;

		 vector<string> state = split(temp_states[i], ":");

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

   if (states_read < states-1)
	  {
	  printf("Error: Read %d states, expected %d.\n", states_read, states-1);
	  exit(1);
	  }

   printf("\nConfig summary:\n %d {%s} symbols\n %d states\n tape: {%s}\n\n", 
		  tape_symbols.length(), 
		  tape_symbols.c_str(), 
		  states,
		  tape.toString().c_str());

   printf("Program run:\n");

   if (run_program(state_table, tape_symbols, tape))
	  cout << "\nSOLUTION: YES\n";
   else
	  cout << "\nSOLUTION: NO\n";

   return 0;
}

bool run_program(
				 transition **state_table, 
				 string tape_symbols, 
				 Tape tape)
{

   int current_state_index = 0;
   int head_position = 0;
   char read_head = NULL;
   int step = 0;
   while (current_state_index >= 0) 
	  {
	  /* read the tape. */
	  read_head = tape.read();

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

	  printf("%3d: STATE: Q%d  HEADPOS: %02d TAPE: (r'%c') (w'%c') NEXT: Q%c DELTA: %c1\n", 
			 step, current_state_index, tape.read_head_pos, tape.read(), 
			 state.write, 
			 human_next_state,
			 state.delta ? '+' : '-');

	  /* write according to our state instruction */
	  tape.write(state.write);

	  /* move the tape as instructed */
	  tape.move(state.delta);

	  /* update current_state_index */
	  current_state_index = state.next_state;

	  step++;
	  }

   return current_state_index == QY ? true : false;

}

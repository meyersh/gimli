#include <iostream>
#include <fstream>
#include <vector>
#include <string>



using namespace std;

#define QY -1
#define QN -2

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


template<class V> 
void print_vector(const std::vector<V> &vec) 
/* iterate through a vector, printing the contents with std::cout */
{
   for (int i = 0; i < vec.size(); i++)
      std::cout << i << ": " << vec[i] << std::endl;
}

int main(int argc, char** argv)
{

   if (argc != 2)
	  {
	  printf("Syntax: %s <config file>\n", argv[0]);
	  exit(1);
	  }
   
   fstream config_file(argv[1]);

   if (!config_file)
	  {
	  printf("Error opening config file '%s'.\n", argv[1]);
	  exit(1);
	  }

   string tape_symbols;
   string input_characters;
   string line;
   int states = 0;

   /* Get the tape symbols. */
   if (!getline(config_file, tape_symbols))
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   /* Get the input_characters */
   if (!getline(config_file, input_characters))
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   /* Read and convert line to number of states. */
   if (!getline(config_file, line))
	  {
	  cout << "Read error.\n";
	  exit(1);
	  }

   states = atoi(line.c_str()) + 1; // +1 because we have the implied blank.
   tape_symbols += ' '; // add the space symbol.

   if (states < 1)
	  {
	  cout << "Non-positive number of states.\n";
	  exit(1);
	  }

   /* Attempt to read in the states. */
   int states_read = 0;

   /* state_table explanation:
	  (transition)state_table[read_value][current_state] = the transition
   */ 
   transition **state_table = new transition*[tape_symbols.length()];
   for (int i = 0; i < tape_symbols.length(); i++)
	  state_table[i] = new transition[states];

   while (getline(config_file, line))  
	  {
	  states_read++;
	  vector<string> temp_states = split(line);
	  if (temp_states.size() > states) 
		 {
		 cout << line << endl;
		 cout << "Line with too many states.\n";
		 exit(1);
		 }
	  for (int i = 0; i < temp_states.size(); i++)
		 {
		 cout << "Reading " << temp_states[i] << endl;
		 vector<string> state = split(temp_states[i], ":");
		 state_table[i][states_read].delta = true ? state[2] == "+" : false;
		 state_table[i][states_read].write = state[1][0];
		 if (state[0] == "Y" || state[0] == "N")
			state_table[i][states_read].next_state = QY ? state[0] == "Y" : QN;
		 else
			state_table[i][states_read].next_state = atoi(state[0].c_str());
		 }
	  }

   printf("\nConfig summary:\n %d {%s} symbols\n %d states\n.", 
		  tape_symbols.length(), 
		  tape_symbols.c_str(), 
		  states);



   return 0;
}

bool run_program(transition **state_table, string tape_symbols, string input_characters)
{
}

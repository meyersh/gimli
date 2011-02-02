
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "stkQ.cpp"

using namespace std;

void print_usage(string);
vector<string> split(const string line, const string split_pattern=",");
template<class V> void print_vector(const vector<V> &vec);


int main(int argc, char **argv)
{
  string inpt,
    status;

  vector<string> command_line;

  cout << "Welcome to the super stkQ test suite." << endl
       << "TEST 1: STRINGS" << endl
       << endl
       << "Initializing list-based stack of ints... ";

  stackL<int> int_stackL;
   
  cout << "Done..." << endl;

  while (1)
    {
      status = "OK";
      cout << "Commands: HELP, NEW, PUSH, POP, CLEAR, TOP, QUIT" << endl;
      
      cout << "CMD [" << status << "]> ";

      std::getline(cin, inpt);
      command_line = split(inpt, " ");
      
      if (command_line.size() < 1)
	{
	  cout << "Again please?\n\n";
	  continue;
	}
      if (command_line.size() == 1)
	{ /* Handle commands that are only one word */
	  if (command_line[0] == "HELP")
	    {
	      cout << "HELP:" << endl
		   << "Commands: HELP, NEW, DELETE, PUSH, POP, CLEAR, TOP, QUIT" << endl
		   << "HELP (COMMAND) for more specific instructions." << endl;
	    }
	  if (command_line[0] == "QUIT")
	    {
	      cout << "Goodbye!\n";
	      break;
	    }
	}
      if (command_line.size() == 2)
	{ /* Handle two-word commands (PUSH 5, etc) */
	  if (command_line[0] == "HELP")
	    {
	      cout << "HELP:" << endl
		   << "Commands: HELP, NEW, DELETE, PUSH, POP, CLEAR, TOP, QUIT" << endl
		   << "HELP (COMMAND) for more specific instructions." << endl;
	    }
	  if (command_line[0] == "PUSH")
	    {
	      cout << "I'd push " << command_line[1] << endl;
	      continue;
	    }
	}
      if (command_line.size() == 3)
	{ /* Handle three-word commands (NEW STACKV INT), etc */
	}
    }

  return 0;
}

vector<string> split(const string line, const string split_pattern)
{
  vector<string> ret;
  int start,
    end,
    len = 0;

  for (start = 0; start < line.size(); start += len + 1)
    {
      len = 0;
      end = line.find(split_pattern, start);
      if (end == string::npos)
	{
	  ret.push_back(line.substr(start, line.size()-start));
	  cout << "beep!\n";
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
void print_vector(const vector<V> &vec) 
{
  for (int i = 0; i < vec.size(); i++)
    cout << i << ": " << vec[i] << endl;
}

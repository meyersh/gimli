
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "stkQ.cpp"

using namespace std;

void print_usage(string);
vector<string> split(const string line, const string split_pattern=",");
template<class V> void print_vector(const vector<V> &vec);
string uppercase(string line);

int main(int argc, char **argv)
{
  string inpt,
    status;

  vector<string> command_line;
  boost::shared_ptr<stkQ_container> stkQ_test_object;

  cout << "Welcome to the super stkQ test suite." << endl
       << "TEST 1: STRINGS" << endl
       << endl
       << "Initializing list-based stack of ints... ";


  cout << "Done..." << endl;

  while (1)
    {
      status = "OK";
      // cout << "Commands: HELP, NEW, PUSH, POP, CLEAR, TOP, QUIT" << endl;
      
      cout << "CMD [" << status << "]> ";

      std::getline(cin, inpt);
      command_line = split(inpt, " ");
      
      if (command_line.size() < 1)
	{
	  cout << "Again please?\n\n";
	  continue;
	}
      else 
	command_line[0] = uppercase(command_line[0]); /* from here on out, all CMD[0] is
							 uppercase... */

      if (command_line.size() == 1)
	{ /* Handle commands that are only one word */
	  if (command_line[0] == "HELP")
	    {
	      cout << "HELP:" << endl
		   << "Commands: HELP, NEW, DELETE, PUSH, ENQUEUE, DEQUEUE, POP, CLEAR, TOP, "
		   << "FRONT, QUIT" << endl
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
	  command_line[1] = uppercase(command_line[1]);
	  if (command_line[0] == "HELP")
	    {
	      if (command_line[1] == "NEW")
		cout << "\nNEW <INT/STRING> <STACKV/STACKL/QUEUEV/QUEUEL>\n";
	      if (command_line[1] == "PUSH" || command_line[1] == "ENQUEUE")
		cout << "\nPUSH <VALUE>" << endl
		     << "\nENQUEUE <VALUE>" << endl;
	      if (command_line[1] == "POP" || command_line[1] == "DEQUEUE")
		cout << "\nPOP removes the top item from a stack." << endl
		     << "DEQUEUE removes the front item from a queue." << endl;
	      if (command_line[1] == "CLEAR")
		cout << "\nCLEAR empties the container of all values." << endl;
	      if (command_line[1] == "TOP" || command_line[1] == "FRONT")
		cout << "\nTOP displays the value of the top of stack." << endl
		     << "FRONT displays the value of the front of the queue." << endl;
	      if (command_line[1] == "QUIT")
		cout << "\nQUIT the program, freeing all memory." << endl;
	      if (command_line[1] == "HELP")
		cout << "HELP <command> for specific command help, HELP alone" << endl
		     << "to show available commands." << endl;
	    }
	  if (command_line[0] == "PUSH")
	    {
	      stkQ_test_object->push(command_line[1]);
	      cout << "PUSHING: '" << command_line[1] << "'" << endl;
	      continue;
	    }
	}
      if (command_line.size() == 3)
	{ /* Handle three-word commands (NEW INT stackv), etc */
	  command_line[1] = uppercase(command_line[1]);
	  command_line[2] = uppercase(command_line[2]);
	  if (command_line[0] == "NEW") 
	    {
	      if (command_line[1] == "INT" && command_line[2] == "STACKV")
		{
		  stkQ_test_object.reset(new stackV<int>);
		}
	      else if (command_line[1] == "INT" && command_line[2] == "STACKL")
		{
		  stkQ_test_object.reset(new stackL<int>);
		}
	      else if (command_line[1] == "INT" && command_line[2] == "QUEUEL")
		{
		  stkQ_test_object.reset(new queueL<int>);
		}
	      else if (command_line[1] == "INT" && command_line[2] == "QUEUEV")
		{
		  stkQ_test_object.reset(new queueV<int>);
		}
	      else if (command_line[1] == "STRING" && command_line[2] == "STACKV")
		{
		  stkQ_test_object.reset(new stackV<string>);
		}
	      else if (command_line[1] == "STRING" && command_line[2] == "STACKL")
		{
		  stkQ_test_object.reset(new stackL<string>);
		}
	      else if (command_line[1] == "STRING" && command_line[2] == "QUEUEL")
		{
		  stkQ_test_object.reset(new queueL<string>);
		}
	      else if (command_line[1] == "STRING" && command_line[2] == "QUEUEV")
		{
		  stkQ_test_object.reset(new queueV<string>);
		}
	      else 
		cout << "I don't understand what you're asking for.\n";
	    }
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

string uppercase(string line)
{
  string ret;
  for (int i = 0; i < line.size(); i++)
    ret += toupper(line[i]);
  
  return ret;
}

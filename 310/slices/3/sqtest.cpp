/*******************************************************************************
 * CSCI-310 Slice 3 Exercise 1
 * Created by: Shaun Meyer (Feb 2011)
 *
 * Stack/queue test program.
 * 
 * I got a bit carried away on this one, I admit. Runtype polymorphism, really?
 * but it seems like a valuable asset. If I knew more it would seem possible
 * to iterate through a set of types, instantiate the class for that type and
 * test it.
 *
 * Oh, well. Enjoy.
 ******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h> // for usleep()
#include "stkQ.cpp"

using namespace std;

/*
 * prototypes
 */

void print_usage(string);
vector<string> split(const string line, const string split_pattern=",");
template<class V> void print_vector(const vector<V> &vec);
string uppercase(string line);
void print_help(string cmd="HELP");
stkQ_ptr new_container(string itemtype, string containertype);
void sprint(string line, unsigned int delay=55000);

/*
 * globals
 */

bool is_int; /* used to remember the data type we're using */


int main(int argc, char **argv)
{
   string inpt,             // raw user input from stdin
      status = "OK" ;       // status for use in prompt
  
   vector<string> command_line; /* container for split commands from input,
				   "print hello" would become {'print', 'hello'} */

   /* A shared_ptr to the base class that we will operate on. This is the
    * secret sauce of run-time polymorphism (yay!) */

   boost::shared_ptr<stkQ_container> stkQ_test_object;

   sprint("Welcome to the super stkQ test suite.\n");
   sprint("  From this interface you may create a queue or stack and manipulate it.\n\n");
   print_help();

   /* 
    * Enter main input loop. 
    * if something goes wrong with stdin we bail out,
    * if the user wants out we bail out,
    * otherwise, repeat forever fulfilling requests.
    */
   while (1)
      {

      if (!cin.good())
	 break;

      cout << "\nCMD [" << status << "]> ";

      /* convert input to something easier to work with */
      std::getline(cin, inpt);
      command_line = split(inpt, " ");
      
      if (command_line.size() < 1) // no commands were entered.
	 {
	 cout << "Again please?\n";
	 continue;
	 }

      
      command_line[0] = uppercase(command_line[0]); 

      if (command_line.size() == 1)
	 { /* Handle commands that are only one word */

	 if (command_line[0] == "HELP")
	    print_help();
	  
	 if (command_line[0] == "QUIT")
	    break;

	 if (command_line[0] == "COUNT" && stkQ_test_object) 
	    cout << "SIZE: " << stkQ_test_object->count() << endl;

	 if (command_line[0] == "TOP" && stkQ_test_object)
	    stkQ_test_object->ptop();
	  
	 if ((command_line[0] == "POP" || command_line[0] == "DEQUEUE")
	     && stkQ_test_object)
	    {
	    cout << command_line[0] << " the ";
	    stkQ_test_object->ptop();
	    if (stkQ_test_object->pop())
	       status = "FAIL";
	    else
	       status = "OK";
	    }

	 if (command_line[0] == "DEQUEUE" && stkQ_test_object)
	    { 
	    cout << "DEQUEUE the ";
	    stkQ_test_object->ptop();
	    stkQ_test_object->pop();
	    }
	       

	 if (command_line[0] == "RESET")
	    status = "OK";
	 }
      else if (command_line.size() == 2)
	 { /* Handle two-word commands (PUSH 5, etc) */

	 command_line[1] = uppercase(command_line[1]);

	 if (command_line[0] == "HELP")
	    print_help(command_line[1]);

	 if ((command_line[0] == "PUSH" || command_line[0] == "ENQUEUE") 
	     && stkQ_test_object)
	    {
	    if (is_int)
	       stkQ_test_object->push(atoi(command_line[1].c_str()));
	    else
	       stkQ_test_object->push(command_line[1]);

	    cout << command_line[0] << ": '" << command_line[1] << "'" << endl;
	    continue;
	    }
	 }
      else if (command_line.size() == 3)
	 { /* Handle three-word commands (NEW INT stackv), etc */
	 command_line[1] = uppercase(command_line[1]);
	 command_line[2] = uppercase(command_line[2]);

	 if (command_line[0] == "NEW") 
	    stkQ_test_object = new_container(command_line[1], command_line[2]);
	    
	 }
      }

   cout << "Goodbye!\n";
   return 0;
}

vector<string> split(const string line, const string split_pattern)
/* Split a string on `split_pattern` and return a vector containing the result. */
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
/* iterate through a vector, printing the contents with std::cout */
{
   for (int i = 0; i < vec.size(); i++)
      std::cout << i << ": " << vec[i] << endl;
}

string uppercase(string line)
/* Returns a given string in UPPERCASE */
{
   string ret;
   for (int i = 0; i < line.size(); i++)
      ret += toupper(line[i]);
  
   return ret;
}

void print_help(string cmd)
/* print the help for a given command (defaults to "HELP" if none specified. */
{
   if (cmd == "HELP")
      {
      sprint("Commands:\nHELP\nNEW\nRESET\nPUSH\nENQUEUE\nDEQUEUE\nPOP\n");
      sprint("CLEAR\nTOP\nFRONT\nQUIT\n\n");
      sprint("HELP (COMMAND) for more specific instructions.\n");
      }
   if (cmd == "NEW")
      {
      sprint("\nNEW <INT/STRING> <STACKV/STACKL/QUEUEV/QUEUEL>\n");
      sprint("The NEW command will delete an old stack or queue if one exists\n");
      sprint("to allow for the new allocation.\n");
      }
   if (cmd == "RESET")
      sprint("\nRESET status alarms.\n");
   if (cmd == "PUSH" || cmd == "ENQUEUE")
      {
      sprint("\nPUSH <VALUE>");
      sprint("\nENQUEUE <VALUE>\n");
      }
   if (cmd == "POP" || cmd == "DEQUEUE")
      {
      sprint("\nPOP removes the top item from a stack.\n");
      sprint("DEQUEUE removes the front item from a queue.\n");
      }
   if (cmd == "CLEAR")
      sprint("\nCLEAR empties the container of all values.\n");
   if (cmd == "TOP" || cmd == "FRONT")
      {
      sprint("\nTOP displays the value of the top of stack.\n");
      sprint("FRONT displays the value of the front of the queue.\n");
      }
   if (cmd == "QUIT")
      sprint("\nQUIT the program, freeing all memory.\n");
}

stkQ_ptr new_container(string itemtype, string containertype)
{
   /* This over-rides the main() version of the same name! */
   stkQ_ptr stkQ_test_object; 

   if (itemtype == "INT")
      is_int = true;
   else if (itemtype == "STRING")
      is_int = false;

   if (itemtype == "INT" && containertype == "STACKV")
      stkQ_test_object.reset(new stackV<int>);

   else if (itemtype == "INT" && containertype == "STACKL")
      stkQ_test_object.reset(new stackL<int>);

   else if (itemtype == "INT" && containertype == "QUEUEL")
      stkQ_test_object.reset(new queueL<int>);

   else if (itemtype == "INT" && containertype == "QUEUEV")
      stkQ_test_object.reset(new queueV<int>);

   else if (itemtype == "STRING" && containertype == "STACKV")
      stkQ_test_object.reset(new stackV<string>);

   else if (itemtype == "STRING" && containertype == "STACKL")
      stkQ_test_object.reset(new stackL<string>);

   else if (itemtype == "STRING" && containertype == "QUEUEL")
      stkQ_test_object.reset(new queueL<string>);

   else if (itemtype == "STRING" && containertype == "QUEUEV")
      stkQ_test_object.reset(new queueV<string>);

   else 
      cout << "I don't understand what you're asking for.\n";

   if (stkQ_test_object)
      cout << "Test object created." << endl;

   return stkQ_test_object;

}

void sprint(string line, unsigned int delay)
/* Slow-print a string. Delay between each character a given number of 
 * microseconds. 25000 is a good starting point. */
{
   for (int i = 0; i < line.size(); i++)
      {
      usleep(delay);
      cout << line[i] << flush;
      }
}

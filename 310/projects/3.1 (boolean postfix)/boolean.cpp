// Ah, boolean postfix.
// Ah, webapps.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "stack.hpp"
#include "shaun.hpp"
#include <stdexcept>

using namespace std;

/*
 * prototypes 
 */
int precedence(char opcode);
void do_op();

stack<int> numbers; 
stack<char> ops;  /* 1 0, etc */
string postfix_out; // postfix output
pair<char, int> get_identifier(string);

int main()
{
  string line;
  std::getline(cin, line);
  if (line.size() == 0)
    return 0;

  /* I expect my input to be one line, comma-separated, beginning with the
   * equation and then followed by 1-26 fields (A=1 or B=0, etc)
   */

  vector<string> form_input = split(line, ",");
  print_vector(form_input);

  map<char, int> variables; // this will hold our CHAR->INT mapping.

  for (int i = 1; i < form_input.size(); i++)
    {
      try {variables.insert(get_identifier(form_input[i]));}
      catch (exception) {continue;}
    }

  cout << "Assigned identifiers:\n";
  print_map(variables);
   
  unsigned int open_parens = 0; // number of un-closed parens.


  /* lets parse / calculate our equation! */
   
  for (int i = 0; i < form_input[0].size(); i++)
    {
      char token = form_input[0][i];
      
      /* 
       * Skip spaces
       */
      if (token == ' ')
	continue;
      
      if (token == '(')
	{
	  open_parens++; // we're looking for closing...
	  ops.push(token);
	}
      else if (token == '&' 
	       || token == '|'
	       || token == '~')
	{
	  if (ops.size() == 0 ||
	      precedence(token) > precedence(ops.top()))
	    {
	      ops.push(token);
	    }
	  else
	    {
	      do_op();
	      ops.push(token);
	    }
	}
      else if (isalpha(token))
	{
	  numbers.push(variables[toupper(token)]);
	  postfix_out += toupper(token);
	}
      else if (token == ')')
	{
	  while (ops.size() != 0 && ops.top() != '(')
	    {
	      try {
		do_op(); ///
	      }
	      catch (exception) {
		cout << "TOP: '" << ops.top() << "'\n";
		cout << "ops.size(): " << ops.size() << endl;
		throw runtime_error("Bailing out!");
	      }
	    }

	  if (ops.size())
	    {
	      ops.pop(); /* get that '(' off 
				 of there. */
	      open_parens--;
	    }
	  else
	    throw runtime_error("Mis-matched parens.");
	}

      else if (isdigit(token))
	throw runtime_error("Numbers are not allowed in this equation.");
      else
	throw runtime_error("Unhandled problem (invalid equation!)");

       if (i == form_input[0].size()-1)
	/* We're done: do the remaining OPS. */
	{
	  if (open_parens)
	    throw runtime_error("Unclosed parens.");
	  else
	    while (ops.size())
	      do_op(); /* this gets tickled when doing '~' ??? */
	}

    }

  cout << "Numbers remaining: " << numbers.size() << endl;
  cout << "Ops remainin: " << ops.size() << endl;
  cout << "RESULT: " << numbers.top() << endl;
  cout << "POSTFIX: " << postfix_out << endl;

  return 0;
}

void do_op()
/* This function grabs the operand and requisite numbers,
 * pops them from the (global) stacks and pushes the result
 * back onto the numbers (global) stack. 
 *
 * For error handling, it throws runtime_error exceptions
 * with an appropriate description of the problem. 
 */
{
  int op1, op2;
  char opcode;
  if (ops.size())
    opcode = ops.pop();
  else
    throw runtime_error("Tried to pop operand from empty op stack.");
  
  postfix_out += opcode;

  if (numbers.size())
    op2 = numbers.pop();
  else 
    throw runtime_error("Tried to pop from empty number stack (op2).");

  /*
   * one exception, the NOT operator (~) is binary so 
   * we don't even bother looking for another number.
   */
  if (opcode != '~')
    {
      if (numbers.size())
	op1 = numbers.pop();
      else
	throw runtime_error("Tried to pop from empty number stack (op1).");
    }

  if (opcode == '&')
    numbers.push(op1 & op2);
  else if (opcode == '|')
    numbers.push(op1 | op2);
  else if (opcode == '~')
    numbers.push(!op2);

  cout << "Doing: " << op1 << " " << 
    opcode << " " << op2 << endl;

  
}

int precedence(char opcode)
/* This function returns the order-of-operations precedence
 * for a given opcode to be compared with other opcodes. */
{
  switch (opcode)
    {
    case '|':
      return 2;
    case '&':
      return 3;
    case '~':
      return 4;
    default:
      return 0;
    }
  

}

pair<char, int> get_identifier(string inpt)
/* given inpt in the form X=500, return a std::pair<char,int>
 * (X, 500) for insertion into a map<char,int> container. */
{
  char identifier;
  int value;

  if (inpt.size() < 2 || inpt[1] != '=' || !isalpha(inpt[0])) // invalid input...
      throw runtime_error("Bad input at '"  + inpt + "'. Ignoring.\n");
  
  identifier = toupper(inpt[0]);

  /* Whatever is left, after `N=`, we shove through atoi() and into our
   * value. Bam! */
  value = atoi(inpt.substr(2, inpt.size()-2).c_str());
  return pair<char,int>(identifier, value);
}

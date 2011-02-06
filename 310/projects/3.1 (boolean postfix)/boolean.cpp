// Ah, boolean postfix.
// Ah, webapps.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "stack.hpp"
#include "shaun.hpp"

using namespace std;

/*
 * prototypes 
 */
int precedence(char opcode);
void do_op();

stack<char> numbers; /* & | ~ */
stack<int> operands;  /* 1 0, etc */



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
      char identifier;
      int value;

      if (form_input[i].size() < 2 || form_input[i][1] != '=' || !isalpha(form_input[i][0])) // invalid input...
	{
	  cout << "Bad input at '" << form_input[i] << "'. Ignoring.\n";
	  continue;
	}
      
      identifier = form_input[i][0];
      value = atoi(form_input[i].substr(2, form_input[i].size()-2).c_str());      
      variables.insert(std::pair<char, int>(identifier, value));
    }

  cout << "Assigned identifiers:\n";
  print_map(variables);
   
  stack<char> operants; /* & | ~ */
  stack<int> operands;  /* 1 0, etc */

  /* lets parse / calculate our equation! */
   

  /* Stackify /Calcukate */

  for (int i = 0; i < form_input[0].size(); i++)
    {
      char token = form_input[0][i];
      
      if (token == ' ')
	continue;

      if (token == '(')
	{
	  operands.push(token);
	}
      else if (token == '&' 
	       || token == '|'
	       || token == '~')
	{
	  if (precedence(token) > (operands.size() && operands.top()))
	    {
	      operands.push(token);
	    }
	  else
	    {
	      do_op();
	    }
	}
      else if (isalpha(token))
	{
	  numbers.push(variables[toupper(token)]);
	}
      else
	{ 
	  numbers.push(token);
	}
    }

  cout << "Numbers remaining: " << numbers.size() << endl;
  cout << "Operands remainin: " << operands.size() << endl;
  cout << "RESULT: " << numbers.top() << endl;



  return 0;
}

void do_op()
{
  int op1, op2;
  char opcode = operands.pop();

  cout << numbers.size() << endl;
  op2 = numbers.pop();

  if (opcode != '~')
    op1 = numbers.pop();

  if (opcode == '&')
    numbers.push(op1 & op2);
  else if (opcode == '|')
    numbers.push(op1 | op2);
  else if (opcode == '~')
    numbers.push(!op2);

  cout << numbers.size() << endl;

  
}

int precedence(char opcode)
{
  switch (opcode)
    {
    case '~':
      return 1;
    case '|':
      return 2;
    case '&':
      return 3;
    }
  

}

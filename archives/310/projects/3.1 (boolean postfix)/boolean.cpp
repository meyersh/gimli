/*******************************************************************************
 * boolean.cpp for S3.P1 in CSCI-310
 * by Shaun Meyer (Feb, 2011)
 * 
 * Takes an infix boolean equation and processes it with postfix, returning
 * the postfix equation on the way out.
 * 
 * Uses exceptions for error catching and a simplified stack class.
 ******************************************************************************/

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
pair<char, int> get_identifier(string);
void print_error(string); 

/*
 * globals
 */
stack<int>    numbers; 
stack<char>   ops;              // ~ & |, etc 
string        postfix_out;      // postfix version of our equation
unsigned int  open_parens = 0;  // number of un-closed parens

int main()
{
   /* We're a web-app now! */
   cout << "Content-Type: text/plain\n\n";

   /* Fetch std input for equation + identifiers */
   string line;
   std::getline(cin, line);

   if (line.size() == 0)
      return 0;

   /* I expect my input to be one line, comma-separated, beginning with the
    * equation and then followed by 1-26 fields (A=1 or B=0, etc)  */

   vector<string> form_input = split(line, ",");
   map<char, int> variables; // this will hold our CHAR->INT mapping.

   /* process each identifier field and place results into a map.
    * we silently ignore too many fields or improperly formatted
    * fields. */
   for (int i = 1; i < form_input.size(); i++)
      {

      if (i > 26)
	 break; 

      try {variables.insert(get_identifier(form_input[i]));}
      catch (exception) {continue;}

      }

   //cout << "Assigned identifiers:\n";
   //print_map(variables);

   try 
      {
      for (int i = 0; i < form_input[0].size(); i++)
	 /* For each token in our equation:
	  * - Skip past spaces
	  * - Maintain two (global) stacks: numbers + ops.
	  * Basic algorithm:
	  * - If we encounter an (, remember it. When the ) 
	  *   is found, do the operands until we find the ( again.
	  *   should it not be there, we've got mis-matched parens.
	  * - If we encounter an opcode, push it unless it's less than 
	  *   the precedence of the last opcode. Should that happen, we
	  *   perform the opcodes until we have better precedence. 
	  * - If we encounter a character, it's an identifier. Look up
	  *   and store its value in the numbers stack.
	  * - At the end, process all un-processed opcodes.
	  */
	 {
	
	 char token = form_input[0][i];
	
	 /* 
	  * Skip spaces
	  */
	 if (token == ' ')
	    continue;
	
	 if (token == '(')
	    {
	    open_parens++; // Keep an eye out for closing parens...
	    ops.push(token);
	    }
	
	 else if (token == '&' 
		  || token == '|'
		  || token == '~')
	    {
	    while (ops.size() && precedence(token) < precedence(ops.top()))
	       do_op();
	   
	    ops.push(token);
	    }
	
	 else if (isalpha(token))
	    {
	    numbers.push(variables[toupper(token)]);
	    postfix_out += toupper(token);
	    }
	
	 else if (token == ')')
	    {
	    /* process all ops until we find the opening paren
	     * or die trying. Throw a runtime_error if we have
	     * ops without enough numbers. */
	    while (ops.size() && ops.top() != '(')
	       {
	       try {do_op();}
	       catch (exception) {
	       cout << "TOP: '" << ops.top() << "'\n";
	       cout << "ops.size(): " << ops.size() << endl;
	       throw runtime_error("Bailing out!");
	       }
	       }
	   
	    /* based on the above while() loop, there must be 
	     * a ( on top of the ops stack. If the stack is 
	     * empty, we have mis-matched parens. */
	    if (ops.size() && ops.pop() == '(') 
	       open_parens--;
	    else
	       throw runtime_error("Mis-matched parens.");
	    }
	
	 else if (isdigit(token))
	    throw runtime_error("Numbers are not allowed in this equation.");
	 else
	    throw runtime_error("Unhandled problem (invalid equation!)");
	
	 if (i == form_input[0].size()-1)
	    /* We're at the end of the equation; do the remaining OPS. */
	    {
	    if (open_parens)
	       throw runtime_error("Unclosed parens.");
	    else
	       while (ops.size())
		  do_op(); 
	    }
	 }
      if (numbers.size() != 1)
	 throw runtime_error("Too few operators, invalid infix equation.");
      } catch (runtime_error e) { print_error(e.what()); }


   cout << "remaining_numbers=" << numbers.size() << endl;
   cout << "remaining_ops=" << ops.size() << endl;
   cout << "result=" << numbers.top() << endl;
   cout << "postfix=" << postfix_out << endl;

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

#ifdef DEBUG
   cout << "Doing: " << op1 << " " << 
      opcode << " " << op2 << endl;
#endif
  
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

void print_error(string err)
{
   cout << "error=" << err << endl << flush;
}

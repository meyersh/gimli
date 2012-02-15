#ifndef __POLYNOMIAL_HPP__
#define __POLYNOMIAL_HPP__

/*
  Evaluate a polynomial from a std::string with alphabet {+, -, ^, *}.

  Allow substitutions for 'n'.

  base algorithm description:
  http://en.wikipedia.org/wiki/Shunting-yard_algorithm

  modified slightly to evaluate while reading where possible.

  Shaun Meyer, Feb 2012
*/

#include <string>
#include <stack>
#include <cctype> // for isdigit()
#include <iostream>
#import <cmath>

int do_op(const char op, int a, int b)
// apply a given operation to two operands
{
   switch (op)
	  {
	  case '*':
		 return a * b;
	  case '+':
		 return a + b;
	  case '-':
		 return a - b;
	  case '^':
		 return pow(a, b);
	  }
}

int op_prec(const char c)
// return the precedence of an operator
{
   switch(c)    {
   case '^':
	  return 4;
   case '*':  
	  return 3;
   case '+': case '-':
	  return 2;
   }
   return 0;
}

bool is_operator(const char c)
{
   return (c == '+' || c == '-' || c == '*' || c == '^');
}

long long int p(std::string polynomial, int n)
/* Parse and evaluate a polynomial contained in the string using Dijkstra's 
 * Shunting-yard algorithm. */
{

   int number = 0; // a dummy variable for reading in numbers.
   std::stack<int> numbers;
   std::stack<char> operators;
   
   bool scanning_number = true;
   
   for (int i = 0; i < polynomial.size(); i++)
	  {
	  char c = polynomial[i]; // the character we're currently scanning

	  if (c == 'n' || c == 'N') // substitute for 'n'
		 numbers.push(n);

	  else if (!(isdigit(c) || is_operator(c))) // non number, non operator
		 continue; 

	  if (isdigit(c)) 
		 {
		 number *= 10;
		 number += c - '0';
		 scanning_number = true;
		 }

	  else  // is operator
		 {
		 if (scanning_number) // we've been reading in a number (to prevent false positives)
			{
			numbers.push(number);
			number = 0;
			scanning_number = false;
			}

		 // precedence is lower than op stack current.
		 if (operators.size() == 0 || op_prec(c) > op_prec(operators.top()))
			operators.push(c);
			   
		 // do the operation if the incoming precendence is higher than top of stack op.
		 else if (op_prec(c) == op_prec(operators.top()) 
				  && numbers.size() >= 2)
			{
			int b = numbers.top(); numbers.pop();
			int a = numbers.top(); numbers.pop();
			numbers.push(do_op(operators.top(), a, b));
			operators.pop(); // remove the op we just did
			operators.push(c); // ad the op we've just scanned in.
			}

		 else if (op_prec(c) < op_prec(operators.top()))
			{ // when c < operators.top, do the operators until we can push c.
			while (operators.size() 
				   && numbers.size() > 1
				   && op_prec(c) < op_prec(operators.top()))
			   {
			   int b = numbers.top(); numbers.pop();
			   int a = numbers.top(); numbers.pop();
			   numbers.push(do_op(operators.top(), a, b));
			   operators.pop();
			   }
			operators.push(c);
			}
 
		 else
			std::cout << "Something went weird!\n"
					  << "scanning: '" << c << "'\n";
		 
		 }
	  }

   
   if (scanning_number) // the line ended while scanning a number. 
	  {
	  numbers.push(number);
	  number = 0;
	  scanning_number = false;
	  }


   // evalulate the remaining stack numbers and operands
   while (numbers.size() > 1 && operators.size())
	  {
	  int b = numbers.top(); numbers.pop();
	  int a = numbers.top(); numbers.pop();
	  numbers.push(do_op(operators.top(), a, b));
	  operators.pop(); 
	  }

   
   // if all went well, numbers.size() == 1 and is our result.
   return numbers.top();
}




#endif

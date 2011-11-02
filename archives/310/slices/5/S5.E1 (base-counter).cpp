/****************************************************************************
 * S5.E1 (Odometer)
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 09 MAR 2011
 *  DESCRIPTION:
 *
 *  A multi-base computer odometer simulatron. The program expects (at the 
 * minimum) a number of wheels to be possessed by the odometer. If a number 
 * is entered that is outside of the MAX_DIGITS range, or negative, the
 * program will complain to you and refuse to odomate until given a 
 * reasonable number.
 *
 * If given boorish input (like strings, filenames, etc) the program will
 * merely check the result from atoi() for sanity. If it passes, the 
 * program is happy.
 * 
 *  For a bonus, you may also specify the base (how many numerals on
 * a wheel) and the program will count using that (the difference from
 * this and a program that is dedicated to binary is very, very little).
 *
 *  If you enter no base, or a base that is greater than MAX_BASE,
 * a binary odometer will be assumed.
 ***************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // for atoi()
using namespace std;

/* Max number of digits we'll accept as valid. */
#define MAX_DIGITS 1024 
#define MAX_BASE 100

/* 
 * Prototypes
 */
int advance(unsigned int *odometer);
int carry_one(unsigned int *odometer, int digits, int base);
void print_odometer(unsigned int *odometer, int digits);

int main(int argc, char **argv)
{
   /* suck in and parse our cli arguments */
   vector<string> args(argv, argc+argv);
   if (args.size() == 1 || args.size() > 3)
      {
      cout << "Usage: " << args[0] << " <digits> [base (1-36)]" << endl;
      return 0;
      }

   int base = 0; // The numeric base of our odometer 

   if (args.size() == 3) /* they may have specified a base */
      base = atoi(args[2].c_str());

   if (base < 1 || base > MAX_BASE)
      base = 2; // default to 
      
   cout << "Using base: " << base << endl;

   int digits = 0; // the number of wheels on our odometer

   if (args.size() > 1)
      digits = atoi(args[1].c_str());

   if (digits < 1 || digits > MAX_DIGITS)
      {
      cout << "Invalid number of digits entered. Expected" << endl
	   << "an integer between 1 and " << MAX_DIGITS << " (inclusive)."
	   << endl;
      return 0;
      }

   cout << "Calculating " << digits << " digits.\n";

   /* Create + Initialize the odometer */
   unsigned int *odometer = new unsigned int[digits];
   for (int i = 0; i < digits; i++)
      odometer[i] = 0;
   
   /* 
    * increment until we break for a carry off the last digit. 
    */
   while (1)
      {
      if(advance(odometer) == base) // time to carry a one       
	 if (carry_one(odometer, digits, base) > digits-1) 
	    // if our carry is > digits, we're done.
	    break; 

      print_odometer(odometer, digits);
                                           
      }

}

int advance(unsigned int *odometer)
/* PARAMS: A big-endian odometer array (one `digit` per element) 
 * RETURN: The current value of the `ones` place in the odometer
 * DESCR:  Advances the `ones place` (does not carry) */
{
   return ++odometer[0];
}

int carry_one(unsigned int *odometer, int digits, int base)
/* PARAMS: A big-endian odometer array (one `digit` per element)
 * RETURN: The digit which we carried into (a value of "digits" length
 *         would indicate that our odometer has wrapped
 * DESCR:  Carry the bit from the first `digit`. */
{

   /* dig represents the digit we're working with and will end up
    * containing the value of the last digit we've carried into. */
   int dig;
   
   for (dig = 0; 
	dig < digits && odometer[dig] >= base; 
	odometer[dig] = 0, odometer[++dig]++)
      ;

   return dig;
      
}

void print_odometer(unsigned int *odometer, int digits)
/* PARAMS: A big-endian odometer array (one `digit` per element)
 * RETURN: void
 * DESCR : Print our array to cout in reverse order. */
{
   for (int i = digits-1; i >= 0; i--)
      cout << "[" << odometer[i] << "]";
   cout << endl;
}

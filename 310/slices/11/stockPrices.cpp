/* 
   stockPrices.cpp
   SUBMITTED BY: Shaun Meyer
   CREATED BY: Shaun Meyer
   CREATED Apr 4 2011
   
   DESCRIPTION:

   This is a standalone program. Read in data from a file. (The name
   of the file should be given as a command-line parameter.) Each line
   of the file has a set of pairs. The first part of each pair is a
   stock symbol (each symbol is composed of one or more uppercase
   letters and may contain one period (.) which must have at least one
   upper case letter to either side of it). The second part is a price,
   which is a real number. A colon (:) separates the two parts. Each
   stock symbol should appear no more than once. (There can be no more
   than one price per symbol).  

   The names should be stored in a ternary search tree. The index
   should hold the price.

   Once the data has been read in, the user should have a menu of the
   following options:
  
   1.  Look up a price 
   2.  Change a price 
   3.  Delete a stock 
   4.  Quit

   When the first option is selected, the program should prompt the
   user for a stock symbol. If the symbol is not found, the user should
   be informed of that. If found, then the price should be
   displayed. In either case, the user is then taken back to the menu.

   When the second option is selected, the program should prompt the
   user for a stock symbol and a price. If the symbol is not found, it
   should be added with the indicated price. If found, then the price
   should be changed. In either case, the symbol and price should be
   displayed and the user should then be taken back to the menu.  

   When the third option is selected the user should be prompted for a
   stock symbol. If it is in the tree, it should be deleted and a
   message to this effect given to the user. If it is not in the tree,
   a message to that effect should be given to the user. In either
   case, the user should then be taken back to the menu.

   If Quit is selected, the program should quit.

*/

#include <fstream>
#include <iostream>
#include <iomanip> // for setprecision
#include <string>
#include <vector>
#include <cstdlib>
#include "hash.hpp"

using namespace std;

/***********************
 * HELPER FUNCTIONS
 **********************/

std::vector<std::string> split(const std::string line, 
			       const std::string split_pattern=",")
/* PARAMS     : string thing, string separator
   RETURNS    : vector of strings
   DESCRIPTION: Splits a string into pieces on `split_pattern` and returns a
   vector of these results. */
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

string tolower(string inpt)
/* PARAMS     : string thing
   RETURNS    : string entirely in lowercase.
   DESCRIPTION: Returns a string as lower case. */
{
   string ret;
   for (int i = 0; i < inpt.size(); i++)
      ret += tolower(inpt[i]); // tolower(char) version.

   return ret;
}

bool is_valid_symbol(const string &symbol)
/* PARAMS     : string thing
   RETURNS    : true if string is a valid symbol, false otherwise.
   DESCRIPTION: Check that symbol is only letters, hyphens, or
   (optionally) one dot. */
{
   int dots = 0;
   string::const_iterator c;
   if (symbol[0] == '.' 
       || (symbol.size() > 1 && symbol[symbol.size()-1] == '.'))
      return false;

   for (c = symbol.begin(); c != symbol.end(); c++)
      {
      if (*c == '.')
	 dots++;
      else if (isalpha(*c) || *c == '-')
	 continue;
      else // non-alpha, non-hyphen, non-dot
	 return false;

      if (dots > 1)
	 return false;
      }
   return true;
}

void pretty_print_vector(vector<string> symbols, int width=80)
/* PARAMS     : vector<string> of symbols, int representing max character width
   RETURNS    : void
   DESCRIPTION: Print pretty the content of a vector<string> */
{
   int widest = 0, // widest string seen.
      w = 0; // width of current line.

   for (int i = 0; i < symbols.size(); i++)
      {
      if (symbols[i].size() > widest)
	 widest = symbols[i].size();
      }
   
   widest++; // pad us by one space.
   
   for (int i = 0; i < symbols.size(); i++, w += widest)
      {
      cout << setw(widest)
	   << symbols[i];
      if (w + widest > width)
	 {
	 cout << endl;
	 w = 1;
	 }
      }
   if (w)
      cout << endl;
}

/***********************
 * THE BIG SHOW
 **********************/

int main(int argc, char **argv)
{
   vector<string>args(argv, argv+argc);
   if (args.size() != 2)
      {
      cout << "USAGE: " << args[0] << " <stockfile.txt>\n";
      return 0;
      }

   ifstream stock_file(args[1].c_str());
   if (!stock_file)
      {
      cout << "Unable to read '" << args[1] << "'.\n";
      return 0;
      }

   hashTable<double> stock_prices(8192); // all stock_prices, in hash form.
   vector<string> invalid_symbols; // all invalid symbols detected.

   string line;
   int num_symbols = 0;
   while(std::getline(stock_file, line))
      {
      vector<string> pair = split(line, ":");
      if (pair.size() != 2)
	 continue; // malformed line
      else if (pair.size() == 2 && pair[1] == "N/A")
	 continue; // N/A stock from yahoo. 
      else if (!is_valid_symbol(pair[0]))
	 {
	 invalid_symbols.push_back(pair[0]);
	 continue;
	 }

      double price = atof(pair[1].c_str());
      stock_prices.insert(tolower(pair[0]), price);
      num_symbols++;
      }

   /*
    * Symbols have been loaded, it's menu time now.
    */

   if (invalid_symbols.size() != 0)
      {
      cout << "Invalid symbols:\n";
      pretty_print_vector(invalid_symbols, 80);
      }

   cout << fixed           // no scientific notation
	<< setprecision(2) // only two decimals, please.
	<< num_symbols << " symbols loaded.\n"
	<< "[L]ookup price, [C]hange a price, [D]elete a price, "
	<< "[P]rint symbols\n[S]how hashTable, or [Q]uit.\n"
	<< "> ";

   double price; // a place holder for our price.

   while (std::getline(cin, line))
      {
      vector<string> cmds = split(line, " ");
      if (tolower(line[0]) == 'q')
	 break;

      if (tolower(line[0]) == 'p')
	 pretty_print_vector(stock_prices.keys());

      if (tolower(line[0]) == 's')
	 cout << stock_prices.hash_print();
      /* This never happens, if it does: Print out errors */
      if (cmds.size() != 2 && cmds.size() != 3)
	 {
	 /* Something was typed wrong. */
	 if (cmds[0][0] == 'l')
	    cout << "To lookup: L <SYMBOL>\n";
	 else if (cmds[0][0] == 'd')
	    cout << "To delete: D <SYMBOL>\n";
	 else if (cmds[0][0] == 'c')
	    cout << "To set/change: C <SYMBOL> <PRICE>\n";
	 else
	    cout << "[L]ookup price, [C]hange a price, [D]elete a price, "
		 << "[P]rint symbols\n[S]how hashTable, or [Q]uit.\n";
	 }

      if (cmds.size() == 2)
	 {
	 cmds[0] = tolower(cmds[0]);
	 if (cmds[0][0] == 'l') // [L]ookup price 
	    {
	    if (!stock_prices.getIndex(tolower(cmds[1]), price))
	       cout << tolower(cmds[1]) << ": $" << price << endl;
	    else
	       cout << "No such symbol, '" << tolower(cmds[1]) << "'.\n";
	    }
	 else if (cmds[0][0] == 'd') // [D]elete a price
	    {
	    price = -1;
	    if (!stock_prices.getIndex(tolower(cmds[1]), price))
	       {
	       cout << "Deleting '" << tolower(cmds[1]) << "'.\n";
	       stock_prices.deleteKey(tolower(cmds[1]));
	       }
	    else
	       cout << "No such symbol, '" << tolower(cmds[1]) << "'.\n";
	    }
	 else if (cmds[0][0] == 'c') // wrong # of args to change a price.
	    cout << "C <symbol> <price>\n";
	 else // something else was wrong. print generic error.
	    cout << "[L]ookup price, [C]hange a price, [D]elete a price, or [Q]uit.\n";

	 }
      else if (cmds.size() == 3)
	 {
	 cmds[0] = tolower(cmds[0]);
	 if (cmds[0][0] == 'c') // [C]hange a price
	    {
	    double price = atof(cmds[2].c_str());
	    stock_prices.insert(tolower(cmds[1]), price);
	    cout << "Setting price of '" << tolower(cmds[1]) << "' to $" << price << ".\n";
	    }
	 else if (cmds[0][0] == 'l')
	    cout << "To lookup: L <SYMBOL>\n";
	 else if (cmds[0][0] == 'd')
	    cout << "To delete: D <SYMBOL>\n";
	 else
	    cout << "[L]ookup price, [C]hange a price, [D]elete a price, or [Q]uit.\n";
	    
	 }

      cout << "> ";	 
      }

   cout << "\nGoodbye!\n";

}


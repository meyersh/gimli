/*
  emailAddy.cpp -
  CREATED BY: Shaun Meyer
  SUBMITTED BY: Shaun Meyer
  CREATED: 4 Apr 2011
  
  DESCRIPTION:
  This is a standalone program. Read in data from a file. (The name of the file 
  should be given as a command-line parameter.) Each line of the file has a set 
  of pairs. The first part of each pair is a name, and may have blanks in it. 
  The second part is an email address. A colon (:) separates the two parts. It 
  is possible that a person may have more than one email address, in which case 
  there would be a separate line in file for each email address (all lines would 
  have the same string before the colon, but a different email address following
  it). 

  The names should be stored in all lowercase. The names should be stored in a 
  ternary search tree. The index should be an integer. This integer is an index 
  into a vector < vector < string > > where each entry in the outer vector holds 
  the email address(es) for one person.
  
  Once the data has been read in, the user should have a menu of the following
  options:

  1. Look up an address
  2. Quit

  When the first option is selected, the program should prompt the user for a 
  name. The name (converted to lowercase) should be searched for in the tree. 
  If not found, the user should be informed of that. If found, all the email 
  addresses associated with that name should be listed. In either case, the 
  user is then taken back to the menu. If Quit is selected, the program should 
  quit.

*/


#include <fstream>
#include <iostream>
#include <iomanip>
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

   for (int i = 0; i < symbols.size(); i++, w += widest)
      {
      cout << setw(widest)
	   << symbols[i];
      if (w + widest > width)
	 {
	 cout << endl;
	 w = 0;
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
   // read in args.
   vector<string> args(argv, argv+argc);
   if (args.size() != 2)
      {
      cout << "USAGE: " << args[0] << " <filename>\n";
      return 0;
      }

   /* open / test file */
   ifstream email_file(args[1].c_str());
   if (!email_file)
      {
      cout << "Error opening '" << args[1] << "'\n";
      return 0;
      }

   /* each key will point to a vector of strings.
      this will allow duplicate email_addresses. */
   hashTable<int> names;
   vector< vector<string > > email_addresses;

   /* statistical counters */
   int num_names = 0;
   int num_addresses = 0;


   /**************************************************
    * READ IN ALL EMAIL ADDRESSES FROM FILE
    *************************************************/
   string line;
   while( std::getline(email_file, line))
      {
      vector<string> pair = split(line, ":");
      if (pair.size() != 2)
	 continue; // malformed line
      
      /* if index is not in the names trie, it is a new entry
	 which means its index would be email_address.size(). */
      int index=email_addresses.size(); 
      
      names.getIndex(tolower(pair[0]), index);
      
      vector<string> current_emails;

      if (index == email_addresses.size())
	 {
	 num_names++;
	 num_addresses++;
	 names.insert(tolower(pair[0]), index);
	 current_emails.push_back(pair[1]);
	 email_addresses.push_back(current_emails);
	 }
      else // name is already in our index.
	 {
	 email_addresses[index].push_back(pair[1]);
	 num_addresses++;
	 }

      }

   /**************************************************
    * ALLOW USER TO VIEW EMAIL ADDRESS FROM MENU
    *************************************************/

   cout << "Loaded " << num_names << " names with " << num_addresses
	<< " addresses. Type a name to search for that exact key or \n" 
	<< "PRINT to show all names loaded.\n"
	<< "HASH to show the hash table.\n"
	<< "QUIT to leave (apologies if your friends name is quit.)\n\n";

   cout << "> "; // initiate prompt.
   while (std::getline(cin, line))
      {
      if (tolower(line) == "print")
	 pretty_print_vector(names.keys(), 0);
     
      else if (tolower(line) == "hash")
	 cout << names.hash_print();

      else if (tolower(line) != "quit")
	 {
	 int index = -1;
	 names.getIndex(tolower(line), index);
	 if (index > -1)
	    for (int i = 0; i < email_addresses[index].size(); i++)
	       cout << i << ": " << email_addresses[index][i] << endl;
	 else
	    cout << "'" << line << "' not found.\n";
	 }
      else
	 break;

      cout << "> ";
	 
      }
   
   cout << "\nGoodbye!\n";
}

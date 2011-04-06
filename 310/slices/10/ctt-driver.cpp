/*
 * ctt-driver (test-suite)
 * CREATED BY: Shaun Meyer
 * CREATED   : 2 APR 2011
 * DESCRIPTION: Tests insert(), getIndex(), and deleteKey() in ctt.hpp
 *              by loading the entire dictionary file and inserting all
 *              words as keys, verifying each. 
 */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "ctt.hpp"

using namespace std;

// LINUX + MAC
// #define WORDS_FILE "/usr/share/dict/words"
// SOLARIS:
//#define WORDS_FILE "/usr/share/lib/dict/words"
#define WORDS_FILE "words.txt"
/* global variables */


/* functions */
int check_keys(vector<string> &words_list, ctt<int> &words, 
	       bool print_found=false, bool print_missing=false, int offset=-1)
{
   /* Search for each word. */
   int found = 0;
   string word;

   for (int i = offset+1; i < words_list.size(); i++)
      {
      word = words_list[i];
      int index = 0;
      int ret = 0;
      ret = words.getIndex(word, index);
      if (ret == 0) 
	 {
	 if (print_found)
	    cout << "Found: " << word << " (" << index << ")\n";
	 found++;
	 }
      else
	 if (print_missing)
	    cout << "Missing: " << word << endl;
      }
   return found;
}

int main(int argc, char **argv)
{
   vector<string> args(argv, argv+argc);

   /* Process options. */
   bool print_found = false;
   bool print_missing = false;
   for (int i = 1; i < args.size(); i++)
      {
      if (args[i] == "-m")
	 print_missing = true;
      if (args[i] == "-f")
	 print_found = true;
      }
   
   ctt<int> words;
   vector<string> words_list;

   ifstream wordsfile(WORDS_FILE);
   if (!wordsfile)
      {
      cout << "Unable to open words file.\n";
      return 0;
      }
   
   string word;

   /* Load in words from file */
   cout << "Loading dictionary into words ctt.\n";
   int i = 0; 
   while(std::getline(wordsfile, word))
      {
      if (word == "")
	 continue; // gimli has a weird dict file.
      words.insert(word, i++);
      words_list.push_back(word);
      }
   wordsfile.close();

   /* test:
      getIndex & insert 
   */

   cout << "\nTesting getIndex() & insert():\n";

   int found = check_keys(words_list, words, print_found, print_missing);

   cout << "Found " << found << "/" << words_list.size() << " words." << endl;
   if (found == words_list.size())
      cout << "Congratulations, you have a flawless implementation of" 
	   << " getIndex & insert\n";
   else
      {
      cout << "There are keys that could not be found. This indicates\n"
	   << "a malfunctioning insert() OR getIndex().\n"
	   << "No further tests will be performed in this condition.\n";
      return 0; // no further tests are done if this has failed.
      }

#ifdef TEST_DELETE
   /* test: 
      void ctt<B>:: deleteKey(const string &key)
   */
   cout << "\nTesting deleteKey():\n";
   for (int i = 0; i < words_list.size(); i++)
      {
      word = words_list[i];
      int index = 0;
      cout << "Deleting '" << word << "'\n";
      words.deleteKey(word);
      
      /* Check that we're not damaging OTHER keys in this process by
	 checking that all other keys are still present. 
	 ( This is expensive, so only check every few thousand keys. */
      if (i % 3000 == 0 || true)
	 if (check_keys(words_list, words, print_found, print_missing) != (words_list.size()-i-1))
	    cout << "\nDelete is damaging other keys.\n" << flush;
	 else 
	    //cout << "." << flush;


      if (!words.getIndex(word, index))
	 {
	 cout << "Failed to deleted key '" << word << "' (" << index << ")\n";
	 }
      else
	 {
	 cout << "Successfully deleted key '" << word << "'\n";
	 }
      }
   cout << endl;
   if (check_keys(words_list, words) == 0)
      cout << "All keys deleted!\n"
	   << "You have an operational version of deleteKey()!\n";
   else
      cout << "Something went wrong, there are keys remaining. This\n"
	   << "indicates that there is something wrong with your deleteKey().\n";

#endif

   cout << "Testing keys()\n";
   vector<string> keys = words.keys();
   for (int i = 0; i < keys.size(); i++)
      cout << i << ": " << keys[i] << endl;


   return 0;
}

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

#include "ctt.hpp"

using namespace std;

#define WORDS_FILE "/usr/share/dict/words"
// #define WORDS_FILE "words.txt"

int main()
{
   ctt<int> words;
   vector<string> words_list;

   int i = 0;
   ifstream wordsfile(WORDS_FILE);
   if (!wordsfile)
      {
      cout << "Unable to open words file.\n";
      return 0;
      }
   
   string word;

   /* Load in words from file */
   while(std::getline(wordsfile, word))
      {
      words.insert(word, i++);
      words_list.push_back(word);
      }
   wordsfile.close();

   /* Search for each word. Report the found words (probably the shorter
      list... */
   int found = 0;
   for (int i = 0; i < words_list.size(); i++)
      {
      word = words_list[i];
      int index = 0;
      int ret = 0;
      ret = words.getIndex(word, index);
      if (ret == 0) 
	 {
	 cout << "Found: " << word << ":" << index << endl;
	 found++;
	 }
      }
   cout << "Found " << found << "/" << words_list.size() << endl;
   
   return 0;
}

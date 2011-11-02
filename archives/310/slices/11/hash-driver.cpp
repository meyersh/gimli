#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "hash.hpp"

using namespace std;

#define WORDS_FILE "/usr/share/lib/dict/words"

int main()
{
   hashTable<int> words;
   vector<string> wordlist;
   int index = 0;

   ifstream words_file(WORDS_FILE);
   if (!words_file)
      return 0;

   string word;
   while (std::getline(words_file, word))
      {
      wordlist.push_back(word);
      words.insert(word, index);
      index++;
      }

   for (int i = 0; i < wordlist.size(); i++)
      {
      if (words.getIndex(wordlist[i], index))
	 cout << "Could not retrieve key '" << wordlist[i] << "'" << endl;
      }

}

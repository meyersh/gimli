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
   int index = 0;

   ifstream words_file(WORDS_FILE);
   if (!words_file)
      return 0;

   string line;
   while (std::getline(words_file, line))
      {
         words.insert(line, index);
	 index++;
      }

}

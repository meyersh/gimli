#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdexcept>

using namespace std;

vector<string> tokenize(string inpt)
/* Tokenize a given string. We break on ', "
 * and finally, space and newlines. */
{
   int start=0;

   int find_start, find_end;

   vector<string> ret;

   while (start < inpt.size())
      {

      /*
       * Find single quote 
       */

      /* lets look for a single quote. */
      find_start = inpt.find('\'', start);
      
      /* if we found one, look for the second */
      if (find_start == start /* make sure we're not skipping stuff */
	  && find_start != string::npos)
	 {
	 find_end = inpt.find('\'', start+1);
	 if (find_end == string::npos)
	    throw std::runtime_error("Missing closing quote.");	  
	 else /* we found the closing quote */ 
	    {
	    find_start++; /* eat the quote */
	    ret.push_back(inpt.substr(find_start, find_end-find_start));
	    start = find_end +1;
	    continue;
	    }
	 }

      /* 
       * Find Double quote 
       */

      /* lets look for a double quote. */
      find_start = inpt.find('"', start);
      
      /* if we found one, look for the second */
      if (find_start == start /* make sure we're not skipping stuff */
	  && find_start != string::npos)
	 {
	 find_end = inpt.find('"', start+1);
	 if (find_end == string::npos)
	    throw std::runtime_error("Missing closing quote.");	  
	 else /* we found the closing quote */ 
	    {
	    find_start++; /* eat the quote */
	    ret.push_back(inpt.substr(find_start, find_end-find_start));
	    start = find_end +1;
	    continue;
	    }
	 }

      /* no quote found - look for a space */
      find_end = inpt.find(' ', start);
      
      /* Eat "empty" spaces */
      if (find_end == start)
	 {
	 start++;
	 continue;
	 }
      if (find_end != string::npos)
	 {
	 //find_end++; /* eat the space */
	 ret.push_back(inpt.substr(start, find_end-start));
	 start = find_end+1;
	 }
      else 
	 {
	 /* no spaces, either? Whatever. grab whatever's left. */
	 ret.push_back(inpt.substr(start, inpt.size()));
	 break;
	 }
      
      }

   return ret;
}

template<class V>
void print_vector(vector<V> vec)
{
   for (int i = 0; i < vec.size(); i++)
      cout << i << ": '" << vec[i] << "'" << endl;
}

int main()
{
  char * foo = NULL;
  char prompt[10];
  sprintf(prompt, "%s", "> ");
  using_history();

  string inpt;

  foo = readline(prompt);
  while (foo[0] != '$')
    {

      foo = readline(prompt);
      std::cout << "Ok." << endl;
      inpt = foo;
      print_vector(tokenize(foo));

      /* Remember non-blank lines in history. */
      if (foo && *foo)
	add_history(foo);

      if (inpt == "history")
	{
	  cout << "History:\n";
	  register HIST_ENTRY **h = history_list();
	  if (!h)
	    continue;
	  for (int i = 0; h[i]; i++)
	    cout << h[i]->line << endl;
	
	}

      /* Give the memory back so we can start again. */
      free(foo);
      sprintf(prompt, "%i > ", history_length);

    }
  return 0;
}

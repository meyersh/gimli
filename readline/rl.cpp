#include <iostream>
#include <string>

#include <stdio.h>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

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
	    cout << h[i] << endl;
	
	}

      /* Give the memory back so we can start again. */
      free(foo);
      sprintf(prompt, "%i > ", history_length);

    }
  return 0;
}

include <wordexp.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define SESSION_SUFFIX ".session"
#define SESSION_PATH "/home/meyersh/private"


int main(int argc, char* argv[])
{
  string glob = "*";
  glob += SESSION_SUFFIX;

  wordexp_t p;
  char **w;
  int i;
  chdir(SESSION_PATH);
  wordexp(glob.c_str() , &p, 0);
  w = p.we_wordv;
  for (i=0; i<p.we_wordc; i++)
    cout << w[i] << endl;
  wordfree(&p);
  return 0;
}

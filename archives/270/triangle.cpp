/***
 * Shaun Meyer goes for BONUS POINTS
 **/
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
  int w = 0;
  if (argc > 1) {
    w = atoi(argv[1]);
  } else {
    cout << "USAGE: " << argv[0] << " <integer>\n";
    cout << "May I please have an integer?>> ";
    cin >> w;
  }

  int maxw = 4*w;

  for (int r = 0; r < w; r++)
    {
      for (int c = 0; c < (w * 4); c++)
	{
	  /* insert manditory spaces */
	  if (c && c % w == 0)
	    cout << " ";

	  if (c < maxw/2) { /* first half */
	    if (c < maxw/4) { /* first quarter */
	      if (c <= r) 
		cout << 'X';
	      else 
		cout << '_';
	    }
	    else { /* 2nd quarter */
	      if (c < w-r+w) 
		cout << 'X';
	      else 
		cout << '_';
	    }
	  }
	  else { /* 2nd half */
	    if (c < maxw*3/4) { /* 3rd quarter */
	      if (c < r + w*2)
		cout << '_';
	      else
		cout << 'X';
	    } 
	    else { /* 4th quarter */
	      if (c < w-1-r+w*3)
		cout << '_';
	      else
		cout << 'X';
	    }
	  }
	}
      cout << endl;
    }

}

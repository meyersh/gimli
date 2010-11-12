#include <iostream>

using namespace std;

int main()
{
  int w = 4;

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
		cout << '4';
	      else 
		cout << 'X';
	    }
	    else { /* 2nd quarter */
	      if (c < w-r+4)
		cout << '4';
	      else 
		cout << 'X';
	    }
	  }
	  else { /* 2nd half */
	    if (c < maxw*3/4) {
	      if (c < r + w*2)
		cout << 'X';
	      else
		cout << '4';
	    } 
	    else { /* 3rd quarter */
	      if (c < w-1-r+w*3)
		cout << 'X';
	      else
		cout << '4';
	    }
	  }
	}
      cout << endl;
    }

}

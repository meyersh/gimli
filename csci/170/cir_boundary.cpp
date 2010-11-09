#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>

using namespace std;


int beg=0,
  bsize=0,
  blen,
  i=0; /* index */

int end()
{
  return (beg+bsize-1)%blen;
}


int next()
{
  return (i+1) % blen;
}

char *getBuffer(char buff[])
{
  char *arr = new char[blen];
  
  if (!arr)
	{
	  cout << "Oops! Unable to allocate memory\n";
	}
  else
	{
	  int ouri = beg,
		index = 0;
	  
	  while ( index < blen )
		{
		  arr[index] = buff[ouri];
		  index++;
		  ouri = (ouri+1) % blen; /* until I re-write next() to handle this situation. */
		}
	  return arr;
	}
  
  /*
	when done - delete[] arr;
  */
}
int prev()
{
  int p = (i-1) % blen;
  /* While the above works with negative numbers in Python,
	 C is not so forgiving. */
  
  if (p < 0)
	return p+blen;
  else
	return p;
}

void push(char c, char buff[])
{
  // Maintain the bsize int
  if (bsize < blen)
	bsize++;
  else if (bsize == blen)
	{
	  /* We're overwritting... update `beg' variable */
	  beg = next();
	}

  // Push in the char
  buff[i] = c;
  i = next();
}

char pop(char buff[])
{
  if (bsize > 0)
	bsize--;

  // Pop out a character
  i = prev();
  return buff[i];
}

int
main (int argc, char* argv[])
{
  string boundary = "ABC123";
  blen = boundary.size(); // length of buffer
  char buff[blen]; // Our circular buffer
  beg=0; // pointers to end and begining of data in circular data
  push('A', buff);
  push('B', buff);
  push('C', buff);
  push('D', buff);
  push('E', buff);
  push('F', buff);
  push('G', buff);
  push('H', buff);
  push('I', buff);

  
  cout << blen << endl;
  /*  cout << i << " " << pop(buff) << endl;
  cout << i << " " << pop(buff) << endl;
  cout << i << " " << pop(buff) << endl; */
  cout << buff << endl;
  cout << beg << " " << end() << endl;
  char *bb = getBuffer(buff);
  cout << bb << endl;
  delete[] bb;
  
  return 0;
}
  

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

/*
 * Impliment RFC1341 multipart.
 *
 * Shaun Meyer, Mar 2010
 */

#define MAX_BOUNDARY_LENGTH 70 /* Boundaries may not be longer than 70 characters. */


int
main(int argc, char* argv[])
{
  char boundary[MAX_BOUNDARY_LENGTH];
  cout << "\n\n" << argc << "\n\n";
  if (argc != 2)
	{
	  strcpy(boundary, "ABC123This is some");
	}
  else
	{
	  char boundary[MAX_BOUNDARY_LENGTH];
	  strcpy(boundary, argv[1]);
	  cout << "\nUsing '" << boundary << "' as boundary." << endl;
	}

  int blen=0; /* length of our boundary key */
  for (int i=0; boundary[i]; i++)
	blen++;
	  
  char data[] = "ABC123This is some data. I hope ABCyou like it!ABC123Here is some more data!";
  char pdata[1024]; /* output buffer */
  char buffer[MAX_BOUNDARY_LENGTH] = "";
  int matches = 0; /* character matches (up to blen) */
  int gm = 0; /* total matches, ever. */
  int buffoff; /* Buffer offset
				* 'I' keeps advancing even if we don't do anything. This buffer offset is
				* used for legibility, it is calculated so that we put data in the right
				* place in pdata[] */
  int bsize=0; /* current amount of data in the buffer */

  for (int i=0; data[i]; i++)
	{
	  if ( matches == blen )
		{
		  // Positive boundary match!
		  matches = 0;
		  gm += blen;
		  buffer[0]=NULL;
		  bsize=matches;
		}
	  else if ( matches )
		{
		  // We're matching -- Buffer it in case of a mis-match
		  buffer[bsize] = data[i];
		  buffer[bsize+1] = NULL; //erase ahead
		  bsize++;
		  pdata[i-gm] = '_'; // for printing, the i's we've overlooked are written with something.
		  pdata[i-gm+1] = NULL;
		}
	  
	  if ( data[i] == boundary[matches] && matches < blen)
		{
		  // Character match from boundary in our stream and we've not checked entire boundary
		  if (!matches)
			{
			  buffer[bsize] = data[i];
			  buffer[bsize+1] = NULL; //erase ahead
			  bsize++;
			  pdata[i-gm] = '_'; // for printing
			}
		  matches++;
		}
	  else if ( data[i] != boundary[matches] && matches  )
		{
		  // Incomplete match. Copy buffer to pdata

		  // By the way, we're 1 ahead of where we want it because we
		  // had to move forward for a failed match.
		  i++;
		  
		  for (matches=bsize; buffer[bsize-matches]; matches--)
			{
			  buffoff = bsize-matches;
			  pdata[i-bsize-gm+buffoff] = buffer[buffoff];
			  pdata[i+1-bsize-gm+buffoff] = NULL;
			  buffer[buffoff] = NULL;
			}
		  i--;
		  bsize=matches=0; // which is zero at this point
		  cout << endl;
		}
	  else if ( !matches )
		{
		  // Not matching, nor have we just come from a match process...
		  pdata[i-gm] = data[i];
		  pdata[i-gm+1] = NULL;
		}
	  else
		{
		  cout << "Whoa, what happened?" << endl;
		}
	  
	  	  cout << "I:" << i
		   << " m:" << matches
		   << " gm:" << gm
		   << " pdata:" << pdata << endl
		   << "buffer(" << bsize << "):" << buffer << endl; 
	 
	}

  cout << pdata << endl;
  return 0;
}

/******************************************************************************
 * CSCI 270 Project 1 
 * Shaun Meyer & Sean Richardson
 * Oct, 2010
 * 
 * Goals: ENCODE / DECODE / CRACK using a XOR scheme.
 *
 * How it works:
 * We expect one line from STDIN containing 
 * <COMMAND> <KEY 0-256> <DATA>\n
 * 
 * Where COMMAND is "ENCODE", "DECODE" or "CRACK".
 * If we are using CRACK code, <KEY> is omitted.
 * 
 * The CRACK function merely counts the number
 * of occurrences in a data-string and assumes
 * that the most common is a space. 
 * 
 * Based on that it tries to decode the string. If
 * it encounters a non-printable character in the 
 * decoded-string we assume we're wrong and immedietly
 * abort with a message.
 *
 ******************************************************************************/

#include <iostream>
#include <iomanip> // for hex
#include <vector>
#include <string>

using namespace std;

string decode_string(const string &data, unsigned char key)
{
  string ret; /* this is the string we eventually return */
  unsigned char c;

      /* all our input must be hex...*/
      for (int i = 0; i < data.size(); i++)
	{
	  /* Decode some HEX */
	  c <<= 4;

	  if (data[i] >= '0' && data[i] <= '9')
	      c += data[i] - '0';
	  else if (data[i] >= 'A' && data[i] <= 'Z')
	      c += data[i] - ('A'-10);
	  else if (data[i] >= 'a' && data[i] <= 'z')
	    c += data[i] - ('a'-10);

	  if (i%2)
	    ret += (char)(c ^ key);

	}
      return ret;
}



int main()
{
  unsigned int key;
  unsigned char c = 0;

  string data,command;
  vector<unsigned char> encryptedData;

  cout << "Content-Type: text/plain\n\n";

  cin >> command;
  
  if (command != "CRACK")
    {
      cin >> key;
      cin.ignore(1); /* we were getting a space before our DATA line below. This 
		      * eats the space. */

      getline(cin, data, '\n');

    }
  else // we're CRACKing
    {
      cin >> data;
    }

  if (command == "ENCODE") 
    {
      for (int i = 0; i < data.size(); i++)
	{
	  /* Insert newline for every 60 characters. 
	   * it makes the output prettier but cut and paste
	   * is more challenging. */

	  if (i % 60 == 0)
	    cout << "<br>\n";
	 
	  encryptedData.push_back(key ^ data[i]);
	  cout << setw(2) << setfill('0') << hex << (key ^ data[i]);
	}
      cout << endl;
      
    }
  else if (command == "DECODE")
    {
      cout << decode_string(data, key) << endl;
    }
  else if (command == "CRACK")
    {
      int occurences[256];

      /* 
       * Count the number of occurences 
       */

      for (int i = 0; i < data.size(); i++)
	{
	  c <<= 4;

	  if (data[i] >= '0' && data[i] <= '9')
	      c += data[i] - '0';
	  else if (data[i] >= 'A' && data[i] <= 'Z')
	      c += data[i] - ('A'-10);
	  else if (data[i] >= 'a' && data[i] <= 'z')
	    c += data[i] - ('a'-10);

	  if (i%2) // we've got a byte!
	    occurences[c]++;
	  
	} // end for-loop

      /*
       * Find the most popular character-code
       */

      unsigned char most_popular = 0;
      
      for (int i = 0; i < 256; i++)
	{
	  if (occurences[i] > occurences[most_popular])
	    most_popular = i;
	}

      cout << "The most popular character is: " << (int)most_popular << "<br>\n";

      string possible_decoded = decode_string(data, 32 ^ most_popular);
      bool decoded_correctly = true;

      /*
       * Check for non-printable characters:
       */

      for (int i = 0; i < possible_decoded.size(); i++)
	{
	if (possible_decoded[i] < 32 || possible_decoded[i] > 126)
	  {
	    decoded_correctly = false;
	    break;
	  }
	}
      
      if (decoded_correctly)
	cout << "Code may be: " << possible_decoded << endl;
      else
	cout << "Your code could not be cracked!\n";
    }
}


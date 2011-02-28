/******************************************************************************
 * Slice 3 Project
 * Team Members: Shaun Meyer + Steven Throne
 * CGI by Shaun Meyer, Oct 2010
 *
 * If I had to write this again, I'd use an object w/ methods. 
 * I wrote this in a traditional C-style and passing all 
 * these function parameters (let alone, remembering) was a huge
 * pain.
 *
 *****************************************************************************/

#include <iostream>
#include <climits>
#include <cmath>

using namespace std;

/* strBinToBoolAry & strBinToInt both depend heavily on the error
 * checking that occurs in main() before they are called. Their 
 * existance is merely to easily facilitate the porting of this
 * from a command-line app to CGI-based.
 */

/* strBinToBoolAry(&ary, &string) - return a 38-bit bool array from our
 * string input... */

void strBinToBoolAry(bool * ary, const string &binaryinput)
{
  for (int i = 0; i < 38; i++)
    {
      ary[i] = binaryinput[i]-'0'; /* should work, quick and dirty 
				      conversion from ASCII to INT */
    }
}

/* strBintoInt(string) - return a 32- binary string as an unsigned int... */

unsigned int strBinToInt(const string & binaryinput)
{
  int ret = 0;
  for (int i = 0; i < 32; i++)
    {
      ret <<= 1;
      if (binaryinput[i] == '1')
	ret += 1;
    }
  return ret;
}

/* showBinary(unsigned int) - output the binary 
 * to stdout 
 */
void showBinary(unsigned int foo)
{
  int length = sizeof(foo)*8;
  unsigned int mask = pow(2, length-1);

  for (int i = 0; i < length; i++)
    {
      if (mask & foo)
	cout << "1";
      else
	cout << "0";
      mask = mask >> 1;
    }
  cout << endl;
}

/* showBinary( bool[], length ) - output the binary
 * to stdout
 */
void showBinary(bool * word, int length, bool insertSpaces=false)
{
  for (int i = 0; i < length; i++)
    {   
      cout << word[i];

      if ((i+1)%8 == 0 && insertSpaces)
	cout << " ";
    }

  cout << endl;
}


/*
 * hammingBits(length) - return the number of hambits for length-bit binary.
 *
 * When in doubt - use brute force. We loop through all 
 * the numbers up to oru length to find out how many hambits
 * we'll need.
 *
 * Based on the formula: (m + r + 1) <= 2^r
 */

int hammingBits(int length)
{
  int hambits = 0;
  for (hambits = 0; hambits < length; hambits++)
    if (hambits + length + 1 <= pow(2, hambits))
      return hambits;
  return -1;
}

/* 
 * setHambits() - 
 * set the hambits in our word. We take in length of word
 * and number of desired hambits.
 * Also, we can calculate both odd / even parity.
 */

void setHambits(bool * word, int length, int hambits, bool evenParity=true)
{
  int numOnes = 0; // number of ones. 
  int checkh; // index we're checking.

  //  int * hamchecks = new int[hambits]; // this is where we store the number of ones.

  /* check parity for each hambit */
  for (int h = 0; h < hambits; h++)
    {
      checkh = pow(2, h)-1; // the hambit (index) we'll modify
      numOnes = 0; // until we count them below:

      for (int i = 0; i < length+hambits; i++)
	{
	  if (i == checkh)
	    continue; // don't check ourselves.

	  if (i+1 & checkh+1 && word[i]) /* i is a one bit checkh is responsible for. */
	    numOnes++;
	}
  
      if ( numOnes % 2 ) /* its an odd number */
	word[checkh] = evenParity; 
      else
	word[checkh] = !evenParity;
    }
}

/* 
 * isHambit(index, length) - is a given index in length a hambit?
 */

bool isHambit(int index, int length)
{
  /* hambit is pow(2, i)-1 so the inverse must be true... */

  int h; 

  for (int i = 0; i < hammingBits(length); i++)
    {
      h = pow(2, i)-1;
      if (h == index)
	return true;
    }
  return false;
  
}

/*
 * encodeWord(bool word[], unHammedWord, wordLength, inthambits)
 * put unHammedWord binary into bool word[] with empty spaces for
 * hambit locations.
 * 
 */

void 
encodeWord(bool * word, unsigned int unHammedWord, 
	   int wordLength, int hambits)
{
  unsigned int mask = 1;
  mask <<= sizeof(unHammedWord)*8-1;
  for (int i = 0; i < wordLength+hambits; i++)
    {
      if (isHambit(i, wordLength))
	{
	  continue;
	}

      if (mask & unHammedWord)
	word[i] = 1;
      else
	word[i] = 0;
      mask >>= 1;
      
    }
}

/*
 * given a bool * word, return the integer it holds. 
 */

unsigned int
decodeWord(bool * word, int wordLength, int hambits)
{
  unsigned int value = 0;

  for (int i = 0; i < wordLength+hambits; i++)
    {
      if (isHambit(i, wordLength))
	continue;
      
      value <<= 1;
      
      if (word[i])
	value |= 1;
      
    }
  return value;
}

/*
 * checkHambits() - given a codeword, length, and (even/odd) parity return
 * the index of the incorrect bit for repair.
 */
int checkHambits(bool * word, int length, int hambits, bool evenParity = true)
{

  /* This deserves a comment. H is our current ham bit, 
   * hh is our sub-loop hambit that we're comparing to.
   */

  int * hamchecks = new int[hambits];
  int checkh = 0; 
  for (int h=0; h < hambits; h++)
    {
      hamchecks[h] = 0; /* initialize our array */
      
      for (int i = 0; i < length+hambits; i++)
	{
	  checkh = pow(2,h);
	  if ( (i+1) & checkh && word[i] )
	    {
	      // h-bit checks i-bit...
	      hamchecks[h]++;
	    }
	}
    }

  cout << "Results: ";
  if (evenParity)
    cout << "(even parity)\n";
  else
    cout << "(odd parity)\n";

  cout << "<br>\n";
  int errBit = 0;
  bool errorsDetected = false;

  for (int i = 0; i < hambits; i++)
    {
      cout << pow(2,i) << " = " << hamchecks[i] << ": ";
      if ( hamchecks[i] % 2 ) /* its an odd number */
	{
	  if (evenParity)
	    {
	      cout << "FAILED<br>\n";
	      errBit += pow(2,i);
	      errorsDetected = true;
	    }
	  else
	    cout << "PASSED<br>\n";
	}
      else /* it's an even number */
	{
	  if (evenParity)
	    cout << "PASSED<br>\n";
	  else
	    {
	    cout << "FAILED<br>\n";
	    errBit += pow(2,i);
	    errorsDetected = true;
	    }
	}
	
    }

  if (errorsDetected)
    if (errBit > length+hambits)
      cout << "Uncorrectable multi-bit error detected.<br>\n";
    else
      cout << "Error in bit " << errBit << "... corrected.<br>" << endl;

  if (errorsDetected)
    word[errBit-1] = !word[errBit-1];

  delete [] hamchecks;

}

int main()
{
  
  /* My input should look like one of the following:

     <ENCODE/DECODE>
     1111-111                                    [32- or 38-bits]
     <ODD/EVEN>

     if it doesn't, abort `cause something is wrong. */

  cout << "Content-Type: text/html\n\n";

  string command, inputbinary, parityType;

  cin >> command;

  if (!(command == "ENCODE" || command == "DECODE" ))
    {
      cout << "Invalid command entered!\n";
      cout << "'" << command << "'\n";
      return -1;
    }

  cin >> inputbinary;

  if ((inputbinary.size() != 32 && command == "ENCODE")
      || (inputbinary.size() != 38 && command == "DECODE"))
    {
      cout << "Invalid word length for this command.\n";
      return -1;
    }

  for (int i = 0; i < inputbinary.size(); i++)
    {
      if (!(inputbinary[i] == '1' || inputbinary[i] == '0'))
	{
	  cout << "Invalid characters in input binary!\n";
	  return -1;
	}
    }

  cin >> parityType;
  bool evenParity;

  if (!(parityType == "ODD" || parityType == "EVEN" ))
    {
      cout << "Invalid parity type specified.\n";
      return -1;
    }
  else if ( parityType == "ODD" )
    evenParity = false;
  else if ( parityType == "EVEN" )
    evenParity = true;

  /* If we've made it so far, we're golden. */

  if (command == "ENCODE")
    {
    
      unsigned int unHammedWord = strBinToInt(inputbinary);
      unsigned int wordLength = sizeof(unHammedWord)*8;
  
      int hambits = hammingBits(wordLength);

      bool * hammedWord = new bool[wordLength+hambits-2];

      cout << "Data Value: " << unHammedWord << " wordLength: " << wordLength << "<br>\n";

      // showBinary(unHammedWord);

      cout << "We require " << hambits << " ham bits.<br>\n";
  
      /* populate our hamming word - HamBits are left undefined here. */
      encodeWord(hammedWord, unHammedWord, wordLength, hambits);

      /* set all the hambits in our populated hamming word to their correct 
       * values. */
      setHambits(hammedWord, wordLength, hambits, evenParity);

      cout << "Hammed Word: " << endl;
 
      showBinary(hammedWord, wordLength+hambits);
      delete [] hammedWord; 
  /* int errBit = 17;

  cout << "\nHammed word with 1-bit error (bit: " << errBit << ")\n";
  hammedWord[errBit-1] = !hammedWord[errBit-1]; // simulate a 1-bit error.
  hammedWord[34] = !hammedWord[34]; // a second error. */

  //showBinary(hammedWord, wordLength+hambits);

  // cout << endl;
  // checkHambits(hammedWord, wordLength, hambits, evenParity);
  //cout << "\nOriginal value: " << decodeWord(hammedWord, wordLength, hambits) << endl; */
    }
  else // we're DECODEing...
    {
      unsigned int wordLength = 32; // for this assignment.
      int hambits = hammingBits(wordLength);

      bool * hammedWord = new bool[wordLength + hambits-2];

      strBinToBoolAry(hammedWord, inputbinary);
      checkHambits(hammedWord, wordLength, hambits, evenParity);
      cout << "Corrected binary codeword: ";
      showBinary(hammedWord, wordLength+hambits);
      cout << "<br>";
      cout << "Data Value: " << decodeWord(hammedWord, wordLength, hambits) << "<br>\n";
      delete [] hammedWord;
    }


  return 0;


}

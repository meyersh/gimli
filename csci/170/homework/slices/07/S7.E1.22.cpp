/*******************************************************************************
 * Shaun Meyer ( Feb 1 2010 )
 * S6.E1.3
 * "Programming Challenge 21 from the end of chapter 3 of the C++ book."
*******************************************************************************/
#include <iostream>
#include <fstream>

using namespace std;


/*************************************************************
 * Get 5 numbers (double) from the user and save them to a file.
 * 
 * INPUTS:
 * doubles a, b, c, d, and e
 * 
 * ALGORITHM:
 * File is space delimited, write the 5 doubles to file 
 * seperated by space.
 *
 * OUTPUT:
 * textfile (newline & space delimited)
*************************************************************/

int
main() 
{
    const char* filename = "/home/meyersh/fivenumbers.txt";
    
    ofstream f;

    f.open(filename);

    // We're on the WEB now!
    cout << "Content-Type: plain/text\n\n";

    if (!f) 
    {
        cout << "notice=Error opening file for writing!\n";
        return -1;
    }

    // cout << "Gimme 5 numbers for the file! They may be space or newline delimited!\n>>";

    double a,b,c,d,e;

    cin >> a >> b >> c >> d >> e;

    f << a << " "
      << b << " "
      << c << " "
      << d << " "
      << e << endl;

    f.close();
    cout << "notice=File written and closed.\n";

    return 0;

}

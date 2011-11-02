/*******************************************************************************
 * Shaun Meyer ( Feb 1 2010 )
 * S6.E1.31
 * "Programming Challenge 21 from the end of chapter 3 of the C++ book."
*******************************************************************************/
#include <iostream>
#include <fstream>

using namespace std;


/*************************************************************
 * Get 5 numbers (double) from a file and return their SUM
 * INPUT:
 * (NONE/FILE) 
 * 
 * ALGORITHM:
 * total = a+b+c+d+e
 *
 * OUTPUT:
 * total
*************************************************************/

int
main() 
{
    const char* filename = "/home/staff/meyersh/fivenumbers.txt";
    
    ifstream f;

    f.open(filename);

    // We're on the WEB now...
    cout << "Content-Type: text/plain\n\n";

    if (!f) 
    {
        cout << "notice=Error opening file!\n";
        return -1;
    }

    cout << "notice=Reading file...\n";

    double a,b,c,d,e;

    f >> a >> b >> c >> d >> e;

    cout << "a=" << a << endl
         << "b=" << b << endl
         << "c=" << c << endl
         << "d=" << d << endl
         << "e=" << e << endl
         << "total=" << a+b+c+d+e 
         << endl;

    f.close();

    return 0;

}

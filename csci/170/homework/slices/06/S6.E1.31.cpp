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
    const char* filename = "./fivenumbers.txt";
    
    ifstream f;

    f.open(filename);

    if (!f) 
    {
        cout << "Error opening file!\n";
        return -1;
    }

    cout << "Reading file...\n";

    double a,b,c,d,e;

    f >> a >> b >> c >> d >> e;

    cout << a+b+c+d+e << endl;

    f.close();

    return 0;

}

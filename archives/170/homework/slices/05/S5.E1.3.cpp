/*******************************************************************************
 *
 * Shaun Meyer (Jan 22 2010)
 * S5.E1.3
 * "Programming Challenge 16 from the end of chapter 2 of the C++ book."
 ******************************************************************************/

#include <iostream>
#include <cstdio>
using namespace std;

/*************************************************************
 * Draw a diamond shape on the screen using astericks.
 *
 ************************************************************/

int
main() 
{

    /* We're online - lets get the content-type right: */
    cout << "Content-Type: text/html\n\n";

    char mark='*'; /* We're using astericks for this example. */

    int inc=2;     /* Amount of change in astericks from last row.
                      Always needs to be an even number. */

    int imax=7;    /* Max width (measured in "marks") 
                      always needs to be an odd number. */

    int spaces;    /* Spaces to pad before the marks on a given row. 
                      Calculated by ( (imax-i)/2 ) */

    int i=1;       /* number of marks to draw on a given row. 
                      In this case, we start with 1 on the point,
                      setting i=imax would only draw the lower half  */

    int icounter;  /* "temporary" variable used while counting 
                       "marks". */

    /* Randy - This program is ``totally'' ready to accept input to change the
       dimensions of the diamond - how can I use the query string to set width
       and increment like I was doing from the console before? */

    /**************************************************** 
        Get user input regarding the size of this diamond 
    *****************************************************/

    cin >> imax;

    if ( imax < 2 ) 
        imax += inc;
    if (! (imax%2) ) 
        imax++;

    cin >> inc;

    if (inc < 2 )
        inc = 2;
    if (inc > imax/2)
        inc = 2;
    if (inc%2) 
        inc++;

    /**************************************************** 
     * Based on the above calculated and definitions, 
     * draw our diamond. When i is 0 we're done.
     ***************************************************/

    cout << "<pre>\n"; // for the interwebs
    while (i > 0) 
    {

        // print the requisite number of spaces...

        for (spaces = (imax-i)/2; spaces > 0; spaces--) {
            cout << " ";
        }

        // print out the requisite number of "marks"...

        for (icounter = i; icounter > 0; icounter--)
        {
            cout << mark;
        } 

        // End of this line -
        cout << endl;

        i += inc;

        // Do we need to change directions?

        if (i == imax)
            inc *= -1;

    } 

    cout << "</pre>\n"; // for the interwebs
    return 0;

}


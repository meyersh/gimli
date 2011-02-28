/*******************************************************************************
 * Shaun Meyer ( Feb 9 2010 )
 * S7.E1.1
 * "Programming Challenge 2 from the end of chapter 3 of the C++ book."
 * -- Modified to work with AJAX for Slice 7
******************************************************************************/

#include <iostream>
#include <iomanip>

using namespace std;


/*************************************************************
 * Stadium Seating -
 * INPUT:
 * number of Class-A, B, C tickets sold
 * ALGORITHM:
 * Class A Seats: $15, B: $12, C: $9.
 * OUTPUT:
 * Income from ticket sales in fixed-point, two decimal places,
 * always displaying the decimal point.
*************************************************************/

#define CLASS_A_COST 15.00
#define CLASS_B_COST 12.00
#define CLASS_C_COST 9.00

int 
main ()
{
    // Storage variables for our totals
    double classA = 0.00,
           classB = 0.00,
           classC = 0.00,
           income = 0.00;

    // Quantity of tickets sold
    int classAsold = 0,
        classBsold = 0,
        classCsold = 0;
    
    // 1. INPUT
    cout << "Number of Class A tickets sold? >>";
    cin >> classAsold;
    cout << "Number of Class B tickets sold? >>";
    cin >> classBsold;
    cout << "Number of Class C tickets sold? >>";
    cin >> classCsold;

    // 2. ALGORITHM
    // calculate the totals...
    classA = (classAsold * CLASS_A_COST);
    classB = (classBsold * CLASS_B_COST); 
    classC = (classCsold + CLASS_C_COST);
    income = classA + classB + classC;

    // We're on the web now...
    cout << "Content-Type: text/html\n\n";
    cout << "<pre>\n";

    // 3. OUTPUT TOTAL(S)
    cout << setprecision(2) << fixed << showpoint 
         << "\n\nIncome Generated:\n"
         << "---------------------\n"
         << "Class A Tickets: " << classAsold 
            << " ( $" << classA << " )" << endl
         << "Class B Tickets: " << classBsold 
            << " ( $" << classB << " )" << endl
         << "Class C Tickets: " << classCsold 
            << " ( $" << classC << " )" << endl
         << "Total Income:  $" << income << endl
         << endl;

    cout << "</pre>\n";

    return 0;
}


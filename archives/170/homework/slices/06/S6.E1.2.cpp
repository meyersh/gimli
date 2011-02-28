/*******************************************************************************
 * Shaun Meyer ( Feb 1 2010 )
 * S6.E1.2
 * "Programming Challenge 17 from the end of chapter 3 of the C++ book."
*******************************************************************************/
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;


/*************************************************************
 * Calculate the monthly payment on a loan.
 * 
 * INPUT:
 * RATE (Annual interest rate divided by 12)
 * N: Number of payments
 * L: Amount of Loan
 *
 * ALGORITHM:
 * Payment = ( (Rate * (1+Rate)^N) / ((1+Rate)^N-1) ) * L
 * 
 * OUTPUT: (Example)
 * Loan Amount:                     $10000.00
 * Monthly Interest Rate:                  1%
 * Number of Payments                      36
 * Monthly Payment:                 $  332.14
 * Amount Paid Back:                $11957.15
 * Interest Paid:                   $ 1957.15
 *
*************************************************************/

int
main() 
{
    // 0 - Definitions
    double  loanamt = 0.0,
               rate = 0.0,
            payment = 0.0,
      totalinterest = 0.0;
    int numpayments = 0;    


    // 1 - INPUTS
    cout << "What is the amount of your loan? >> $";
    cin >> loanamt;

    cout << "What is your Annual Interst rate?>> %";
    cin >> rate;

    rate /= 100; // Presumably this is in %/100 notation.

    cout << "How many payments do you have?  >> ";
    cin >> numpayments;

    // 2- ALGORITHM
    rate /= 12; // We only want the monthly rate.

    payment = ( 
        (rate * pow((1+rate), numpayments) ) / 
        ( pow((1+rate),numpayments)-1) 
        ) * loanamt;

    
cout << setprecision(2) << fixed << showpoint 
     << "\nYour loan:\n\n"
     << "Loan Amount:                     $" << setw(10) << loanamt << endl
     << "Monthly Interest Rate:           " << setw(10) << rate*12*100 << "%" << endl
     << "Number of Payments                " << setw(10) << numpayments << endl
     << "Monthly Payment:                 $" << setw(10) << payment << endl
     << "Amount Paid Back:                $" << setw(10) << (payment * numpayments) << endl
     << "Interest Paid:                   $" << setw(10) << (payment * numpayments) - loanamt << endl;

    return 0;

}

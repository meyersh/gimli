/*******************************************************************************
 *
 * Shaun Meyer (Jan 22 2010)
 * S5.E1.2
 * "Programming Challenge 4 from the end of chapter 2 of the C++ book."
 ******************************************************************************/

#include <iostream>
#include <cstdio>

using namespace std;

/*************************************************************
 * Restaurant Bill.
 * Compute the Tax & Tip on a restaurant bill for a patron
 * with a $44.50 meal charge. Tax should be 6.75% of the meal
 * cost. 
 *
 * The tip should be 15% of the total (after taxes) 
 *
 * Input: None
 * Output: Meal cost, tax amount, tip amount, and total bill.
 ************************************************************/

int
main() 
{
    double meal_cost, tax_rate, tax_amount, tip_rate, tip_amount, total;

    meal_cost = 44.50;
    tax_rate = .0675;

    tax_amount = meal_cost * tax_rate;

    tip_rate = .15;
    
    tip_amount = (meal_cost + tax_amount) * tip_rate;

    total = meal_cost + tax_amount + tip_amount;

    printf("Content-Type: text/plain\n\n");

    /* I hope this format isn't too obfuscated, multiline quotes didn't
       work as I expected. */

    printf("Your Bill\n\n Meal cost: $%2.2f\n Tax amount: $%1.2f\n", 
            meal_cost, tax_amount);

    printf("Tip amount: $%1.2f\n ---------------------\n Total:     $%2.2f\n\n",
            tip_amount, total);

    return 0;
}


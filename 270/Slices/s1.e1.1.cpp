/*******************************************************************************
 * CSCI-270 : S1.E1.1
 * Shaun Meyer
 * Sept, 2010
 ******************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

int findLowest(vector<int>&);
float calcAverage(vector<int>&);

int main()
{
  vector<int> numbers;
  int i;

  while (cin >> i)
    {
      numbers.push_back(i);

    }

  cout << "Content-Type: text/plain\n\n";
  cout << calcAverage(numbers) << endl;


  return 0;
}

/* 
 * return the lowest element from a vector of integers. 
 */

int findLowest(vector<int>& numbers)
{
  int lowest=numbers[0];
  for (int i=1; i < numbers.size(); i++)
    if (numbers[i] < lowest)
      lowest = numbers[i];

  return lowest;
}

/* return a float representing the average from a vector
 * of integers with the lowest value dropped. */

float calcAverage(vector<int>& numbers)
{

  int sum=0;

  for (int i = 0; i < numbers.size(); i++)
    sum += numbers[i];

  sum -= findLowest(numbers);

  return sum / numbers.size()-1;

}

   

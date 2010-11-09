/*******************************************************************************
 * CSCI-270 : S3.E1.1
 * Shaun Meyer
 * Sept, 2010
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int findLowest(vector<int>&);
int findHighest(vector<int>&);
float calcAverage(vector<int>&);
void print_vector(vector<int>&);
/*
 * ** If there are no scores it should return an error message saying that 
 *    there is no average because there are no scores. 
 * 
 * ** If there are 1-4 scores it should return the average of those 4 scores 
 *
 * ** If there are 5 or more scores throw away the top and the bottom score 
 *    and return the average of the rest. 
 * 
 * The scores brought into the CGI script should be converted to numbers 
 * and stored in a vector.
 */


int main()
{
  vector<int> numbers;
  int i;

  /* get all the numbers to populate the vector */
  while (cin >> i)
      numbers.push_back(i);

  cout << "Content-Type: text/plain\n\n";

  /* 
   * IF there are 1-4 scores, return the average of those scores.
   * 
   * IF there are >= 5 scores throw away top & bottom and avg the
   * rest.
   */

  if (numbers.size() < 5)
    cout << calcAverage(numbers) << endl;
  else
    {
      sort(numbers.begin(), numbers.end());
      print_vector(numbers);
      cout << "<br>\n";

      // Drop the highest:
      numbers.erase(numbers.begin() + findHighest(numbers));
      // Drop the lowest:
      numbers.erase(numbers.begin() + findLowest(numbers));
      cout << "The average: " << calcAverage(numbers) << "<br>" << endl;
    }

  return 0;
}

/* 
 * return the lowest element from a vector of integers. 
 */

int findLowest(vector<int>& numbers)
{
  int lowest=0;
  for (int i=1; i < numbers.size(); i++)
    if (numbers[i] < numbers[lowest])
      lowest = i;

  return lowest;
}

/*
 * return the highest element from a vector of integers.
 */

int findHighest(vector<int>& numbers)
{
  int max=0;
  for (int i=1; i< numbers.size(); i++)
    if (numbers[i] > numbers[max])
      max = i;

  return max;
}

/* return a float representing the average from a vector
 * of integers with the lowest value dropped. */

float calcAverage(vector<int>& numbers)
{

  int sum=0;

  for (int i = 0; i < numbers.size(); i++)
    sum += numbers[i];

  return sum / numbers.size();

}

void print_vector(vector<int>& ary)
{
  for (int i = 0; i < ary.size(); i++)
    cout << ary[i] << " ";
  cout << endl;
}


   

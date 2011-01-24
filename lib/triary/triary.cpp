#include "triary.hpp"

/*
 * This is SUPER frustrating!
 * http://www.parashift.com/c++-faq-lite/templates.html#faq-35.12
 *
 * The solution may be to use inline functions?
 * http://www.parashift.com/c++-faq-lite/inline-functions.html
 */

template<class V>
V& triary<V>::access(int row, int col)
/* Unified getter+setter function for the triangle array */
{
    if (col > row)
      throw std::out_of_range("Col is greater than row.");
    if (row > num_rows)
      throw std::out_of_range("Row is greater than number of rows.");

    return triangle_array[ t(row) + col ];
}

template<class V>
void triary<V>::resize(int rows)
/* Set the size of the array to accomodate a
 * given number of rows and migrate the 
 * relevent elements */
{
  /* remember the old triangle */
  int old_size = num_rows;
  V old_array = triangle_array;
  
  /* Make a new triangle */
  triangle_array = new V[t(rows)];
  
  int boundary;
  if (rows > num_rows)
    boundary = num_rows;
  else
    boundary = rows;
      
  /* Copy the old data as far as we can */
  for (int i = 0; i < num_rows; i++)
    triangle_array[i] = old_array[i];

  delete [] old_array;
  num_rows = rows;
}

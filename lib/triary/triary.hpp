/*******************************************************************************
 * CREATED BY: Shaun Meyer, Jan-2011
 ******************************************************************************/

#ifndef __TRIARY_HPP__
#define __TRIARY_HPP__

#include <cstdlib>
#include <stdexcept>

template<class V>
class triary
{
private:
   V *triangle_array,        /* Dynamically allocated V pointer to linear array. */
      main_diagonal_element; /* value: n,n where n==n if 
				has_main_diagonal is false. */

   int num_rows;             // number of rows in triangle

   bool has_main_diagonal;   // Are we storing values for the main diagonal?

public:
  int rows() {return num_rows;}

  int t(int row)  
  /* Return the given Triangle-Number for a row. */
  {
    if (has_main_diagonal)
      return row*(row+1)/2;
    else
      return row*(row-1)/2;
  }

  V& access(int row, int col);

  void resize(int rows);
  void free_array() {delete [] triangle_array; triangle_array=NULL;}
  void create_array(int size) 
  {
    if (triangle_array)
      free_array();
    triangle_array = new V[t(size)];
  }

  triary() {num_rows = 0; triangle_array=NULL;}
  triary(int rows, bool set_main_diagonal=true, V set_diagonal_element=NULL) 
  {
    main_diagonal_element = set_diagonal_element;
    num_rows = rows; 
    has_main_diagonal=set_main_diagonal;
    triangle_array=NULL;
    create_array(rows);
  }
  ~triary() {free_array();}

};

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
    return access(col, row);
  if (row > num_rows)
    throw std::out_of_range("Row is greater than number of rows.");
  
  /* Unintended consequence warning:
   * We are returning a REFERENCE to the main_diagonal_element
   * potentially, this could be used to SET the main_diagonal_element
   * for better or worse. */
  if (col == row && !has_main_diagonal)
    return main_diagonal_element;
    
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
  V *old_array = triangle_array;
  
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

#endif

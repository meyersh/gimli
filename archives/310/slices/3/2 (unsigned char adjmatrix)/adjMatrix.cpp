/******************************************************************************
 * adjMatrix.cpp - unweighted Directed / Undirected graph matrix class
 * CREATED BY: Shaun Meyer 
 * CREATED   : 11 Feb 2011
 ******************************************************************************/

#include "adjMatrix.hpp"

/****************************************************************
 * Begin unweightedDirected adjacency matrix member functions:
 ***************************************************************/

void unweightedDirected::create (int sz)
/* if the pointer is not NULL, it first gracefully deallocates the 
   current array so that there are no memory leaks.  It then allocates 
   enough memory for an array of sz rows and columns. (Note that for 
   the triangular array only the triangle below the main diagonal needs 
   to be allocated.)  If sz < 2 set it to 2. */
{
   if (matrix)
      delete matrix;

   matrix = new bitarray(sz*sz);
   size = sz;

}

void unweightedDirected::setBit(int r, int c)
/* make the bit in row r and column c have the value 1 without changing the 
 * values of any other bits. */
{
   (*matrix)[r*size+c] = 1;
}

void unweightedDirected::clearBit(int r, int c)
/* make the bit in row r and column c have the value 0 without changing the
   values of any other bits. */
{
   (*matrix)[r*size+c] = 0;
}

void unweightedDirected::setAll()
/* make all bits in the array have the value 1.*/
{
   for (int i = 0; i < size*size; i++)
      (*matrix)[i] = 1;
}

void unweightedDirected::clearAll()
/* make all bits in the array have the value 0. */
{
   matrix->clear();
}

int unweightedDirected::checkBit(int r, int c)
/* return the value (0 or 1) of the bit in row r and column c without 
   changing the values of any bits (including those being tested). */
{
   return (*matrix)[r*size+c];
}

void unweightedDirected::print()
/* should output the array one full row at a time of  0's and 1's.  Note 
   that both, including the triangular array, should be output as a sz x sz 
   array on the screen.  (It's okay if lines too long for the screen wrap.) */
{
   for (int row = 0; row < size; row++)
      {
      for (int col = 0; col < size; col++)
	 std::cout << (*matrix)[row*size+col] << " ";
      std::cout << std::endl;
      }
}

/***************************************************************
 * Begin unweightedUndirected adjacency matrix member functions:
 **************************************************************/

void unweightedUndirected::create (int sz)
/* if the pointer is not NULL, it first gracefully deallocates the 
   current array so that there are no memory leaks.  It then allocates 
   enough memory for an array of sz rows and columns. (Note that for 
   the triangular array only the triangle below the main diagonal needs 
   to be allocated.)  If sz < 2 set it to 2. */
{
   if (matrix)
      delete matrix;

   matrix = new bitarray(t(sz));
   size = sz;

   main_diagonal = 0;

}

void unweightedUndirected::setBit(int r, int c)
/* make the bit in row r and column c have the value 1 without changing the 
 * values of any other bits. */
{
   if (c > r)
      setBit(c, r);
   else if (c == r)
      main_diagonal = 1;
   else
      (*matrix)[t(r)+c] = 1;

}

void unweightedUndirected::clearBit(int r, int c)
/* make the bit in row r and column c have the value 0 without changing the
   values of any other bits. */
{
   if (c > r)
      clearBit(c,r);
   else if (c == 0)
      main_diagonal = 0;
   else
      (*matrix)[t(r)+c] = 0;
}

void unweightedUndirected::setAll()
/* make all bits in the array have the value 1.*/
{
   for (int i = 0; i < t(size); i++)
      (*matrix)[i] = 1;
}

void unweightedUndirected::clearAll()
/* make all bits in the array have the value 0. */
{
   main_diagonal = 0;
   matrix->clear();
}

int unweightedUndirected::checkBit(int r, int c)
/* return the value (0 or 1) of the bit in row r and column c without 
   changing the values of any bits (including those being tested). */
{
   if (c > r)
      return checkBit(c, r);
   else if (c == r)
      return main_diagonal;
   else
      return (bool)(*matrix)[t(r)+c];
}

void unweightedUndirected::print()
/* should output the array one full row at a time of  0's and 1's.  Note 
   that both, including the triangular array, should be output as a sz x sz 
   array on the screen.  (It's okay if lines too long for the screen wrap.) */
{
   for (int row = 0; row < size; row++)
      {
      for (int col = 0; col < size; col++)
	 std::cout << checkBit(row, col) << " ";
      std::cout << std::endl;
      }
}

#include "adjMatrix.hpp"

std::ostream& operator<<(std::ostream& output, const bitarray::proxy& p) 
{
  output << p.parent->showBit(p.index);
  return output;  // for multiple << operators.
}

void unweightedDirected::create (int sz)
/* if the pointer is not NULL, it first gracefully deallocates the 
   current array so that there are no memory leaks.  It then allocates 
   enough memory for an array of sz rows and columns. (Note that for 
   the triangular array only the triangle below the main diagonal needs 
   to be allocated.)  If sz < 2 set it to 2. */
{
}

void unweightedDirected::setBit(int r, int c)
/* make the bit in row r and column c have the value 1 without changing the 
 * values of any other bits. */
{
}

void unweightedDirected::clearBit(int r, int c)
/* make the bit in row r and column c have the value 0 without changing the
   values of any other bits. */
{
}

void unweightedDirected::setAll()
/* make all bits in the array have the value 1.*/
{
}

void unweightedDirected::clearAll()
/* make all bits in the array have the value 0. */
{
}

int unweightedDirected::checkBit(int r, int c)
/* return the value (0 or 1) of the bit in row r and column c without 
   changing the values of any bits (including those being tested). */
{
}

void unweightedDirected::print()
/* should output the array one full row at a time of  0's and 1's.  Note 
   that both, including the triangular array, should be output as a sz x sz 
   array on the screen.  (It's okay if lines too long for the screen wrap.) */
{
}

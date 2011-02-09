#ifndef __ADJMATRIX__BINARY__HPP
#define __ADJMATRIX__BINARY_HPP

#include <iostream>
#include "bitarray.hpp"

/*
 * Abstract base class for unweighted graphs (they both have to
 * have these functions and this will make some usage cleaner.
 */

class unweightedGraph
{
public:
   virtual void create (int sz) = 0;
   virtual void setBit(int r, int c) = 0; 
   virtual void clearBit(int r, int c) = 0; 
   virtual void setAll() = 0;
   virtual void clearAll() = 0;
   virtual int  checkBit(int r, int c) = 0;
   virtual void print() = 0;
};


class unweightedDirected : public unweightedGraph
{
private:
   bitarray *matrix;
   int size; // size of one dimension
public:
   void create (int sz);
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */
   void setBit(int r, int c); 
   /* make the bit in row r and column c have the value 1 without changing the 
    * values of any other bits. */
   void clearBit(int r, int c); 
   /* make the bit in row r and column c have the value 0 without changing the
      values of any other bits. */
   void setAll();
   /* make all bits in the array have the value 1.*/

   void clearAll();
   /* make all bits in the array have the value 0. */
   int checkBit(int r, int c);
   /* return the value (0 or 1) of the bit in row r and column c without 
      changing the values of any bits (including those being tested). */
   void print();
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */

   unweightedDirected(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedDirected() {
      if (matrix) {delete matrix;}
   }
};

class unweightedUndirected: public unweightedGraph
{
private:
   bitarray *matrix;
   bool main_diagonal;
   int size;
   
   unsigned int t(int row)
   /* Return the "triangle" number for a given row. */
   {
      return row*(row-1)/2;
   }
   
public:
   void create (int sz);
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */
   void setBit(int r, int c); 
   /* make the bit in row r and column c have the value 1 without changing the 
    * values of any other bits. */
   void clearBit(int r, int c); 
   /* make the bit in row r and column c have the value 0 without changing the
      values of any other bits. */
   void setAll();
   /* make all bits in the array have the value 1.*/

   void clearAll();
   /* make all bits in the array have the value 0. */
   int checkBit(int r, int c);
   /* return the value (0 or 1) of the bit in row r and column c without 
      changing the values of any bits (including those being tested). */
   void print();
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */

   unweightedUndirected(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedUndirected() {
      if (matrix) {delete matrix;}
   }
};

#endif

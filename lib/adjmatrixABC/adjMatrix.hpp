/******************************************************************************
 * adjMatrix (binary) header file
 * CREATED BY: Shaun Meyer
 * CREATED   : 11 Feb 2011
 ******************************************************************************/
#ifndef __ADJMATRIX__BINARY__HPP
#define __ADJMATRIX__BINARY__HPP

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "bitarray.hpp"

/*
 * Abstract base class for unweighted graphs (they both must
 * have these functions and this will make some usage cleaner.
 * by allowing us to create a pointer to the base class which
 * can then be pointed at either UNdirected or Directed graphs.
 */

class unweightedGraph
{
public:
   virtual void create (int sz)        = 0;
   virtual void setBit(int r, int c)   = 0; 
   virtual void clearBit(int r, int c) = 0; 
   virtual void setAll()               = 0;
   virtual void clearAll()             = 0;
   virtual int  checkBit(int r, int c) = 0;
   virtual void print()                = 0;
   virtual unsigned int size()         = 0;
};

class unweightedDirectedLST : public unweightedGraph
{
public:
   struct Item {
      Item *next;
      int value;
   };
private:
   Item **matrix;
   int matrix_size; // size of one dimension
public:

   void create (int sz);
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */

   void setBit(int r, int c); 
   /* Add the neighbor c to node r */

   void clearBit(int r, int c); 
   /* Remove the neighbor c from node r */ 

   void setAll();
   /* make all bits in the array have the value 1.*/

   void clearAll();
   /* make all bits in the array have the value 0. */
   int checkBit(int r, int c);
   /* return true if node r has neighbor c */

   void print();
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */

   unsigned int size();

   unweightedDirectedLST(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedDirectedLST() {
      if (matrix) {delete matrix;}
   }
};

class unweightedDirected : public unweightedGraph
{
private:
   bitarray *matrix;
   int matrix_size; // size of one dimension
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

   unsigned int size();

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
   int matrix_size;
   
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

   unsigned int size();

   unweightedUndirected(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedUndirected() {
      if (matrix) {delete matrix;}
   }
};

class weightedDirected
{
private:
   int *matrix, 
      matrix_size;
  
public:
   int size() {return matrix_size;}
   void free_matrix();
   void create_matrix(int size);
   void resize_matrix(int new_size);
   int & edge(int vertex_x, int vertex_y);

   weightedDirected() { matrix=NULL; create_matrix(10); }
   weightedDirected(int size) {matrix=NULL; create_matrix(size);}
   ~weightedDirected() { free_matrix(); }

};


#endif

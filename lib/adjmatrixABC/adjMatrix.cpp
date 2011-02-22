/******************************************************************************
 * adjMatrix.cpp - unweighted Directed / Undirected graph matrix class
 * CREATED BY: Shaun Meyer 
 * CREATED   : 11 Feb 2011
 ******************************************************************************/

#include "adjMatrix.hpp"

/****************************************************************
 * Begin unweightedDirected adjacency list member functions:
 ***************************************************************/

void unweightedDirectedLST::create (int sz)
/* if the pointer is not NULL, it first gracefully deallocates the 
   current array so that there are no memory leaks.  It then allocates 
   enough memory for an array of sz rows and columns. (Note that for 
   the triangular array only the triangle below the main diagonal needs 
   to be allocated.)  If sz < 2 set it to 2. */
{
   matrix_size = sz;
   if (matrix)
      delete [] matrix;

   /* Allocate a new array-of-pointers of size `sz` to matrix */
   matrix = new unweightedDirectedLST::Item*[sz];

   /* Just to be on the safe size, initialize matrix to NULL... */
   clearAll();
   
}

void unweightedDirectedLST::setBit(int r, int c)
/* Set `c` as neighbor to node `r`. */
{
   if (!checkBit(r, c) && r < matrix_size && c < matrix_size)
      {
      unweightedDirectedLST::Item *new_item = new unweightedDirectedLST::Item;
      new_item->value = c;
      new_item->next = matrix[r];
      matrix[r] = new_item;
      }
}

void unweightedDirectedLST::clearBit(int r, int c)
/* Remove the neighbor c from node r */ 
{
   if (!checkBit(r, c)) 
      return; // nothing to do.

   unweightedDirectedLST::Item *trail;
   if (matrix[r] && matrix[r]->value == c)
      {
      trail = matrix[r]->next;
      delete matrix[r];
      matrix[r] = trail;
      }
   else
      {
      for (trail = matrix[r]; 
	   trail && trail->next && trail->next->value != c; 
	   trail = trail->next)
	 ;
      
      if (trail && trail->next && trail->next->value == c)
	 {
	 unweightedDirectedLST::Item *del;
	 del = trail->next;
	 trail->next = trail->next->next;
	 delete del;
	 }

      }
   
}

void unweightedDirectedLST::setAll()
/* make all bits in the array have the value 1.*/
{
   for (int x = 0; x < matrix_size; x++)
      for (int y = 0; y < matrix_size; y++)
	 setBit(x, y);
}

void unweightedDirectedLST::clearAll()
/* make all bits in the array have the value 0. */
{
   for (int i = 0; i < matrix_size; i++)
      {
      if (matrix[i])
	 delete matrix[i];
      matrix[i] = NULL;
      }
}

int unweightedDirectedLST::checkBit(int r, int c)
/* return true if node r has neighbor c */
{
   unweightedDirectedLST::Item *cur;
   for (cur = matrix[r]; cur && cur->value == c; cur = cur->next)
      ;

   if (cur && cur->value == c)
      /* We found it. Move it to the front of the list and return true. */
      {
      unweightedDirectedLST::Item *mov;
      mov = 
      matrix[r] = 
      return true;
      }
   
   return false;
}

void unweightedDirectedLST::print()
/* should output the array one full row at a time of  0's and 1's.  Note 
   that both, including the triangular array, should be output as a sz x sz 
   array on the screen.  (It's okay if lines too long for the screen wrap.) */
{
   // I want to print the actual list structure for debugging here.
   for (int i = 0; i < matrix_size; i++)
      {
      unweightedDirectedLST::Item *node;

      std::cout << i << ": ";
      for (node = matrix[i]; node; node = node->next)
	 std::cout << "-> [ " << node->value << " ] ";
      std::cout << std::endl;
      }
}

unsigned int unweightedDirectedLST::size()
{
   return matrix_size;
}

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
   matrix_size = sz;

}

void unweightedDirected::setBit(int r, int c)
/* make the bit in row r and column c have the value 1 without changing the 
 * values of any other bits. */
{
   (*matrix)[r*matrix_size+c] = 1;
}

void unweightedDirected::clearBit(int r, int c)
/* make the bit in row r and column c have the value 0 without changing the
   values of any other bits. */
{
   (*matrix)[r*matrix_size+c] = 0;
}

void unweightedDirected::setAll()
/* make all bits in the array have the value 1.*/
{
   for (int i = 0; i < matrix_size*matrix_size; i++)
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
   return (*matrix)[r*matrix_size+c];
}

void unweightedDirected::print()
/* should output the array one full row at a time of  0's and 1's.  Note 
   that both, including the triangular array, should be output as a sz x sz 
   array on the screen.  (It's okay if lines too long for the screen wrap.) */
{
   for (int row = 0; row < matrix_size; row++)
      {
      for (int col = 0; col < matrix_size; col++)
	 std::cout << (*matrix)[row*matrix_size+col] << " ";
      std::cout << std::endl;
      }
}

unsigned int unweightedDirected::size()
{
   return matrix_size;
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
   matrix_size = sz;

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
   for (int i = 0; i < t(matrix_size); i++)
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
   for (int row = 0; row < matrix_size; row++)
      {
      for (int col = 0; col < matrix_size; col++)
	 std::cout << checkBit(row, col) << " ";
      std::cout << std::endl;
      }
}

unsigned int unweightedUndirected::size()
{
   return matrix_size;
}

/***************************************************************
 * weightedDirected definitions.
 **************************************************************/

void weightedDirected::free_matrix()
/* Free the memory in use by the matrix member. */
{
   if (matrix) 
      {
      delete [] matrix;
      matrix_size = 0;
      matrix = NULL;
      }
}

void weightedDirected::create_matrix(int size)
/* Dynamically allocate the requisite memory for a matrix of `size`. */
{
   /* for now: if matrix already exists,
    * whack it and create the requested
    * matrix (empty) */
   if (matrix)
      free_matrix();
   
   /* Cool, now make the new matrix and
    * set the size */
   matrix = new int[size*size];
   matrix_size = size;
}

void weightedDirected::resize_matrix(int new_size)
/* Resize the matrix to the new size and copy over all data. 
 * Currently only works with enlarging. */
{ 
   /* remember old matrix */
   int * old_matrix = matrix;
   int old_size = matrix_size;
   
   /* make new matrix */
   matrix = NULL; // Don't delete []! We're still using that data.
   create_matrix(new_size);

   /* Determine our bounds */
   int boundary;

   if (new_size == old_size)
      return;
   else if (new_size > old_size)
      boundary = old_size;
   else if (new_size < old_size)
      boundary = new_size;

     
   /* Copy all the datas over */
   for (int x = 0; x < boundary; x++)
      for (int y = 0; y < boundary; y++)
	 edge(x, y) = old_matrix[old_size*x + y];

   /* Finally, free the old data */
   delete [] old_matrix;
   
   return;
}

int& weightedDirected :: edge(int vertex_x, int vertex_y)
/* Get and Set a given edge between two nodes. */
{
   if (vertex_x > matrix_size || vertex_x < 0)
      throw std::out_of_range("vertex_x");
   else if (vertex_y > matrix_size || vertex_y < 0)
      throw std::out_of_range("vertex_y");

   return matrix[matrix_size*vertex_x + vertex_y];
}

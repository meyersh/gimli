/*******************************************************************************
 * adjmatrix.cpp -
 * Created for CSCI 310 
 * SUBMITTED BY: Shaun Meyer
 *   CREATED BY: Shaun Meyer
 *      CREATED: 16 Feb, 2011
 * 
 * DESCRIPTION:
 * A basic class to maintain an adjacency matrix in memory.
 *******************************************************************************/
#include <stdexcept>
#include "adjmatrix.hpp"

void adjMatrix::free_matrix()
/* Free the memory in use by the matrix member. */
{
   if (matrix) 
      {
      delete [] matrix;
      matrix_size = 0;
      matrix = NULL;
      }
}

void adjMatrix::create_matrix(int size)
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

void adjMatrix::resize_matrix(int new_size)
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

int& adjMatrix :: edge(int vertex_x, int vertex_y)
/* Get and Set a given edge between two nodes. */
{
   if (vertex_x > matrix_size || vertex_x < 0)
      throw std::out_of_range("vertex_x");
   else if (vertex_y > matrix_size || vertex_y < 0)
      throw std::out_of_range("vertex_y");

   return matrix[matrix_size*vertex_x + vertex_y];
}

#include "adjMatrix.hpp"

void adjMatrix::free_matrix()
{
  if (matrix) 
    {
      delete [] matrix;
      matrix_size = 0;
      matrix = NULL;
    }
}

void adjMatrix::create_matrix(int size)
{
  /* 
   * for now: if matrix already exists,
   * whack it and create the requested
   * matrix (empty)
   */
  if (matrix)
    free_matrix();

  /* 
   * Cool, now make the new matrix and
   * set the size
   */
  matrix = new int[size*size];
  matrix_size = size;
}

void adjMatrix::resize_matrix(int new_size)
{ 
  /* remember old matrix */
  int * old_matrix = matrix;
  int old_size = matrix_size;

  /* make new matrix */
  matrix = NULL; // Don't delete []! We're still using that data.
  create_matrix(new_size);

  /* Copy all the datas over */
  for (int x = 0; x < old_size; x++)
    for (int y = 0; y < old_size; y++)
	edge(x, y) = old_matrix[old_size*x + y];
    
  /* Finally, free the old data */
  delete [] old_matrix;

  return;
}

int& adjMatrix :: edge(int vertex_x, int vertex_y)
{
  return matrix[matrix_size*vertex_x + vertex_y];
}

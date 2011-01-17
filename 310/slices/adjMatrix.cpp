#include "adjMatrix.hpp"

void adjMatrix::free_matrix()
{
  if (matrix) 
    {
      delete [] matrix;
      size = 0;
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
  matrix = new int[size*size];
  this->size = size;
}

void adjMatrix::resize_matrix(int new_size)
{ 
  return;
}

int & edge(int vertex_x, int vertex_y)
{
  return matrix[size*vertex_x + vertex_y];
}

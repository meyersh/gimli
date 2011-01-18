#ifndef __ADJMATRIX_HPP__
#define __ADJMATRIX_HPP__

#include <cstdlib>

class adjMatrix 
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

  adjMatrix() { matrix=NULL; create_matrix(10); }
  adjMatrix(int size) {matrix=NULL; create_matrix(size);}
  ~adjMatrix() { free_matrix(); }

};

#endif

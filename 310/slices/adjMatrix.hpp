#ifndef __ADJMATRIX_HPP__

class adjMatrix 
{
private:
  int *matrix, 
    size = 0;
  
public:
  void free_matrix();
  void create_matrix(int size);
  void resize_matrix(int new_size);
  int & edge(int vertex_x, int vertex_y);

  adjMatrix() { create_matrix(10); }
  ~adjMatrix() { free_matrix(); }

};

#endif

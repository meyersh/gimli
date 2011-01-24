#ifndef __TRIARY_HPP__
#define __TRIARY_HPP__

#include <cstdlib>
#include <stdexcept>

template<class V>
class triary
{
private:
  V *triangle_array;
  int num_rows;
  
public:

  int rows() {return num_rows;}
  int t(int row)  {return row*(row+1)/2;}
  V& access(int row, int col);

  void resize(int rows);
  void free_array() {delete [] triangle_array; triangle_array=NULL}
  void create_array(int size) 
  {
    triangle_array = new V[t(size)];
  }

  triary() {num_rows = 0; triangle_array=NULL;}
  triary(int rows) {num_rows = rows; create_array(rows);}
  ~triary() {free_array();}

};

#endif

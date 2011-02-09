#include "bitarray.hpp"

std::ostream& operator<<(std::ostream& output, const bitarray::proxy& p) 
/* This is used to allow us to output bitarray::proxy objects to cout
 * for instance: cout << ba[i]... */
{
   output << p.parent->showBit(p.index);
   return output;  // for multiple << operators.
}

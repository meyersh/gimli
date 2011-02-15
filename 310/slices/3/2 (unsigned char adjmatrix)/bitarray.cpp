/******************************************************************************
 * This function must, unfortunately, be in a cpp which gets made into an 
 * object file and then LINKED into whatever program will use it. Just an 
 * example of c++ warts. If it is placed into the header file, it will be 
 * re-processed multiple times (gross) and result in compile-time errors.
 *
 * CREATED BY: Shaun Meyer
 * CREATED   : 11 Feb 2011
 *****************************************************************************/
#include "bitarray.hpp"

std::ostream& operator<<(std::ostream& output, const bitarray::proxy& p) 
/* This is used to allow us to output bitarray::proxy objects to cout
 * for instance: cout << ba[i]... */
{
   output << p.parent->showBit(p.index);
   return output;  
}


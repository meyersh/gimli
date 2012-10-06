/* Constant time char-set.
   Algorithm: http://research.swtch.com/sparse
   Code: Shaun Meyer, Feb 2012
*/

#ifndef __SET_HPP__
#define __SET_HPP__

#include <sstream>

struct char_set {
   char dense[255];
   char sparse[255];
   unsigned int size;

   char_set() {
	  size = 0;
   }

   bool contains(char d) {
	  /* Check and see if a given char is in our set */
	  return sparse[d] < size && dense[sparse[d]] == d;
   }
   

   void insert(char d) {
	  /* insert `d` into the set. */
	  sparse[d] = size++;
	  dense[sparse[d]] = d;
   }

   void clear() {
	  size = 0;
   }

   std::string string() {
	  /* Return the string representation of the set */
	  std::stringstream ss;
	  for (int i = 0; i < size; i++)
		 ss << dense[i];

	  return ss.str();
   }

   int length() {
	  return size;
   }

};

#endif

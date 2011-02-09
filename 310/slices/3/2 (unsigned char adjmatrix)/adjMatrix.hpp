#ifndef __ADJMATRIX__BINARY__HPP
#define __ADJMATRIX__BINARY_HPP

#include <iostream>

#include <boost/shared_array.hpp>

class bitarray
{
private:
   boost::shared_array<unsigned char> ary;
   unsigned int word_size, // number of bits in our base unit
      num_words; // number of words in ary

public:
   class proxy 
   {  friend std::ostream& operator<<(std::ostream &output, const proxy &p);

   private:
      bitarray *parent;
      unsigned int index;
   public:
      void set_index(int i) {index = i;}
      proxy(bitarray &parent, int idx)
      {
	 this->parent = &parent;
	 this->index = idx;
      }

      proxy &operator=(bool inpt)
      {
	 unsigned char mask = 0x80; // 1000 0000
	 mask >>= index%parent->word_size;

	 if (inpt == 1)
	    parent->ary[index/parent->word_size] |= (mask);

	 else if (inpt == 0)
	    parent->ary[index/parent->word_size] &= (~mask);
      
	 return *this;
      }

      operator bool() {
	 return this->parent->showBit(this->index);
      }
    
   };

   /*
    * END OF PROXY DEFINITION 
    */

   friend class proxy;

private:
   proxy *bp;

public:
   bitarray() 
   {
      bp = new proxy(*this, 0);
      num_words = 0;
      word_size = sizeof(char)*8;
   }

   bitarray(int sz) 
   {
      word_size = sizeof(char)*8;
      num_words=sz/8 + sz%word_size;
      ary.reset(new unsigned char[num_words]);
      bp = new proxy(*this, 0);
   }

   ~bitarray() {
      delete bp;
   }

   bool const operator[](int index) const
   /* It doesn't get any conster than this. */
   {
      unsigned char mask = 0x80; // 1000 0000
      mask >>= index % word_size;
    
      return (ary[index/word_size] & mask) == 1;
   }

   proxy& operator[](int index)
   {
      bp->set_index(index);
      return *bp;
   }

   const bool showBit(const int index) const
   /* Show off a given bit... */
   {
      unsigned char mask = 0x80; // 1000 0000
      mask >>= index % word_size;
    
      return (ary[index/word_size] & mask) != 0;
   }

   void clear()
   /* Set all of the bitarray to nil */
   {
      for (int i = 0; i < num_words; i++)
	 ary[i] = 0;
   }

};

/*
 * Abstract base class for unweighted graphs (they both have to
 * have these functions and this will make some usage cleaner.
 */

class unweightedGraph
{
public:
   virtual void create (int sz) = 0;
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */
   virtual void setBit(int r, int c) = 0; 
   /* make the bit in row r and column c have the value 1 without changing the 
    * values of any other bits. */
   virtual void clearBit(int r, int c) = 0; 
   /* make the bit in row r and column c have the value 0 without changing the
      values of any other bits. */
   virtual void setAll() = 0;
   /* make all bits in the array have the value 1.*/
   virtual void clearAll() = 0;
   /* make all bits in the array have the value 0. */
   virtual int checkBit(int r, int c) = 0;
   /* return the value (0 or 1) of the bit in row r and column c without 
      changing the values of any bits (including those being tested). */
   virtual void print() = 0;
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */
};


class unweightedDirected : public unweightedGraph
{
private:
   bitarray *matrix;
   int size; // size of one dimension
public:
   void create (int sz);
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */
   void setBit(int r, int c); 
   /* make the bit in row r and column c have the value 1 without changing the 
    * values of any other bits. */
   void clearBit(int r, int c); 
   /* make the bit in row r and column c have the value 0 without changing the
      values of any other bits. */
   void setAll();
   /* make all bits in the array have the value 1.*/

   void clearAll();
   /* make all bits in the array have the value 0. */
   int checkBit(int r, int c);
   /* return the value (0 or 1) of the bit in row r and column c without 
      changing the values of any bits (including those being tested). */
   void print();
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */

   unweightedDirected(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedDirected() {
      if (matrix) {delete matrix;}
   }
};

class unweightedUndirected: public unweightedGraph
{
private:
   bitarray *matrix;
   bool main_diagonal;
   int size;
   
   unsigned int t(int row)
   /* Return the "triangle" number for a given row. */
   {
      return row*(row-1)/2;
   }
   
public:
   void create (int sz);
   /* if the pointer is not NULL, it first gracefully deallocates the 
      current array so that there are no memory leaks.  It then allocates 
      enough memory for an array of sz rows and columns. (Note that for 
      the triangular array only the triangle below the main diagonal needs 
      to be allocated.)  If sz < 2 set it to 2. */
   void setBit(int r, int c); 
   /* make the bit in row r and column c have the value 1 without changing the 
    * values of any other bits. */
   void clearBit(int r, int c); 
   /* make the bit in row r and column c have the value 0 without changing the
      values of any other bits. */
   void setAll();
   /* make all bits in the array have the value 1.*/

   void clearAll();
   /* make all bits in the array have the value 0. */
   int checkBit(int r, int c);
   /* return the value (0 or 1) of the bit in row r and column c without 
      changing the values of any bits (including those being tested). */
   void print();
   /* should output the array one full row at a time of  0's and 1's.  Note 
      that both, including the triangular array, should be output as a sz x sz 
      array on the screen.  (It's okay if lines too long for the screen wrap.) */

   unweightedUndirected(int sz) {
      matrix=NULL;
      create(sz);
   }
   ~unweightedUndirected() {
      if (matrix) {delete matrix;}
   }
};

#endif

#ifndef __BITARRAY_HPP__
#define __BITARRAY_HPP__ 

#include <boost/shared_array.hpp>
#include <iostream>

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
      bitarray     *parent;
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

#endif

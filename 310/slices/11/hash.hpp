/******************************************************************************
 * hash.hpp - Shauns Hash Table implementation
 *
 * SUBMITTED BY: Shaun Meyer
 * CREATED BY: Shaun Meyer
 * CREATED: 10 APR 2011
 * 
 * DESCRIPTION:
 * Declares + Defines a hashTable template class. Implemented using th ELF
 * Hashing algorithm.
 *
 * This class shares, surprisingly, an identical interface with the 
 * ctt class, making it suitable as a drop-in replacement.
 * 
 *****************************************************************************/

#ifndef __HASH__HPP__
#define __HASH__HPP__

#include <string>
#include <vector>

using namespace std;


template<class V>
class hashTable {

   /* ------------------------------------------------------------------
    * Sub-class: Node, each "key" in the hashTable is represented by one
    * of these. */
   struct Node {
      string key;
      Node *next, *prev;
      V index;
      Node(const string &key="", const V &index=0)
      {
	 this->key = key;
	 this->index = index;
	 next = NULL;
	 prev = NULL;
      }
   };
   /* ------------------------------------------------------------------ */
private:
   vector< Node* > buckets; /* hash buckets, each begins a linked-list of
			       nodes (for collision resolution). */

public:
   hashTable(int size=16)
   {
      buckets.resize(size);
      return;
   }

   unsigned hash(const string &key, unsigned int mask=~0)
   /* PARAMS: string `key' and unsigned int mask.
      RETURN: unsigned int representing the hash value of `key'
      DESCRI: Implements the ELF hash algorithm. Uses mask to
      reduce the size of the returned integer by bitwise ANDing
      the result of the hash function with the mask. */
    */ 
   {
      string::const_iterator p; 
      unsigned h = 0, g;

      for (p = key.begin(); p != key.end(); p++)
	 {
	 h = ( h << 4 ) + *p;
	 g = h & 0xf0000000L;

	 if ( g != 0 )
	    h ^= g >> 24;

	 h &= ~g;
	 }

      return h & mask;
   }

   unsigned int mask()
   /* PARAMS: None
      RETURN: buckets.size() - 1;
      DESCRI: Default mask is the working-set size of the buckets 
      vector. */
   {
      return buckets.size() -1 ;
   }

   // insert a key
   void insert(const string &key, const V &index); 

   // delete a key
   void deleteKey(const string &key); 

   // return the index of a key
   int getIndex(const string &key, V &index); // Return 0 on success, NZ if fail

   // return a vector<string> of all keys.
   vector<string> keys();

};

template<class V>
void hashTable<V>::insert(const string &key, const V &index)
/* PARAMS: `key' and `index`
   RETURN: void
   DESCRI: Inserts a given `key' with `index' into the hashTable. */
{
   /* deduce the bucket element for a given key based on its hash: */
   unsigned int bucket_location = hash(key, mask());

   /* Check the bucket_location for sanity */
   if (bucket_location > buckets.size()-1)
      {
      cout << "Key_out_of_bounds: " << key << " hash: " 
	   << bucket_location << " : " << index << endl;
      return;
      }

   /* If this is the first element in that location, just
      create one element */
   if (buckets[bucket_location] == NULL)
      {
      buckets[bucket_location] = new Node(key, index);
      }
   else /* For subsequent elements, insert into the FRONT of the list */
      {
      buckets[bucket_location]->prev = new Node(key, index);
      buckets[bucket_location]->prev->next = buckets[bucket_location];
      buckets[bucket_location] = buckets[bucket_location]->prev;
      }
   
}

template<class V>
void hashTable<V>::deleteKey(const string &key)
/* PARAMS: `key'
   RETURN: void
   DESCRI: Delete a given key from the hashTable (if it exists). 
   Do nothing, otherwise. */
{
   /* deduce the bucket element for a given key based on its hash: */
   int bucket_location = hash(key, mask());

   /* For a given bucket, that is not null, evaluate each item in the 
      linked list. When the item is found, remove it. */
   for (Node *b = buckets[bucket_location]; 
	b != NULL; 
	b = b->next)
      {
      if (b->key == key) /* found it! */
	 {
	 if (buckets[bucket_location] == b) /* it's the first element */
	    if (b->next) /* there are other elements following... */
	       buckets[bucket_location] = b->next;
	    else /* there no other elements */
	       buckets[bucket_location] = NULL;

	 /* Re-attach the surrounding Nodes */
	 if (b->next)
	    b->next->prev = b->prev;
	 if (b->prev)
	    b->prev->next = b->next;
	 
	 delete b;
	 return;
	 }
      }
}

template<class V>
int hashTable<V>::getIndex(const string &key, V &index)
/* PARAMS: `key' and `index'
   RETURN: Zero on success. (`index' returned by reference)
   DESCRI: Lookup a given index in the hashTable. */
{
   /* deduce the bucket element for a given key based on its hash: */
   unsigned int bucket_location = hash(key, mask());

   /* Check the bucket_location for sanity */
   if (bucket_location > buckets.size()-1)
      {
      cout << "Key_out_of_bounds: " << key << " hash: " 
	   << bucket_location << " : " << index << endl;
      return -1; // insane hash.
      }
   
   if (buckets[bucket_location] == NULL)
      return -2; // empty bucket.

   /* For each Node *k, if k == key, return. */
   for (Node* k = buckets[bucket_location]; k; k = k->next)
      {
      if (k->key == key)
	 {
	 index = k->index;
	 return 0; // success
	 }
      
      }

   return -3; // not found
}

template<class V>
vector<string> hashTable<V>::keys()
/* PARAMS: None.
   RETURN: vector<string> of all keys in hashTable.
   DESCRI: Return all keys in the hashTable. */
{
   Node *n = NULL; // search node ptr
   vector<string> ret; // return vector

   /* For each bucket... */
   for (int i = 0; i < buckets.size(); i++)
      {
      /* skip empty buckets... */
      if (buckets[i] == NULL)
	 continue;
      
      /* for each non-empty bucket, peruse all nodes in list */
      for (n = buckets[i]; n; n = n->next)
	 {
	 ret.push_back(n->key);
	 }
      }
   return ret;
}

#endif

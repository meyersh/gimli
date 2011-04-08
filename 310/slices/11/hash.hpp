#ifndef __HASH__HPP__
#define __HASH__HPP__

#include <string>
#include <vector>

using namespace std;


template<class V>
class hashTable {
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

private:
   vector< Node* > buckets;

public:
   hashTable(int size=16)
   {
      buckets.resize(size);
      return;
   }

   unsigned hash(const string &key, unsigned int mask=~0)
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

      return h&mask;
   }

   unsigned int mask()
   {
      return buckets.size() -1 ;
   }

   void insert(const string &key, const V &index); 

   void deleteKey(const string &key); 

   int getIndex(const string &key, V &index); // Return 0 on success, NZ if fail

   vector<string> keys();

};

template<class V>
void hashTable<V>::insert(const string &key, const V &index)
{
   unsigned int bucket_location = hash(key, mask());
   if (bucket_location > buckets.size()-1)
      {
      cout << "Key_out_of_bounds: " << key << " hash: " 
	   << bucket_location << " : " << index << endl;
      return;
      }
   if (buckets[bucket_location] == NULL)
      {
      buckets[bucket_location] = new Node(key, index);
      }
   else
      {
      buckets[bucket_location]->prev = new Node(key, index);
      buckets[bucket_location]->prev->next = buckets[bucket_location];
      buckets[bucket_location] = buckets[bucket_location]->prev;
      }
   
}

template<class V>
void hashTable<V>::deleteKey(const string &key)
{
   int bucket_location = hash(key, mask());
   for (Node *b = buckets[bucket_location]; 
	b != NULL; 
	b = b->next)
      {
      if (b->key == key)
	 {
	 if (buckets[bucket_location] == b)
	    if (b->next)
	       buckets[bucket_location] = b->next;
	    else
	       buckets[bucket_location] = NULL;

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
{
   unsigned int bucket_location = hash(key, mask());
   if (bucket_location > buckets.size()-1)
      {
      cout << "Key_out_of_bounds: " << key << " hash: " 
	   << bucket_location << " : " << index << endl;
      return -1;
      }
   
   if (buckets[bucket_location] == NULL)
      return -2; // nothing there.

   for (Node* k = buckets[bucket_location]; k; k = k->next)
      {
      if (k->key == key)
	 {
	 index = k->index;
	 return 0;
	 }
      
      }
}

template<class V>
vector<string> hashTable<V>::keys()
{
   Node *n = NULL;
   vector<string> ret;
   for (int i = 0; i < buckets.size(); i++)
      {
      if (buckets[i] == NULL)
	 continue;
      for (n = buckets[i]; n; n = n->next)
	 {
	 ret.push_back(n->key);
	 }
      }
   return ret;
}



#endif

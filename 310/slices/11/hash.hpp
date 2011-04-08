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
   if (hash(key, mask()) > buckets.size()-1)
      {
      cout << "Key_out_of_bounds: " << key << " hash: " 
	   << hash(key, mask()) << " : " << index << endl;
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
}

template<class V>
int getIndex(const string &key, V &index)
{
}


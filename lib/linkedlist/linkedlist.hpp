#ifndef __LINKEDLIST__HPP__
#define __LINKEDLIST__HPP__

/*
 * Wonderful linked list class.
 *
 */

/*
 * LinkedList holds these:
 */

template<classV>
struct LinkedListItem<V> {
   V value;
   LinkedListItem* next;
};


template<class V>
class LinkedList {
private:
   LinkedListItem<V> *list;
   unsigned int size;
public:
   unsigned int size() {return size;}

   bool has(V needle)
   {
      if (list && list->value == needle)
	 return true;
      else 
	 {
	 LinkedListItem<V> *c;
	 for (c = list->next; c && c->value != needle; c = c->next)
	    ; /* pass */
	 if (c && c->value == needle)
	    return true
	 }
      return false;
   }

   void remove(V needle)
   {
      LinkedListItem<V> *del, t;
      if (list && list->value == needle)
	 {
	 del = list;
	 list = list->next;
	 delete del;
	 }
      else 
	 {
	 for (t = list; t && t->next && t->next->value != needle; t = t->next)
	    ;
	 if (t && t->next && t->next->value == needle)
	    {
	    del = t->next;
	    t->next = del->next;
	    delete del;
	    }
	 }
   }

   void insert_last(V new_value)
   {
      LinkedListItem<V> *new_item = new LinkedListItem<V>;
      new_item->value = new_value;
      new_item->next = NULL;
      
      if (!list)
	 list = new_item;
      
      else
	 {
	 LinkedListItem<V> *c;
	 for (c = list; c->next; c = c->next)
	    ;
	 c->next = new_item;
	 }
   }

   void insert_first(V new_value)
   {
      LinkedListItem<V> *new_item = new LinkedListItem<V>;
      new_item->next = list;
      new_item->value = new_value;
      list=new_item;
   }

   void remove_first()
   {
      LinkedListItem<V> *old_list = new LinkedListItem<V>;
      old_list = list;
      list = list->next;
      delete old_list;
   }

   void remove_last()
   {
      LinkedListItem<V> *c;
      for (c = list; c && c->next && c->next->next; c = c->next)
	 ;
      delete c->next;
      c->next = NULL;
   }

};

#endif

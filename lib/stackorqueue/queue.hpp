/*******************************************************************************
 * queue container (renders a linked-list queue)
 * Shaun Meyer, Jan 2011
 ******************************************************************************/

#include "boost/shared_ptr.hpp"

/*
 * Each item in the queue looks like this:
 */

template<class V>
struct queueItem {
   V value;
   boost::shared_ptr< queueItem<V> > next;
};

/* 
 * Meanwhile, a queue contains items (queueItems, ironically)
 */

template<class V>
class queue {
public:
   typedef boost::shared_ptr< queueItem<V> > queue_ptr;

private:

  queue_ptr  front_of_queue,
             back_of_queue;

  int        queue_size;
   
public:

   queue() { queue_size = 0;}
   unsigned int size() {return queue_size;}

   /*
    * push()
    */
   void push(V item)
   /* Push an item onto the queue. */
   {
      queue_ptr new_item( new queueItem<V> );
      new_item->value = item;

      /* Skip this if we're the only item in queue. */
      if (back_of_queue)
     	back_of_queue->next = new_item; 

      /* If this is the first item in queue, it's the front */
      if (!front_of_queue)
	front_of_queue = new_item;

      back_of_queue = new_item; // new item is dead last now.
      queue_size++;
   }

   queue_ptr popptr()
   /* Pop an item off the front of the queue and return the queue_ptr. */
   {
      queue_ptr old_tos = front_of_queue; // old front-of-queue.
      front_of_queue = front_of_queue->next;
      queue_size--;
      return old_tos;
   }

   V pop()
   /* Pop an item off the front of the queue and return the value. */
   {
      return popptr()->value;
   }
   

  /* 
   * front()
   */
  V &front()
  /* Return the value of the front_of_stack without removing it. */
  {
    return front_of_queue->value;
  }
   /* 
    * nth()
    */
   V &nth(unsigned int item_index)
   /* Return a reference to the nth item of the queue */
   {
      return front_of_queue->value;
   }
};


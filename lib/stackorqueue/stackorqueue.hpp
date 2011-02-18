#ifndef __STACKORQUEUE__HPP__
#define __STACKORQUEUE__HPP__


/*******************************************************************************
 * stackorqueue container class (renders a linked-list stack)
 * Shaun Meyer, Jan 2011
 ******************************************************************************/

#include "boost/shared_ptr.hpp"

/*
 * Each item in the stack looks like this:
 */

template<class V>
struct stackorqueueItem {
   V value;
   boost::shared_ptr< stackorqueueItem<V> > next;
};

/*
 * abstract base class which both queue+stack draw from
 * which allows us to spawn one container pointer/reference
 * of stackorqueue type which can be filled with a stack (or
 * a queue) and act like that kind of thing.
 */

template<class V>
class stackorqueue {
public:
   virtual void         push(V) = 0;
   virtual V             pop()  = 0;
   virtual V            &top()  = 0;
   virtual unsigned int size()  = 0;
};


/* 
 * Meanwhile, a stack contains items (stackItems, ironically)
 */

template<class V>
class stack : public stackorqueue<V>
{
public:
   typedef boost::shared_ptr< stackorqueueItem<V> > stack_ptr;

private:

   stack_ptr  top_of_stack;
   int        stack_size;
   
public:

   stack() { stack_size = 0;}
   unsigned int size() {return stack_size;}

   /*
    * push()
    */
   void push(V item)
   /* Push an item onto the stack. */
   {
      stack_ptr new_item( new stackorqueueItem<V> );
      new_item->value = item;
      new_item->next = top_of_stack;
      top_of_stack = new_item;
      stack_size++;
   }

   /* 
    * popptr()
    */
   stack_ptr popptr()
   /* Pop an item off the top of the stack and return its pointer. */
   {
      stack_ptr old_tos = top_of_stack; // old top-of-stack.
      top_of_stack = top_of_stack->next;
      stack_size--;
      return old_tos;
   }

   /*
    * pop()
    */
   V pop()
   /* Pop an item off the stack, returning its value */
   {
      return popptr()->value;
   }


  /*
   * top()
   */
  V &top()
  /* Return the top-of-stack value without popping it off. */
  {
    return top_of_stack->value;
  }
   
   /* 
    * nth()
    */
   V &nth(unsigned int item_index)
   /* Return a reference to the nth item of the stack */
   {
      return top_of_stack->value;
   }
};

/*******************************************************************************
 * queue container (renders a linked-list queue)
 * Shaun Meyer, Jan 2011
 ******************************************************************************/

/* 
 * Meanwhile, a queue contains items (queueItems, ironically)
 */

template<class V>
class queue : public stackorqueue<V>
{
public:
   typedef boost::shared_ptr< stackorqueueItem<V> > queue_ptr;

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
      queue_ptr new_item( new stackorqueueItem<V> );
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
    * top()
    */
   V &top()
   /* Return the value from the front_of_stack, without removing it. */
   {
      return front();
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

#endif

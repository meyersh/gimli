/*******************************************************************************
 * stack container (renders a linked-list stack)
 * Shaun Meyer, Jan 2011
 ******************************************************************************/

#include "boost/shared_ptr.hpp"

/*
 * Each item in the stack looks like this:
 */

template<class V>
struct stackItem {
   V value;
   boost::shared_ptr< stackItem<V> > next;
};

/* 
 * Meanwhile, a stack contains items (stackItems, ironically)
 */

template<class V>
class stack {
public:
   typedef boost::shared_ptr< stackItem<V> > stack_ptr;

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
      stack_ptr new_item( new stackItem<V> );
      new_item->value = item;
      new_item->next = top_of_stack;
      top_of_stack = new_item;
      stack_size++;
   }

   /* 
    * pop()
    */
   stack_ptr popptr()
   /* Pop an item off the top of the stack. */
   {
      stack_ptr old_tos = top_of_stack; // old top-of-stack.
      top_of_stack = top_of_stack->next;
      stack_size--;
      return old_tos;
   }

   V pop()
   /* Pop an item off the top of the stack. */
   {
      return popptr()->value;
   }
   
  /*
   * top()
   */
   stack_ptr &topptr()
   /* Return the top-of-stack value without popping it off. */
   {
      if (top_of_stack)
	 return top_of_stack;
   }
   const V top()
   {
      if (top_of_stack)
	 return top_of_stack->value;
      else 
	 return 0;
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


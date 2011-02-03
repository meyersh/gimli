#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

/*
 * linked-listed based stacks and queue's hold these:
 */
template<class V>
struct Item
{
   V                            value;

   boost::shared_ptr< Item<V> > next;
};

class stkQ_container
{
public:
  virtual void push(const std::string &);
  virtual void push(const int &);
};


template<class V>
class stackL : public stkQ_container
{
private:
   typedef boost::shared_ptr< Item<V> > item_ptr;

   item_ptr     top_of_stack;

   unsigned int size;
   
public:
   void push(const V &value)
   /* Push a new value onto the stack */
   {
      boost::shared_ptr<V> new_item(new Item<V>);
      new_item->value = value;

      if (top_of_stack)
	 new_item->next = top_of_stack; 

      top_of_stack = new_item;
      size++;
   }

   int pop(V &ret_value)
   /* Pop the top value, returning its pointer */
   {
      if (size)
	 {
	 boost::shared_ptr<V> old_top = top_of_stack;

	 if (top_of_stack->next)
	    top_of_stack = top_of_stack->next;
	 size--;
	 ret_value = old_top->value;
	 return false;
	 }
      return true;
   }

   int top(V &ret_value)
   /* Return the top value, leaving it on the stack. */
   {
      if (top_of_stack)
	 {
	 ret_value = top_of_stack->value;
	 return false;
	 }
      return true;
   }

   bool isEmpty() {return size==0;}

   void clear()
   {
      top_of_stack.reset(); /* this deserves some explaination. I'm using 
			       shared_ptr's here which do reference counting.
			       When the references are 0; they `delete` whatever
			       they're pointing at.

			       Thus, we've basically implemented "cascade" 
			       since when we remove the top_of_stack, nothing
			       references the next-> item, so it's deleted, 
			       and so on. Cool! */
      size = 0;
   };

   int count() {return size;}

   stackL()
   {
      size = 0;
   };
   ~stackL() {clear();}
};

template<class V>
class stackV : public stkQ_container 
{
private:
   std::vector<V> stack_vector;

public:
   void push(const V &value)
   {
      stack_vector.push_back(value);
   }
   int pop(V &ret_value)
   {
      if (stack_vector.size() == 0)
	 return true;

      ret_value = stack_vector[stack_vector.size()-1];
      stack_vector.pop_back();
      return false;
   }
   int top(V &ret_value)
   {
      if (stack_vector.size() == 0)
	 return true;

      ret_value = stack_vector[stack_vector.size()-1];
      return false;
   }

   bool isEmpty() {return stack_vector.size() == 0;}

   void clear()
   {
      stack_vector.clear();
   };

   int count() {return stack_vector.size();}
};

template<class V>
class queueL : public stkQ_container 
{
private:
   typedef boost::shared_ptr< Item<V> > item_ptr;

   item_ptr              back_of_queue;

   unsigned int          size;

public:
   void enqueue(const V &value)
   {
      item_ptr new_item(new Item<V>);
      new_item->value = value;
      
      if (back_of_queue)
	 {
	 new_item->next = back_of_queue;
	 back_of_queue = new_item;
	 }
      else /* Empty queue */
	 {
	 new_item->next = new_item;
	 back_of_queue = new_item;
	 }

      size++;
   };
   int dequeue(V &ret_value)
   {
      if (size)
	 {
	 ret_value = back_of_queue->next->value;

	 /* Some explanation is warranted here. We're simply closing up the ring
	    ( this is a circular linked-list ) and write the old element off.
	    Its reference counter drops to zero and it `delete`s itself. */

	 back_of_queue->next = back_of_queue->next->next; 

	 size--;
	 return false;
	 }
      
      return true;
	 
   }
   int front(V &ret_value)
   {
      if (back_of_queue)
	 {
	 ret_value = back_of_queue->next->value;
	 return false;
	 }
      return true;
   }

   bool isEmpty() {return size==0;}
   void clear() 
   {
      /* A shared_ptr is a wonderful thing. Destroying the only link to it
	 will cause it to delete itself, which in turn removes the only link
	 to the other shared_ptrs which delete themselves -- Cascade! */

      back_of_queue.reset();
   }
   int count() {return size;}

   queueL() {size=0;}
   ~queueL() {clear();}
};

template<class V>
class queueV : public stkQ_container 
{
private:
   std::vector<V> queue_vector;

public:
   void enqueue(const V &item)
   {
      queue_vector.insert(queue_vector.begin(), 
			  item);
   }
   int dequeue(V &ret_value)
   {
      if (queue_vector.size())
	 {
	 ret_value = queue_vector.back(); /* not intuitive, I know. We're 
					     using this vector backwards. */
	 queue_vector.erase(queue_vector.back());
	 return false;
	 }
      return true;
   }
   int front(V &ret_value)
   {
      if (queue_vector.size())
	 {
	 ret_value = queue_vector.back(); /* the back is the front when your 
					     vector is turned around! */
	 return false;
	 }
      return true;
   }
   bool isEmpty() {return queue_vector.size() == 0;}
   void clear() {queue_vector.clear();}
   int count() {return queue_vector.size();}
};

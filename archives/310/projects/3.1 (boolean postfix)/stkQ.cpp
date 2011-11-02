/******************************************************************************
 * stkQ.cpp 
 * created by Shaun Meyer (Feb 2011)
 * for CSCI-310
 * S3.E1
 *
 * Good:
 *   - Runtime Polymorphism
 *   - smart pointers
 * Bad:
 *   - A few rough edges on return type.
 * Ugly:
 *   - cout statements. 
 *****************************************************************************/

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <iostream> // for debugging 

/*
 * linked-listed based stacks and queue's hold these:
 */
template<class V>
struct Item
{
   V                            value;

   boost::shared_ptr< Item<V> > next;
};

/*
 * Abstract base class for both stack and queue containers
 * the only purpose for this is to enable run-time polymorphism
 * in the sqtest (in c++ this was like pulling teeth!)
 * 
 * All of these functions MUST be defined in the derived classes
 * or you'll get vtable errors from the linker.
 *
 * ptop() was a cop-out because I couldn't figure out how to get a
 * general polymorphic "response" container OUT of the C-style
 * pop functions [thanks, Randy!] when I am neck deep in C++ style
 * coding here.
 *
 * pop/dequeue push/enqueue top/front are basically synonyms here since 
 * you can't screw that up too badly and in the classes simply wrap 
 * around the correct function.
 */

class stkQ_container
{
public:
   typedef boost::shared_ptr<stkQ_container> stkQ_ptr;

   /* Lame, I know! but these couldn't (that I'm aware of) be templated. */
   virtual void push(const std::string &){};
   virtual void push(const int &){};
   virtual int top(const std::string &){};
   virtual int top(const int &){};
   virtual void ptop(){}; // print the top/front.
   virtual int pop(){}; // used in conjunction with ptop();

   virtual ~stkQ_container(){};
   virtual int count(){};
   
};

typedef boost::shared_ptr<stkQ_container> stkQ_ptr;


/*******************************************************************************
 * linked-list based stack.
 ******************************************************************************/

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
      boost::shared_ptr< Item<V> > new_item(new Item<V>);
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
	 item_ptr old_top = top_of_stack;


	 top_of_stack = top_of_stack->next; /* top_of_stack->next is empty, 
					       top_of_stack gets a NULL ptr. */
	 
	 size--;
	 ret_value = old_top->value;
	 return false;
	 }
      return true;
   }

   int pop()
   {
      V tmp;
      return pop(tmp);
   }

   int top(V &ret_value)
   /* Return the top value, leaving it on the stack. */
   {
      if (size)
	 {
	 ret_value = top_of_stack->value;
	 return false;
	 }
    
      return true;
   }

   void ptop()
   {
      if (top_of_stack)
	 std::cout << "TOP: '" << top_of_stack->value << "'\n";
      else
	 std::cout << "TOP: NULL\n";
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

/*******************************************************************************
 * vector-based stack
 ******************************************************************************/

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

   int pop()
   {
      V tmp;
      return pop(tmp);
   }

   int top(V &ret_value)
   {
      if (stack_vector.size() == 0)
	 return true;

      ret_value = stack_vector[stack_vector.size()-1];
      return false;
   }
   
   void ptop()
   {
      if (stack_vector.size())
	 std::cout << "TOP: '" << stack_vector[stack_vector.size()-1] << "'\n";
      else
	 std::cout << "TOP: NULL\n";
   }


   bool isEmpty() {return stack_vector.size() == 0;}

   void clear()
   {
      stack_vector.clear();
   };

   int count() {return stack_vector.size();}
};

/*******************************************************************************
 * link-list based queue
 ******************************************************************************/

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
	 new_item->next = back_of_queue->next;
	 back_of_queue->next = new_item;
	 back_of_queue = new_item;
	 }
      else /* Empty queue */
	 {
	 new_item->next = new_item;
	 back_of_queue  = new_item;
	 }

      size++;
   };

   void push(const V &value) {enqueue(value);}

   int dequeue(V &ret_value)
   {
      if (back_of_queue)
	 {
	 ret_value = back_of_queue->next->value;

	 /* Some explanation is warranted here. We're simply closing up the ring
	    ( this is a circular linked-list ) and write the old element off.
	    Its reference counter drops to zero and it `delete`s itself. */

	 /* Something very special happens when you point a pointer to itself,
	  * you get an endless loop!... */
	 if (back_of_queue->next == back_of_queue->next->next)
	    back_of_queue.reset();
	 else
	    back_of_queue->next = back_of_queue->next->next;

	 size--;
	 return false;
	 }
      
      return true;
   }

   int pop()
   {
      V tmp;
      return dequeue(tmp);
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

   void ptop()
   {
      if (back_of_queue)
	 std::cout << "FRONT: '" << back_of_queue->next->value << "'\n";
      else
	 std::cout << "FRONT: NULL\n";
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

/*******************************************************************************
 * vector-based queue
 ******************************************************************************/

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

   void push(const V &value) {enqueue(value);}

   int dequeue(V &ret_value)
   {
      if (queue_vector.size())
	 {
	 ret_value = queue_vector.back(); /* not intuitive, I know. We're 
					     using this vector backwards. */
	 queue_vector.erase(queue_vector.end());
	 return false;
	 }
      return true;
   }
   
   int pop()
   {
      V tmp;
      return dequeue(tmp);
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

   void ptop()
   {
      if (queue_vector.size())
	 std::cout << "FRONT: '" << queue_vector.back() << "'\n";
      else
	 std::cout << "FRONT: NULL\n";
   }

   bool isEmpty() {return queue_vector.size() == 0;}
   void clear() {queue_vector.clear();}
   int count() {return queue_vector.size();}
};

/* Templates I WILL be using: */

/* Not sure that I need these, could comment-out and recompile to see if 
 * I get errors. This basically just tells the compiler to make code for
 * these classes using these types.
 */
template class stackV<int>;
template class stackL<int>;
template class queueV<int>;
template class queueL<int>;
template class stackV<std::string>;
template class stackL<std::string>;
template class queueV<std::string>;
template class queueL<std::string>;

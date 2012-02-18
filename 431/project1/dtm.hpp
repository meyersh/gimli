/* 
 * DTM constructs:
 * - Tape structure
 * - Transition structure
 *
 * Shaun Meyer, Feb 2012
 */

#ifndef __DTM_HPP__
#define __DTM_HPP__

#include <string>

#define QY 1
#define QN 2

#define FORWARD true
#define BACKWARD false


/***
 * Tape structure, handles all tape operations by using a linked-list 
 * for tape squares. This allows any arbitrary moves prev & next.
 ***/

struct Tape {
   struct Cell {
	  Cell* next;
	  Cell* prev;
	  char data;
	  int index;

	  Cell(char d)
	  {
		 data = d;
		 next = prev = NULL;
	  }

   };

   Cell *beg;
   Cell *end;
   Cell *read_head;
   int read_head_pos;
   int tape_length;

   Tape() {
	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
   }

   Tape(std::string tape)
   {
	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
	  for (int i = 0; i < tape.length(); i++)
		 append(tape[i]);

	  read_head = beg;
   }

   int length() 
   {
	  return tape_length;
   }

   std::string toString()
   {
	  std::string ret;
	  for (Cell *i = beg; i; i = i->next)
		 ret += i->data;
	  return ret;
   }

   void append(char d=' ')
   {
	  if (end)
		 {
		 end->next = new Cell(d);
		 end->next->prev = end;
		 end = end->next;
		 end->index = end->prev->index + 1;
		 }
	  else
		 {
		 beg = end = new Cell(d);
		 end->index = 1;
		 }

	  tape_length++; 
	  
   }

   void clear() {
	  Cell *t;
	  for (t = beg; t; t = t->next)
		 delete t;

	  beg = end = read_head = NULL;
	  tape_length = read_head_pos = 0;
   }

   void set(std::string t)
   {
	  clear();
	  for (int i = 0; i < t.length(); i++)
		 append(t[i]);

	  rewind();
   }

   void prepend(char d=' ')
   {
	  if (beg)
		 {
		 beg->prev = new Cell(d);
		 beg->prev->next = beg;
		 beg = beg->prev;
		 beg->index = beg->next->index - 1;
		 }
	  else
		 {
		 beg = end = new Cell(d);
		 beg->index = beg->next->index - 1;
		 }
	  
	  tape_length++;
   }
   
   char read()
   {
	  if (read_head)
		 return read_head->data;
	  else
		 return ' ';
   }

   void write(char d=' ')
   {
	  if (read_head)
		 read_head->data = d;
   }

   int move(bool delta)
   {
	  if (delta == FORWARD)
		 {
		 if (read_head == NULL || !read_head->next)
			append();

		 read_head = read_head->next;
		 read_head_pos++;

		 }
	  else if (delta == BACKWARD)
		 {

		 if (read_head == NULL || !read_head->prev)
			prepend();

		 read_head = read_head->prev;
		 read_head_pos--;
		 }

	  return read_head_pos;
   }
   
   int rewind()
   {
	  read_head = beg;
	  return read_head_pos = 0;
   }
};

/***
 * transition structure - This holds the three pieces of a state for
 * easy access later. 
 **/

struct transition {
   int next_state; // 1 = QY, 2 = QN
   char write;
   bool delta; // TRUE == +1, FALSE == -1
   transition() {
	  write = ' ';
	  delta = true;
	  next_state = 0;
   }

   transition(int tonext, int towrite, bool todelta)
   {
	  next_state = tonext;
	  write = towrite;
	  delta = todelta;
   }
};

struct Stats {
   int pn, n, r, v;
}; // the universe of stats.

#endif

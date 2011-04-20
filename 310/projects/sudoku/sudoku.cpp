/******************************************************************************
CREATED BY: Shaun Meyer
CREATED: 16 APR 2011

DESCRIPTION:
Initial functions + class to interact with a sudoku board.

*****************************************************************************/

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <string.h> // for strcmp()

using namespace std;

//#define PLATINUM_BLONDE ".......12........3..23..4....18....5.6..7.8.......9.....85.....9...4.5..47...6..."
#define PALMS_PUZZLE "....3..51..36......2..948......5..7.59.....62.8..2......491..8......24..23..8...."

struct sudoku_table {
   int table[81]; /* the table (expressed as a 1d array. */
   unsigned int pencil_mark[81]; /* temp table for scribbling in pencil marks */

   unsigned int bit(int val);
   int get_subsquare_address(int subsquare_number);
   unsigned int check_row(int cell);
   unsigned int check_col(int cell);
   unsigned int check_subsquare(int cell);
   vector<int>  neighbors(int subsquare);
   int          sub_square(int cell);
   void         print_table();
   void         print_web_table();

   int zeros(unsigned int cell);

   int do_single_position();
   int do_single_occurence(); 

   sudoku_table(string board="") {
      /* PARAMS: an optional string representation of the sudoku board.
	 RETURN: None (default constructor)
	 DESCRI: If nothing is specified, create an empty sudoku. If
	 an 81 character string is specified, assume each position in 
	 the string represents a place in the table and initialize it
	 accordingly. */

      /* Invalid input or empty board string. Initialize to 0's */
      if (board == "" || board.size() != 81)
	 for (int i = 0; i < 81; i++)
	    {
	    table[i] = 0;
	    pencil_mark[i] = 0;
	    }
      else /* Valid string input, attempt to intialize sudoku board. */
	 for (int cell = 0; cell < 81; cell++)
	    {
	    pencil_mark[cell] = 0;
	    if (board[cell] != '.')
	       {
	       table[cell] = board[cell]-'0';
	       
	       }
	    else
	       table[cell] = 0;
	    }

      /* 
       * Set initial pencil marks
       */
      for (int i = 0; i < 81; i++)
	 {
	 // check all rows
	 check_row(i);
	 check_col(i);
	 check_subsquare(i);
	 }
   }
};

unsigned int bit(int val) 
/* PARAMS: a value (between 1-9, inclusive)
   RETURN: A bitwise array with the number marked. 
   DESCRI: We're only worried about the right-most 9 bits and mapping
   them to the values 1-9 (inclusive). This will make other code below
   a bit more legible. */
{
   if (val > 0 && val <= 9)
      return 1 << val - 1;

   throw logic_error("bit value out-of-range.");
}

int sudoku_table::zeros(unsigned int cell)
/* PARAMS: A cell address
   RETURN: Number of zero bits in the pencil_mark[cell]
   DESCRI: Pencil marks are kept in a small bit array, one
   popular calculation is how many zeros are in a given array
   (used to determine if we have a UNIQUE potential for a cell.) */
{
   unsigned int val = pencil_mark[cell];
   int z = 0;

   for (unsigned int mask = 1; mask <= 256; mask <<= 1)
      {
      /* XOR compares the ENTIRE number, so we make it match with the (mask | val)
	 so that only the mask bit we're checking may differ from val. */
      if ((mask | val) ^ val)
	 z++;
      }
   return z;
}

int sudoku_table::get_subsquare_address(int subsquare_number)
{
  int subsquare_addresses[] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
  return subsquare_addresses[subsquare_number];
}

unsigned int sudoku_table::check_row(int cell)
/* PARAMS: cell number whose row we wish to check
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate across a row and note which elements are present. 
   if an element is ALREADY specified, throw an exception, */
{

   unsigned int numbers = 0;

   /* give us the first element # for the row cell is in. */
   int row = 9 * (cell / 9); 

   /* For each element in cell's row: */
   for (int x = row; x < row+9; x++)
      {
      if (table[x]) /* if there is a number in this cell... */
	 {
	 if (numbers & 1 << table[x] - 1) /* and it's a duplicate... */
	    {
	    print_table();
	    cout << x << ": " << table[x] << endl;
	    throw logic_error("Duplicate Number in Row!");
	    }
	 else /* it's ok, note it. */
	    numbers |= 1 << table[x] - 1;
	 }
      }
   pencil_mark[cell] |= numbers;
   return numbers;
}

unsigned int sudoku_table::check_col(int cell)
/* PARAMS: cell number whose column we wish to check.
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate across a col and note which elements are present. */
{
   unsigned int numbers = 0;
   
   /* give us the first element in cell's column: */
   int col = cell % 9;
   for (int y = col; y <= col+72; y += 9)
      {
      if (table[y]) /* there is a number in this cell... */
	 {
	 if (numbers & 1 << table[y] - 1) /* and it's a duplicate... */
	    throw logic_error("Duplicate Number in Col!");
	 else /* it's ok, note it. */
	    numbers |= 1 << table[y] - 1;
	 }
      }

   pencil_mark[cell] |= numbers;

   return numbers;
}

int sudoku_table::sub_square(int cell)
/* PARAMS: the cell whose sub-square we wish to identify.
   RETURN: The number of the cell in the top-left of cell's sub-square.
   DESCRI: Convert cell to row,col coords and compare to know coordinates
   of subsquare boundaries. */
{
   int row = cell / 9;
   int col = cell % 9;

   for (int subsquare_row = 0;
	subsquare_row < 9; 
	subsquare_row += 3)
      {
      for (int subsquare_col = 0;
	   subsquare_col < 9;
	   subsquare_col += 3)
	 {
	 if (row >= subsquare_row && row < subsquare_row + 3)
	    if (col >= subsquare_col && col < subsquare_col + 3)
	       return (subsquare_row * 9) + subsquare_col;
	 }
      }
   return -1;
}

vector<int> sudoku_table::neighbors(int subsquare)
/* PARAMS: A subsquare address. Use sub_square() to determine this address
   from a cell number. 
   RETURN: A vector of cell addresses.
   DESCRI: Get all elements in a given sub_square (based on the cell address 
   of the top-left cell. */
{

  /* At the end of a subsquare row, add 7 for next row. */

   vector<int> ret;
   for (int row = 0; row < 3; row++)
      for (int col = 0; col < 3; col++)
	 ret.push_back(subsquare+col + 9*row);

   return ret;
}

unsigned int sudoku_table::check_subsquare(int cell)
/* PARAMS: 2d array of unsigned int (assumed 9x9 elements)
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate over a sub-box and note which elements are present. */
{
   vector<int> elements = neighbors( sub_square(cell) );
   unsigned int numbers = 0;

   for (int c = 0; c < elements.size(); c++)
      {
      if (table[ elements[c] ])
	 {
	 if ((1 << table[elements[c]] - 1) & numbers)
	    throw logic_error("Duplicate detected in subsquare.");
	 else 
	    numbers |= 1 << table[elements[c]] - 1;
	 
	 }
      }   

   pencil_mark[cell] |= numbers;

   return numbers;
}

void sudoku_table::print_table()
/* PARAMS: None
   RETURN: Void
   DESCRI: Print out the table (to the cli) for debugging. */
{
   int counter = 0;
   for (int i=0; i < 81; i++)
      {
      if (table[i] == 0)
	 cout << setw(3) << '_';
      else
	 cout << setw(3) << table[i];
      counter++;
      if (counter == 9)
	 {
	 counter = 0;
	 cout << endl;
	 
	 }
      }
}

void sudoku_table::print_web_table()
/* PARAMS: None
   RETURN: Void
   DESCRI: Print out the table to the web. */
{
   for (int i = 0; i < 81; i++)
      cout << table[i];
   cout << endl;
}

int sudoku_table::do_single_position()
/* PARAMS: None
   RETURN: Number of (new) cells filled.
   DESCRI: rip through every cell. When/if the pencil marks has only one 
   possibility, set it... */
{
   int cells_filled = 0;

   for (int i = 0; i < 81; i++)
      {
      if (table[i])
	 continue;
      if (zeros(i) == 1)
	 {
	 unsigned int bitset = 0x1FF - pencil_mark[i];
	 table[i] = 1;
	 while (!(1 & bitset))
	    {
	    table[i]++;
	    bitset >>= 1;
	    }
	 cells_filled++;

	 cout << "0x1FF - " << pencil_mark[i] << "\n";
	 }

      }
   /* update pencil marks */
   for (int i = 0; i < 81; i++)
      {
      // check all rows
      check_row(i);
      check_col(i);
      check_subsquare(i);
      }
   return cells_filled;
}

int sudoku_table::do_single_occurence()
{
   int cells_filled = 0;
   vector< vector<int> > cells(10);
   /*
    * Check each row for "single_occurrences"
    */
   for (int row = 0; row <= 72; row += 9)
      {
      /* Check every cell in a row */
      for (int cell = row; cell < row + 9; cell++)
	 {
	 /* Read the pencil marks into values */
	 unsigned int pencil = pencil_mark[cell];
	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(cell);
	 }

      /* Check our cells for unique values */
      for (int value = 0; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    table[cells[value][0]] = value;
	    cells_filled++;

	    /* update pencil marks for the row we've just finished */
	    check_row(cells[value][0]);
	    check_col(cells[value][0]);
	    check_subsquare(cells[value][0]);
	    }
	 
	 cells[value].clear();
	 }
      }

   /*
    * Check each col for "single_occurrences"
    */
   for (int col = 0; col < 9; col++)
      {
      /* For each cell in a column... */
      for (int cell = col; cell < col + 72; cell += 9)
	 {
	 /* Read the pencil marks into values */
	 unsigned int pencil = pencil_mark[cell];
	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(cell);
	 }

      /* Check our cells for unique values */
      for (int value = 0; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    table[cells[value][0]] = value;
	    cells_filled++;
	    /* update pencil marks for the column we've just finished */
	    check_row(cells[value][0]);
	    check_col(cells[value][0]);
	    check_subsquare(cells[value][0]);
	    }

	 cells[value].clear();
	 }
      }


   /*
    * Check each subsquare for "single_occurrences"
    */
   for (int subcell = 0; subcell < 9; subcell++)
      {
      vector<int> n = neighbors( get_subsquare_address(subcell) );
      for (int cell = 0; cell < 9; cell++)
	 {
	 /* Read the pencil marks into values */
	 unsigned int pencil = pencil_mark[n[cell]];
	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(n[cell]);
	 }
      for (int value = 0; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    table[cells[value][0]] = value;
	    cells_filled++;
	    cout << cells[value][0] << " could be " << value << endl;
	    /* update pencil marks for the subcell we've just finished. */
	    check_row(cells[value][0]);
	    check_col(cells[value][0]);
	    check_subsquare(cells[value][0]);
	    }
	 cells[value].clear();
	 }

      }

   return cells_filled;
}


template<class V>
void print_array(vector<V> vec)
/* PARAMS: vector of anything (that can be << to cout)
   RETURN: void
   DESCRI: Debugging function, cout's the contents of any vector. */
{
   for (int i = 0; i < vec.size(); i++)
      cout << vec[i] << " ";
   cout << endl;
}

int main()
{

   /* If this is called as CGI, do the CGI thing */
   if (strcmp( getenv("REQUEST_METHOD"), "POST" ) == 0)
      {
      cout << "Content-Type: text/plain\n\n";
      string puzzle;
      std::getline(cin, puzzle);
      sudoku_table table(puzzle);
      while (table.do_single_position())
	 ;
      table.print_web_table();

      return 0; 
      }

   /* Not called as CGI, so do the driver thing. */

   sudoku_table table(PALMS_PUZZLE);

   table.print_table();

   //   table.check_subsquare(0); /* will exit program if this test fails. */

   //   cout << hex << "pencil_mark: " << table.pencil_mark[64] << endl;

   while (table.do_single_position())
      {
      cout << ".";
      }
   cout << endl;
   

   cout << "\nAfter single_position:\n";
   table.print_table();
   int check_cell = 9;
   cout << "\n"
	<< "Pencil for " << check_cell << ": " << hex << table.pencil_mark[check_cell] << endl
	<< "Zeros for " << check_cell << ":  " << table.zeros(check_cell) << endl;

   cout << "Doing single_occurence (it's going to be great!)\n";
   cout << table.do_single_occurence() << endl;
   cout << table.do_single_occurence() << endl;
   cout << table.do_single_occurence() << endl;
   cout << table.do_single_occurence() << endl;
   table.print_table();


#ifdef DEBUG_PENCIL_ROWCOL

   for (int cell = 0; cell < 9; cell++)
      {
      cout << "Cell " << cell << " Row: " << cell/9 << " Col: " << cell%9 << endl;

      cout << hex << "check_row " << table.check_row(cell) << endl;
      cout << hex << "check_col " << table.check_col(cell) << endl;

      cout << "pencil_mark: " << hex << (table.pencil_mark[cell]) << endl;
      
      }

#endif
 
#ifdef DEBUG_MATHS
   cout << i << ": " << (i / 9) << "r" << (i % 9) 
	<< " = " << ( i % 9 ) - ( i / 9 ) << endl;
#endif

#ifdef DEBUG_NEIGHBORS
   for (int i = 0; i < 81; i++)
      {
      print_array(table.neighbors(table.sub_square(i)));
      }
#endif
}

/******************************************************************************
sudoku.cpp

CREATED BY: Shaun Meyer, Chelsey Dunivan
CREATED: 16 APR 2011
SUBMITTED BY: Shaun Meyer, Chelsey Dunivan, Steven Throne

DESCRIPTION:
Initial functions + class to interact with a sudoku board. 

TABLE OF CONTENTS:
*Sudoku (class)
:: Applies DLX algorithm to solve any puzzle.

*sudoku_table (class)
:: Applies PURE logic to solve nearly any human-solvable puzzle.
:: may guess with Nishio or Force Chains (but doesn't have to.)

Logic techniques used:
1. Solve Single Occurence. 
   If, among pencil marks in a row / col / subcell there is only a 
   single place for a given value, insert that value.
2. Solve Single Possibility
   If, among pencil marks, there is only one possibility for a row,
   fill it in.
3. Noshio guessing
   When logic fails, if there is a cell with 2 possibilities, guess one
   and try logic'ing on it. If this results valid table ASSUME it is a
   correct guess and keep solving. Hit or miss stuff, here.

HOW THIS PROGRAM WORKS:
Logic is first applied to the puzzle. The results are then passed to DLX
brute forcing. 

*****************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <cstdlib>
#include <string.h> // for strcmp()

using namespace std;

/*
 * Awesome test puzzles
 */
#define PLATINUM_BLONDE ".......12........3..23..4....18....5.6..7.8.......9.....85.....9...4.5..47...6..."
#define PALMS_PUZZLE "....3..51..36......2..948......5..7.59.....62.8..2......491..8......24..23..8...."
#define CHELSEY_PUZZLE "3...2...4.....8.23.784...6.8..1..4...2..8..5...7..9..6.8...614.59.8.....6...3...8"
#define EASY_PUZZLE "1234567.9789123456456789123312845967697312845845697312231574698968231574574968231"
#define INVALID_TABLE "11..............................................................................."
#define DEBUG 0
#define DEBUG_CHAINS 0
#define MAX_ITERATIONS 32

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

/******************************************************************************
 * SUDOKU (DLX solver) Class definition
 *****************************************************************************/


class Sudoku 
{

   /* 
    * Begin data-type definitions.
    */

   struct Column; 

   struct Node {
      Node *l, *r, *u, *d;
      Column *header;
      int row; /* Set what row we're on. (in the matrix, not the puzzle)  */
      Node()
      {
	 row = 0;
      }
      Node(Column *c)
      {
	 header = c;
      }
   };

   struct Column : public Node {
      int size;
      Column *L, *R; /* L and R are used because l and r pointing
			to a Node type, this avoids ugly type_casting. */
      bool covered;
      Column()
      {
	 size = 0;
	 covered = false;
      }
   };

   /*
    * Begin private member functions
    */

   void cover(Column *col)
   /* PARAMS: col ptr to the column object we're covering.
      RETURN: void
      DESCRI: Decends down a column (col), for each row we see
      traverse right on c to cover that node, also. */
   {
      col->R->L = col->L;
      col->L->R = col->R;
      col->covered = true;

      for (Node *row = col->d; row != col; row = row->d)
	 {
	 for (Node *c = row->r; c != row; c = c->r)
	    {
	    c->u->d = c->d;
	    c->d->u = c->u;
	    c->header->size--;
	    }
	 }
   }

   void uncover(Column *col)
   /* PARAMS: col ptr to the column object we're uncovering.
      RETURN: void
      DESCRI: Descends down a column (col), for each row we
      see, traverse right on c to uncover that node. */
   {
      col->R->L = col;
      col->L->R = col;
      col->covered = false;

      for (Node *row = col->d; row != col; row = row->d)
	 {
	 for (Node *c = row->r; c != row; c = c->r)
	    {
	    c->u->d = c;
	    c->d->u = c;
	    c->header->size++;
	    }
	 }
   }

   void search(int k)
   {
      /* 1. If the matrix A is empty, the problem is solved; terminate successfully.
	 2. Otherwise choose a column c `next` (deterministically).
	 3. Choose a row r such that Ar, c = 1 (nondeterministically).
	 4. Include row r in the partial solution.
	 5. For each column j such that Ar, j = 1,
	 for each row i such that Ai, j = 1,
	 delete row i from matrix A;
	 delete column j from matrix A.
	 6. Repeat this algorithm recursively on the reduced matrix A. */

      if (root->R == root)
	 {
	 solutions++;
	 return; /* #1. If the matrix is empty, the problem is solved. */
	 }

      Column *next = root->R;
      
      /* Choose the next column, look at the smallest number of possibilities
	 first. */
      for (Column *c = root->R; c != root; c = c->R)
	 {
	 if (c->size < next->size)
	    next = c;
	 if (next->size == 0) /* no solution in this subtree. */
	    return;
	 }

      cover(next);

      /* foreach r <D[c], D[D[c]], ...., while r != c */
      for (Node *i = next->d; i != next && solutions < max_solutions; i = i->d)
	 {
	 /* Set Ok <- R...
	    the last thing set will be the correct (while solutions==0) */
	 if (solutions == 0)
	    solution[k] = i->row;

	 /* foreach j <-R[r], R[R[r]], ... while j != r */
	 for (Node *j = i->r; j != i; j = j->r)
	    cover(j->header);

	 search(k + 1); /* recurse! */

	 /* set r<- Ok and c <- C[r]; */
	 /* foreach j <- L[r], L[L[r]], ..., while j != r */
	 for (Node *j = i->l; j != i ; j = j->l)
	    uncover(j->header);
	 }

      uncover(next);
   }


public:
   vector<Column> y;     /* columns */
   vector<vector<Node> > matrix;  /* our node matrix */
   Column *root;         /* root points to the first column. */
   int solutions;        /* number of solutions to puzzle. */
   int max_solutions;    /* when to stop finding solutions */
   vector<int> solution; /* final solution to puzzle */
   int BOX, UNIT, GRID;  /* a few dimensional variables */

   Sudoku(int size=3, int max=1) :
      /* Initialize some before the function body */
      BOX(size), /* size in a standard board is 3 (the diameter of a subcell) */
      UNIT(BOX*BOX), /* UNIT is the number of cells in a subcell, col, or row */
      GRID(UNIT*UNIT), /* The entire grid, then, is a UNIT*UNIT (9*9) cells. */
      solution(81),
      max_solutions(max)
   {
      matrix.resize(729, vector<Node>(4, Node()));
      y.resize(GRID*4+1, Column());
      root = &(y[y.size()-1]);
      /* We generate our matrix to look something like:
	 http://www.stolaf.edu/people/hansonr/sudoku/exactcovermatrix.htm
	
	 Where there are 4 constraints:
	 1. Cell (every cell needs a number 1-9)
	 2. Row (every row needs number 1-9)
	 3. Col (every column needs numbers 1-9)
	 4. Subcell (every sub-square needs numbers 1-9)
	 
	 a. 81 cells * 4 constraints = 324 (how many columns our 
	 matrix should have)
	 b. 9 rows * 9 cols * 9 subcells = 729 (how many rows our 
	 matrix should have)
      */

      /* Lets make our columns data structure! */

      int cols = GRID*4+1; // number of cols + root

      for (int i = 0; i < cols; i++)
	 {
	 y[i].R = &(y[(i+1)%cols]); /* i+1 % cols gives us the index 
				       one to the right (or wraps around 
				       to 0) */ 

	 y[i].L = &(y[(i+cols-1)%cols]); /* (i+cols-1)%cols gives us the index 
					    on to the left (or wraps around to 
					    the last col. */

	 y[i].d = y[i].u = &(y[i]); /* set both the up and down ptr 
				       to the column itself. */
	 }

      /* Lets make our rows data structure! */
      for (int i = 0; i < 729; i++)
	 {
	 /* For each general constraint, calculate 
	    the column that this node belongs to. */
	 for (int j = 0; j < 4; j++)
	    {
	    int header = GRID*j; /* which specific constraint 
				    are we working in? 0, 81, 162, or 243. */

	    /* Cell constraint: only one value in each of the 81 cells. 
	       for every row, we want 9 of these all in the same column before
	       shifting over one place (as in the cover matrix example. */
	    if (j == 0) 
	       {
	       header += i/9;
	       }

	    /* Row constraint: This moves diagonal (see matrix example), 
	       repeating 9 times before shifting over one place. */
	    else if (j == 1)
	       {
	       /* row%9 is the individual offset.
		  i/81*9 is the overall offset. */
	       header += i/81*9 + i%9;
	       }

	    /* column constraint. This is diagonal also, each consecutive 9
	       shifting over. The cycle repeats every 9 iterations. */
	    else if (j == 2)
	       {
	       /* i%82 gives us a constant, repeating, diagonal. */
	       header += i%81;
	       }

	    /* subcell constraint. This is a little trickier (see matrix 
	       example.) Each is diagonal and shifts over 9 places every
	       3*9 rows. This pattern repeats 3x before shifting over (minor 
	       pattern.) the Minor pattern repeats 3x before shifting over 
	       (major pattern. ) */
	    else if (j == 3)
	       {
	       /* i%9 will give us a basic diagonal.
		  (i%81/27)*9 will give us the shift for the minor pattern
		  where 81 is the size of the entire pattern and 27 is the 
		  length of a cycle within that pattern. 
		  (i/243)*27 will give us the shift for the major pattern */
	       header += i%9 + (i%81/27)*9 + (i/243)*27;
	       }

	    matrix[i][j].r = &(matrix[i][(j+1)%4]); /* r == next j, or wrap */
	    matrix[i][j].l = &(matrix[i][(j+3)%4]); /* l == prev j, or wrap. */
	    matrix[i][j].u = y[header].u; /* inherit our column->up */

	    /* Actually insert the row. */
	    y[header].u = y[header].u->d = &(matrix[i][j]);

	    /* set d to header */
	    matrix[i][j].d = matrix[i][j].header = &(y[header]); 

	    matrix[i][j].row = i;
	    matrix[i][j].header->size++;
	    }
	 }

   }

   int solve(vector<int> &puzzle)
   /* PARAMS: puzzle
      RETURN: Number of Solutions. (If there are more than one)
      DESCRI: Applys the DLX algorithm to recusively search for solution(s)
      to the puzzle. */
   {
      if (puzzle.size() != 81)
	 return -1;

      int k = 0;
      solutions = 0;
      
      bool solveable = true;

      for (int i = 0; i < 81; i++)
	 {
	 if (puzzle[i])
	    {
	    /* deduce the row # from our puzzle. Cells are counted from top 
	       left, 0. i*9, therefore, will give us the set of 9 possibilities
	       and adding (value-1) to that will give the row representing
	       puzzle[i] in cell [i]. */
	    int row = i*9 + puzzle[i] - 1;
	    
	    /* if any of the row's constraints are covered, 
	       there is no solution (eg, user puzzle had doubles in it) */
	    if (matrix[row][0].header->covered
		|| matrix[row][1].header->covered
		|| matrix[row][2].header->covered
		|| matrix[row][3].header->covered)
	       {
	       solveable = false;
	       break;
	       }

	    /* Cover this row / col because the user has entered it, 
	       it has to occur in the solution. */
	    for (int j = 0; j < 4; j++)
	       cover(matrix[row][j].header);

	    /* increment k and note row. */
	    solution[k++] = row;
	    }
	 }

      /* After adding the user puzzle, if it is valid (solvable), 
	 do a search for the solution to the exact-cover problem. */
      if (solveable)
	 search(k);

      /* If we've found a solution, we must convert the ROW number into
	 a meaningful value to place in that cell. This will be the opposite
	 of the formula used above to convert a value+cell into a row. 
	 (row = cell*9 + value - 1) */
      if (solutions > 0)
	 for (int i = 0; i < 81; i++)
	    {
	    int cell = solution[i] / 9;
	    int value = solution[i] % 9 + 1;
	    puzzle[cell] = value;
	    }

      /* uncover all headers (in case we want to do this again. */
      for (int i = k - 1; i >=0; i--)
	 for (int j = 3; j >= 0; j--)
	    uncover(matrix[solution[i]][j].header);

      /* return the solution count. */
      if (solveable)
	 return solutions;
      else
	 return -2;
   }
   
};

/******************************************************************************
 * SUDOKU_TABLE (logic solver) Class definition
 *****************************************************************************/


struct sudoku_table {

   /* 
    * Member variables
    */

   enum {ONE = 0x1,
	 TWO = 0x2,
	 THREE = 0x4,
	 FOUR = 0x8,
	 FIVE = 0x16,
	 SIX = 0x32,
	 SEVEN = 0x64,
	 EIGHT = 0x128,
	 NINE = 0x256,
	 TEN = 0x512
   };

   vector<int> table; /* the table (expressed as a 1d array. */
   vector<vector <int> > rows, cols, subcells;
   
   vector<unsigned int> pencil_mark; /* temp table for scribbling in pencil marks */

   /* 
    * Prototypes 
    */
   vector<int> &tablevector();
   unsigned int bit(int val);
   void set(int cell, int value);

   int get_subsquare_address(int subsquare_number);
   int get_subsquare_from_address(int cell);

   unsigned int check_row(int cell);
   unsigned int check_col(int cell);
   unsigned int check_subsquare(int cell);
   vector<int>  neighbors(int subsquare);
   int          sub_square(int cell);
   void         print_table();
   void         print_web_table();
   bool         is_solved();
   bool         valid_table();
   int zeros(unsigned int cell);

   int do_single_position();
   int do_single_occurence(); 
   int do_single_occurence_row(); 
   int do_single_occurence_col(); 
   int do_single_occurence_subcell();

   int find_cell_by_pmark(int marks = 2, int start_cell=0);
   int check_by_logic(int check_steps = MAX_ITERATIONS);
   vector<int> values_in_pmark(unsigned int pmark);

   int nishio(int check_steps=16);
   int force_chain(int start_cell=0);

   int do_doubles();

   sudoku_table(string board="") {
      /* PARAMS: an optional string representation of the sudoku board.
	 RETURN: None (default constructor)
	 DESCRI: If nothing is specified, create an empty sudoku. If
	 an 81 character string is specified, assume each position in 
	 the string represents a place in the table and initialize it
	 accordingly. */

      table.resize(81);
      pencil_mark.resize(81);

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
       * Set rows, cols, and subcells references. 
       * (it's gonna be great!)
       */
      rows.resize(9); cols.resize(9); subcells.resize(9);
      for (int i = 0; i < 81; i++)
	 {
	 rows[i/9].push_back(i);
	 cols[i%9].push_back(i);
	 subcells[get_subsquare_from_address(i)].push_back(i);
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

vector<int> &sudoku_table::tablevector()
{
   return table;
}

void sudoku_table::set(int cell, int value)
/* PARAMS: A cell address, and a cell value.
   RETURN: void
   DESCRI: Set a cell, and update its pencil marks. */
{
   table[cell] = value;
   unsigned int mask = (1 << value - 1);

   int row = cell / 9;
   int col = cell % 9;
   int subcell = get_subsquare_from_address(cell);

   /* unset that pencil_mark in all of the same row/col/subcell */
   for (int i = 0; i < 9; i++)
      {
      pencil_mark[rows[row][i]] |= mask;
      pencil_mark[cols[col][i]] |= mask;
      pencil_mark[subcells[subcell][i]] |= mask;
      }

}

bool sudoku_table::is_solved()
/* PARAMS: void
   RETURN: True if a puzzle is completed.
   DESCRI: Quick test if a puzzle is solved. */
{
   int entries = 0;
   for (int i = 0; i < 81; i++)
      {
      check_row(i);
      check_col(i);
      check_subsquare(i);

      if (table[i])
	 entries++;
      }

   if (entries == 81)
      return true;
   return false;
}

bool sudoku_table::valid_table()
/* PARAMS: None
   RETURN: true/false (true if table contains no contradictions)
   DESCRI: Check all rows/cols/subcells for contradictions */
{
   for (int i = 0; i < 81; i++)
      {
      if (table[i] == 0 && pencil_mark[i] == 511)
	 return false;

      try{
      check_row(i);
      check_col(i);
      check_subsquare(i);
      }
      catch (logic_error &e)  {
      return false;
      }
      }
   return true;
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
/* PARAMS: A subsquare number 0-8
   RETURN: A subsquare cell address
   DESCRI: A helper function to convert addresses. */
{
   int subsquare_addresses[] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
   return subsquare_addresses[subsquare_number];
}

int sudoku_table::get_subsquare_from_address(int cell)
/* PARAMS: A cell address
   RETURN: The subsquare number (0-8)
   DESCRI: Reverse get_subsquare_address() helper */
{
   int subsquare_addresses[] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
   cell = sub_square(cell);
   for (int i = 0; i < 9; i++)
      if (subsquare_addresses[i] == cell)
	 return i;

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
      if (table[x] == 0) /* if there is no number in this cell... */
	 continue;
      
      if (numbers & 1 << table[x] - 1) /* and it's a duplicate... */
	 throw logic_error("Duplicate Number in Row!");

      /* it's ok, note it. */
      numbers |= 1 << table[x] - 1;
      
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
	    {
	    print_table();
	    throw logic_error("Duplicate Number in Col!");
	    }
	 /* it's ok, note it. */
	 numbers |= 1 << table[y] - 1;
	 }
      }

   pencil_mark[cell] |= numbers;

   return numbers;
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
	 int value = 1;

	 while (!(1 & bitset))
	    {
	    value++;
	    bitset >>= 1;
	    }

	 set(i, value);
	 cells_filled++;

	 }

      }
   /* update pencil marks */
   for (int i = 0; i < 81; i++)
      {
      // check all rows
      pencil_mark[i] = 0;
      check_row(i);
      check_col(i);
      check_subsquare(i);
      }
   return cells_filled;
}

int sudoku_table::do_single_occurence()
/* PARAMS: void
   RETURN: Number of modified cells
   DESCRI: Wrapper function to solve single occurences. */
{
   return do_single_occurence_row() +
      do_single_occurence_col() +
      do_single_occurence_subcell();
}

int sudoku_table::do_single_occurence_row() 
/* PARAMS: void
   RETURN: Number of modified cells
   DESCRI: Solve single occurences in rows. */
{
   int cells_filled = 0;
   vector< vector<int> > cells(10);
   /*
    * Check each row for "single_occurrences"
    */
   for (int row = 0; row <= 72; row += 9)
      {
      if (DEBUG)
	 cout << "Checking row " << row << '\t';

      /* Check every cell in a row */
      for (int cell = row; cell < row + 9; cell++)
	 {
	 if (table[cell])
	    continue; 

	 if (DEBUG) cout << setw(3)<< dec << cell;

	 /* Read the pencil marks into values `v' */
	 unsigned int pencil = pencil_mark[cell];
	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(cell);
	 }
      if (DEBUG) cout << endl;
      /* Check our cells for unique values & fill them in */
      for (int value = 1; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    set(cells[value][0], value);
	    cells_filled++;

	    /* update pencil marks for the row we've just finished */
	    check_row(cells[value][0]);
	    check_col(cells[value][0]);
	    check_subsquare(cells[value][0]);
	    }
	 cells[value].clear();
	 }
      }

   return cells_filled;
}

int sudoku_table::do_single_occurence_col()
/* PARAMS: void
   RETURN: Number of modified cells
   DESCRI: Solve single occurences in cols. */
{
   int cells_filled = 0;
   vector< vector<int> > cells(10);

   /*
    * Check each col for "single_occurrences"
    */
   for (int col = 0; col < 9; col++)
      {
      if (DEBUG) cout << "Checking col " << col;

      /* For each cell in a column... */
      for (int cell = col; cell <= col + 72; cell += 9)
	 {
	 if (table[cell]) 
	    continue;

	 if (DEBUG) cout << setw(3) << dec << cell;

	 /* Read the pencil marks into values */
	 unsigned int pencil = pencil_mark[cell];
	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(cell);
	 }

      if (DEBUG) cout << endl;

      /* Check our cells for unique values */
      for (int value = 1; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    set(cells[value][0], value);
	    cells_filled++;
	    /* update pencil marks for the column we've just finished */
	    check_row(cells[value][0]);
	    check_col(cells[value][0]);
	    check_subsquare(cells[value][0]);
	    }

	 cells[value].clear();
	 }
      }

   return cells_filled;
}

int sudoku_table::do_single_occurence_subcell()
/* PARAMS: void
   RETURN: Number of modified cells
   DESCRI: Solve single occurences in subcells. */
{
   int cells_filled = 0;
   vector< vector<int> > cells(10);

   /*
    * Check each subsquare for "single_occurrences"
    */
   for (int subcell = 0; subcell < 9; subcell++)
      {
      if (DEBUG) cout << "Checking subcell " << subcell;

      vector<int> n = neighbors( get_subsquare_address(subcell) );
      for (int cell = 0; cell < 9; cell++)
	 {
	 if (table[n[cell]])
	    continue;

	 if (DEBUG) cout << setw(3) << dec << n[cell];

	 /* Read the pencil marks into values */
	 unsigned int pencil = pencil_mark[n[cell]];

	 for (unsigned int mark = 1, v = 1; v <= 9; mark <<= 1, v++)
	    if (mark & ~pencil)
	       cells[v].push_back(n[cell]);
	 }

      if (DEBUG) cout << endl;

      for (int value = 1; value < cells.size(); value++)
	 {
	 if (cells[value].size() == 1) /* fill it in, it's unique! */
	    {
	    set(cells[value][0], value);
	    cells_filled++;

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

int sudoku_table::find_cell_by_pmark(int marks, int start_cell)
{
   int cell = -1;
   for (int i = start_cell; i < 81 && cell < 0; i++)
      {
      if (table[i])
	 continue; /* This cell is already solved. No point. */
      
      if (zeros(i) != 2)
	 continue; /* there are more than two possibilities. */
      
      /* If we've made it this far, we're at a cell with two possibilities! */
      cell = i; 
      }
   return cell;
}

int sudoku_table::check_by_logic(int check_steps)
{
   int changes = 0;
   for (int i = 0; i < check_steps; i++)
      {
      int new_changes; /* Changes made in THIS iteration. */

      /* Do as many do_single_position() as makes sense */
      while (new_changes = do_single_position())
	 changes += new_changes;

      /* Count our changes + do_single_occurence() changes*/
      new_changes += do_single_occurence();
      changes += new_changes;

      /* If we're not doing anything anymore we should quit this. */
      if (new_changes == 0)
	 break; 
      }

   return changes;
}

vector<int> sudoku_table::values_in_pmark(unsigned int pmark)
{

   unsigned int mask;
   int value;

   vector<int> ret;
   for (value = 1, mask = 1; 
	value < 10;  /* note the available values */
	mask <<= 1, value++)
      {
      if (mask & ~pmark)
	 ret.push_back(value);
      }

   return ret;
}

int sudoku_table::nishio(int check_steps)
/* PARAMS: void
   RETURN: ?? (error codes. Not used)
   DESCRI: Attempt the Nishio technique (guess on a pair and try a few 
   iterations to see if our guess was correct.) */
{
   if (valid_table() == false)
      {
      if (DEBUG) 
	 cout << "Table is in an invalid state. Will not run Nishio on it.\n"; 
      return -2; // no sense guessing on an invalid table!
      }

   /* Find a cell with only two possibilities. */

   int cell = find_cell_by_pmark();

   if (cell < 0)
      {
      if (DEBUG)
	 cout << "Nishio could find no cells with only two possiblities.\n";
      return -1; /* no cell found with only two possibilities. */
      }

   else if (DEBUG)
      cout << "Nishio: Chose cell " << cell << endl;

   vector<int> possible_values = values_in_pmark(pencil_mark[cell]);
   vector<int> original_table = table;
   vector<unsigned int> original_pmark = pencil_mark;
   
   set(cell, possible_values[0]); /* Actually setting our "guess" */

   /* Try logic'ing the table. */
   check_by_logic(check_steps);

   /* Our guess was wrong. Good thing there were only two possibilities! */
   if (valid_table() == false)
      {
      if (DEBUG)
	 cout << "Invalid table! Reverting...\n";
      table = original_table; /* revert the table to the pre-guess state */
      pencil_mark = original_pmark;
      set(cell, possible_values[1]); /* Set the cell to the OTHER 
					guess (one MUST be right.) */

      /* Try logic'ing again. */
      check_by_logic(check_steps);
      }

   if (valid_table() == false)
      {
      table = original_table; /* something went wrong, put it all back. */
      pencil_mark = original_pmark;
      }

   /* If everything is OK after X moves, consider the Nishio a success. */
}

int sudoku_table::force_chain(int start_cell)
/* PARAMS: void
   RETURN: number of changes
   DESCRI: Finds a cell with only two possibilities, makes the change and 
   checks to see what cells stay the same with either possibility. */
{
   int changes = 0;
   int cell = -1;
   while ((cell = find_cell_by_pmark(2, cell+1)) >= 0)
      {
      if (DEBUG_CHAINS)
	 cout << "Trying forced chains with cell " << cell << " ";


      bool table_ok = true; 

      vector<vector<int> > states;
      vector<vector<unsigned int> > pmark_states;
      
      states.push_back(table);
      pmark_states.push_back(pencil_mark);
      
      if (cell < 0)
	 return 0; /* no appropriate cells. Nothing to do. */

      vector<int> values = values_in_pmark(pencil_mark[cell]);

      if (DEBUG_CHAINS)
	 {
	 cout << "(values:";
	 for (int i = 0; i < values.size(); i++)
	    cout << setw(3) << values[i];
	 cout << ")" << endl;
	 }
   
      /* Create and try a table of each potential. */
      for (int i = 0; i < 3 && table_ok; i++)
	 {
	 /* Set possibility. */
	 set(cell, values[i]);
	 check_by_logic();
	 
	 if (!valid_table())
	    {
	    table_ok = false;
	    }
	 else /* table is ok, store its state. */
	    {
	    states.push_back(table);
	    pmark_states.push_back(pencil_mark);
	    }

	 /* Revert global state. */
	 table = states[0];
	 pencil_mark = pmark_states[0];
	 }

      /* Tables don't make sense, skip. */
      if (!table_ok)
	 continue;

      /* Compare all three tables to see which cells are the same... */
      vector<int> unchanged_cells;
      for (int i = 0; i < 81; i++)
	 {
	 if (states[0][i])
	    continue; /* this cell is already filled in! */
      
	 /* If the number, in each of the two possibilities,
	    matches, it is a sure thing. Set it. */
	 if (states[1][i] && states[2][i] && states[1][i] == states[2][i])
	    {
	    set(cell, states[1][i]);
	    changes++;
	    cout << "Cell match! " << cell << endl;
	    }
	 }
      }
   return changes;
}

int sudoku_table::do_doubles()
/* PARAMS: void
   RETURN: ??
   DESCRI: Do doubles logic technique. */
{

   /*  Chelsey says we didn't really need this function.*/

}

int main()
{
   cout << "Content-Type: text/plain\n\n";

   try {
   string puzzle;
   std::getline(cin, puzzle);

   sudoku_table logic_solver(puzzle);
   /* Solve as far as we can with pure logics ( no guessing! )*/
   logic_solver.check_by_logic();

   /* Brute force the rest via dancing links */
   Sudoku dlx;
   dlx.solve(logic_solver.tablevector());

   /* Output the (beautiful) results. */
   logic_solver.print_web_table();
   }
   catch (logic_error &e)
      {
      cout << "Invalid table!\n";
      }
      
   /* Cool, refreshing... */
   return 0;

}

int main2()
/* The main that used to be main. It is here for historical purposes. */
{

   /* If this is called as CGI, do the CGI thing */
   if (getenv("REQUEST_METHOD") && 
       strcmp( getenv("REQUEST_METHOD"), "POST" ) == 0)
      {
      cout << "Content-Type: text/plain\n\n";
      string puzzle;
      std::getline(cin, puzzle);
      sudoku_table table(puzzle);

      int max_iterations = MAX_ITERATIONS;
      int step = 0;

      while (!table.is_solved() && max_iterations > 0)
	 {
	 int modified_cells = 0;
	 int new_cells = 0;
	 while ((new_cells = table.do_single_position()))
	    modified_cells += new_cells;
      
	 modified_cells += table.do_single_occurence();
     
	 max_iterations--;
      
	 if (modified_cells == 0)
	    //table.nishio(MAX_ITERATIONS*3);
	    table.force_chain();
            
	 }

      table.print_web_table();

      return 0; 
      }

   /**********************************************
    * Not called as CGI, so do the driver thing. *
    *********************************************/
   try 
      {
      sudoku_table logic_solver(INVALID_TABLE);
      logic_solver.print_table();

      //   table.check_subsquare(0); /* will exit program if this test fails. */

      //   cout << hex << "pencil_mark: " << table.pencil_mark[64] << endl;

      /* // This is/was the original solving by logic:
	 while (!table.is_solved() && max_iterations > 0)
	 {
	 int modified_cells = table.check_by_logic();

	 if (modified_cells == 0)
	 //table.nishio(MAX_ITERATIONS*3);
	 modified_cells += table.force_chain();

	 if (modified_cells == 0)
	 {
	 cout << "Trying nishio()...\n";
	 table.nishio();
	 }

	 max_iterations--;
	 }
      */
      int step = logic_solver.check_by_logic();
      cout << "Steps solved with logic: " << step << endl;
      Sudoku dlx;
      dlx.solve(logic_solver.tablevector());

      cout << endl;
   
      logic_solver.print_table();

      cout << "Table is valid? " << (logic_solver.valid_table() ? 'Y' : 'N') << endl;

      int check_cell = 0;
      cout << "\n"
	   << "Pencil for " << check_cell << ": " << hex << logic_solver.pencil_mark[check_cell] << endl
	   << "Zeros for " << check_cell << ":  " << logic_solver.zeros(check_cell) << endl;


      int i = 2;

      }
   catch (logic_error)
      {
      cout << "Failed table!\n";
      return -1;
      }
}

/* El fin. */

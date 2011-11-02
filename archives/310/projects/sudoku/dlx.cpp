#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

class dlx {
   class Column; 

   class Node {
   public:
      Column *header;
      Node  *up, *down, *left, *right;
      int row; /* row number */
      /*
      Node() 
      {
	 header = NULL;
	 up = down = left = right = NULL;
	 } */

   };

   class Column : public Node {
   public:
      Column *left, *right;
      Node *up, *down;
      int size;
      bool covered;

      Column()
      {
	 size = 0;
	 left = right = NULL;
	 up = down = this;
      }
   };
   
   /* DLX data members */
   Node *root;
   vector< vector<Node> > matrix;
   vector<Column> columns;
   vector<int> solution;
   
   /* Sizes */
   int BOX, UNIT, GRID;
   
private:
   void cover(Column *col)
   {
      col->covered = true;
      col->right->left = col->left;
      col->left->right = col->right;
      for (Node *row = col->down; row != col; row = row->down)
	 {
	 for (Node *c = row->right; c != row; c = c->right)
	    {
	    c->up->down = c->down;
	    c->down->up = c->up;
	    c->header->size--;
	    }
	 }
   }

   void uncover(Column *col)
   {
      col->covered = false;
      col->right->left = col;
      col->left->right = col;
      for (Node *row = col->down; row != col; row = row->down)
	 {
	 for (Node *c = row->right; c != row; c = c->right)
	    {
	    c->up->down = c;
	    c->down->up = c;
	    c->header->size++;
	    }
	 }
   }

   void search(int k)
   {
      /* Donald Knuth's Algorithm X is as follows:
	 (http://en.wikipedia.org/wiki/Knuth's_Algorithm_X)
	 1. If the matrix A is empty, the problem is solved; terminate successfully.
	 2. Otherwise choose a column c (deterministically).
	 3. Choose a row r such that Ar, c = 1 (nondeterministically).
	 4. Include row r in the partial solution.
	 5. For each column j such that Ar, j = 1,
	    5.1  for each row i such that Ai, j = 1,
	    5.2    delete row i from matrix A;
	    5.3  delete column j from matrix A.
	 6. Repeat this algorithm recursively on the reduced matrix A.
      */
	 if (root->right = root)
	 {
	 return; /* solution found */
	 }

   }

public:
   dlx(int sz)
   {
      BOX = sz; /* Describing a typical sudoku board, a box is a 3 cells wide. */
      UNIT = BOX * BOX; /* A UNIT is 3 cells by 3 cells. */
      GRID = UNIT * UNIT; /* A GRID is 9 cells by 9 cells (or 3 units by 3 units...) */

      int g4 = GRID*4+1; /* pre-calculate this since we'll be using it a lot. */

      /* Set vector sizes... */
      solution.resize(GRID); /* our solution will be GRID cells long. */
      columns.resize(g4, Column()); /* there are GRID possibilities with 4 constraints. */

      /* there are GRID*UNIT rows, and 4 columns */
      matrix.resize(GRID*UNIT,  
	     vector<Node>(4, Node()));

      root = &(columns[GRID*4]); // the last item in the columns shall be the "root" */

      /* Set all columns' left and right pointers. */
      for (int i = 0; i < columns.size(); i++) 
	 {
	 columns[i].left = &(columns[(i+g4-1)%g4]); /* one i to the left, using mod to wrap to the last column if i==0. */
	 columns[i].right = &(columns[(i+1)%g4]); /* one i to the right, using mod to wrap to the first column. */
	 }
      
      /* Set the rows... */
      for (int row = 0; row < GRID*UNIT; row++)
	 {
	 for (int col = 0; col < 4; col++) /* four constraints. */
	    {
	    matrix[row][col].right = &(matrix[row][(col+1)%4]); /* one column to the right, mod 4 to loop around if need be. */
	    matrix[row][col].left = &(matrix[row][(col+3)%4]); /* one column to the left, mod'ed to loop around if need be. */
	    matrix[row][col].row = row; /* set our current row number */

	    int colNum = GRID*col%4;
	    
	    if (colNum == 0) /* All cells must be filled in. */
	       colNum += row / GRID * UNIT + row % GRID / UNIT;
	    else if (colNum == 1) /* All rows must have 1-9 */
	       colNum += row / GRID * UNIT + row % UNIT;
	    else if (colNum == 2) /* All cols must have 1-9 */
	       colNum += row % GRID / UNIT * UNIT + row % UNIT;
	    else if (colNum == 3) /* All subsquares must have 1-9 */
	       colNum += (row / GRID / BOX * BOX + row % GRID / UNIT / BOX) * UNIT + row % UNIT;
	    else
	       {
	       cout << colNum << endl;
	       throw runtime_error("Something went wrong!!");
	       }

	    matrix[row][col].down = matrix[row][col].header = &(columns[colNum]);

	    /* Inherit the column headers' up pointer */
	    matrix[row][col].up = columns[colNum].up; 

	    /* Set the new column header down pointer */
	    matrix[row][col].up = columns[colNum].up = &(matrix[row][col]);

	    /* Count this row */
	    columns[colNum].size++;
	       
	    }
	 }
     
      
   }

   void print_matrix()
   {
      cout << matrix.size()
	   << endl
	   << matrix[0].size() 
	   << endl;
   }

   void insert_row(const string &row)
   /* row is a string of 1's and 0's. size() should be less 
      than or equal to the row_width. 
      if it is less than, we assume it is fixed by the least
      signifiant digits. */
   {

   }
};

int main()
{
   dlx foo(8);
   foo.insert_row("10110001");
   foo.print_matrix();
   return 0;
}

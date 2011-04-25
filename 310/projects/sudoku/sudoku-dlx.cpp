#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

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
      /*y(GRID*4+1, Column()), /* See #a below +1 for the root node. */
      /* matrix(729*4, Node()), /* This only needs to be 4 because 
	 we never have more than 4 ones 
	 on a given a row. Sparse! */
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

int main(int argc, char **argv)
{
   Sudoku sudoku;
   vector<int> puzzle;
   string inpt;

   if (argc == 2)
      {
      ifstream pfile(argv[1]);

      if (!pfile)
	 {
	 cout << "Invalid puzzle file..?\n";
	 return 0;
	 }

      while (std::getline(pfile, inpt))
	 {
	 puzzle.resize(81);
	 for (int i = 0; i < inpt.size(); i++)
	    {
	    if (inpt[i] == '.')
	       puzzle[i] = 0;
	    else
	       {
	       int cell = inpt[i]-'0';
	       puzzle[i] = cell;
	       }
	    }

	 int solutions = sudoku.solve(puzzle);
	 for (int i = 0; i < puzzle.size(); i++)
	    cout << puzzle[i]; 
	 cout << endl;
	 }
      }
   else
      { /* read std in */
      std::getline(cin, inpt);
	 
      for (int i = 0; i < inpt.size(); i++)
	 {
	 if (inpt[i] == '.')
	    puzzle.push_back(0);
	 else
	    {
	    int cell = inpt[i]-'0';
	    puzzle.push_back(cell);
	    }
	 }
	 
	
      int solutions = sudoku.solve(puzzle);
	 
      /* Output an appropriate header */
      if (getenv("REQUEST_METHOD"))
	 cout << "Content-Type: text/plain\n\n";
      else 
	 cout << solutions << " Solutions.\n";
	 
      /* output the solution: */
      for (int i = 0; i < puzzle.size(); i++)
	 cout << puzzle[i];
      cout << endl;
	 
      //cout << "Goodbye!\n";
	 
      return 0;
      }
}

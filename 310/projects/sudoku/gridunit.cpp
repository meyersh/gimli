#include <iostream>
#include <iomanip>

using namespace std;
int main()
{
   /* Set the rows... */
   int GRID, BOX, UNIT;
   BOX = 3;
   UNIT = BOX*BOX;
   GRID = UNIT*UNIT;

   for (int i = 0; i < GRID*UNIT; i++)
      {
      for (int j = 0; j < 4; j++) /* four constraints. */
	 {
	 int colNum = GRID*j;
	 int header = colNum;
	    
	 if (j == 0) /* All cells must be filled in. */
	    colNum += i/GRID*UNIT+i%GRID/UNIT;
	 else if (j == 1) /* All rows must have 1-9 */
	    colNum += i/GRID*UNIT+i%UNIT;
	 else if (j == 2) /* All cols must have 1-9 */
	    colNum += i%GRID/UNIT*UNIT+i%UNIT;
	 else if (j == 3) /* All subsquares must have 1-9 */
	    colNum += (i/GRID/BOX*BOX+i%GRID/UNIT/BOX)*UNIT+i%UNIT;

	 if (j == 0) 
	    {
	    header += i/9;
	    }

	 /* Row constraint: This moves diagonal (see matrix example), 
	    repeating 9 times before shifting over one place. */
	 else if (j == 1)
	    {
	    /* row%9 is the individual offset.
	       is the overall offset. */
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
	    3*9 rows. It resets every 9*9 rows. */
	 else if (j == 3)
	    {
	    /* i%9 will give us a basic diagonal.
	       (i%81/27)*9 will give us the shift for the minor pattern
	       where 81 is the size of the entire pattern and 27 is the 
	       length of a cycle within that pattern. 
	       (i%729/243)*18 will give us the shift for the major pattern
	    */
	    header += i%9 + (i%81/27)*9 + (i/243)*27;
	    }

	 if (colNum != header)
	    cout << "row: " << i << setw(6) << "col" << j << ": " << colNum
		 << " | " << header << " " << endl;
	 
	 }
      }
}

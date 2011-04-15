struct sudoku_table {
   unsigned int table[9][9];
};

unsigned int check_row(unsigned int **table, int row)
/* PARAMS: 2d array of unsigned int (assumed 9x9 elements)
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate across a row and note which elements are present. */
{
   unsigned int numbers = 0;
   for (int x = 0; x < 9; x++)
      {
      if (table[row][x])
	 numbers |= 1 << table[row][x];
      }

   return numbers;
}

unsigned int check_col(unsigned int **table, int col)
/* PARAMS: 2d array of unsigned int (assumed 9x9 elements)
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate across a col and note which elements are present. */
{
   unsigned int numbers = 0;
   for (int y = 0; y < 9; y++)
      {
      if (table[y][col])
	 numbers |= 1 << table[y][col];
      }

   return numbers;
}

unsigned int check_box(unsigned int **table)
/* PARAMS: 2d array of unsigned int (assumed 9x9 elements)
   RETURN: bitwise array indicating which elements have been found
   DESCRI: Iterate over a sub-box and note which elements are present. */
{
}

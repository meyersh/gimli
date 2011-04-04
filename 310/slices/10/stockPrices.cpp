/* 
   stockPrices.cpp
   SUBMITTED BY: Shaun Meyer
   CREATED BY: Shaun Meyer
   CREATED Apr 4 2011
   
   DESCRIPTION:

   This is a standalone program. Read in data from a file. (The name
  of the file should be given as a command-line parameter.) Each line
  of the file has a set of pairs. The first part of each pair is a
  stock symbol (each symbol is composed of one or more uppercase
  letters and may contain one period (.) which must have at least one
  upper case letter to either side of it). The second part is a price,
  which is a real number. A colon (:) separates the two parts. Each
  stock symbol should appear no more than once. (There can be no more
  than one price per symbol).  

  The names should be stored in a ternary search tree. The index
  should hold the price.

  Once the data has been read in, the user should have a menu of the
  following options:
  
  1.  Look up a price 
  2.  Change a price 
  3.  Delete a stock 
  4.  Quit

  When the first option is selected, the program should prompt the
  user for a stock symbol. If the symbol is not found, the user should
  be informed of that. If found, then the price should be
  displayed. In either case, the user is then taken back to the menu.

  When the second option is selected, the program should prompt the
  user for a stock symbol and a price. If the symbol is not found, it
  should be added with the indicated price. If found, then the price
  should be changed. In either case, the symbol and price should be
  displayed and the user should then be taken back to the menu.  

  When the third option is selected the user should be prompted for a
  stock symbol. If it is in the tree, it should be deleted and a
  message to this effect given to the user. If it is not in the tree,
  a message to that effect should be given to the user. In either
  case, the user should then be taken back to the menu.

If Quit is selected, the program should quit.

*/

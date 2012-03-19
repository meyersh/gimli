#include <iostream>
#include <vector>
#include "assert.h"
#include "Pente.hpp"

using namespace std;

int main() {
   Pente p;

   p.players[0] = "COMPUTER";
   p.players[1] = "TesterMcTester";


   // Test playerNumber functionality
   assert (p.playerNumber("COMPUTER") == 0);
   assert (p.playerNumber("TesterMcTester") == 1);
   assert (p.playerNumber("Wrong!") == -1);

   // Test playerColor functionality
   assert (p.playerColor("COMPUTER") == WHITE);
   assert (p.playerColor("TesterMcTester") == BLACK);
   assert (p.playerColor("Wrong!") == EMPTY);

   // Test coords functionality
   assert (p.isValidCoords(-1,0) == false);
   assert (p.isValidCoords(0,-1) == false);
   assert (p.isValidCoords(19,0) == false);
   assert (p.isValidCoords(0,19) == false);

   assert (p.isValidCoords(0,0) == true);

   // Test color functionality
   assert (p.isValidColor('B') == true);
   assert (p.isValidColor('b') == true);
   assert (p.isValidColor('W') == true);
   assert (p.isValidColor('w') == true);

   assert (p.isValidColor('x') == false);

   // Test fillCell, getFilled, and getEmpty functionality
   p.fillCell(2,12, WHITE);
   assert (p.getFilled(WHITE).size() == 1);
   assert (p.getFilled(BLACK).size() == 0);
   assert (p.getEmpty().size() == 361-1);

   assert (p.isEmpty(2,12) == false);
   assert (p.isEmpty(2,13) == true);

   // test clearCell.
   p.clearCell(2, 12);
   assert (p.isEmpty(2, 12) == true);

   // Test nInARow() - simple case
   p.fillCell(0,0, WHITE);
   p.fillCell(0,1, WHITE);
   assert (p.nInARow(2, WHITE) == 1);
   p.fillCell(0,2, WHITE);
   assert (p.nInARow(2, WHITE) == 0); // verify that 3-in-a-row is not also 2-in-a-row
   assert (p.nInARow(3, WHITE) == 1);
   p.fillCell(0,3, WHITE);
   assert (p.nInARow(4, WHITE) == 1);
   p.fillCell(0,4, WHITE); 
   assert (p.nInARow(5, WHITE) == 1);

   p.reset();
   assert(p.getEmpty().size() == 361);

   // Try more complicated example (and this proved an error)
   p.fillCell(0,0, WHITE);
   p.fillCell(0,1, WHITE);
   p.fillCell(0,2, BLACK);
   p.fillCell(1,2, BLACK);
   p.fillCell(0,3, WHITE);
   p.fillCell(0,4, WHITE);
   assert (p.nInARow(2, WHITE) == 2);
   assert (p.nInARow(2, BLACK) == 1);

   // Test getCaptures

   assert (p.getCaptures(BLACK) == 1);
   assert (p.getCaptures(WHITE) == 0);

   p.fillCell(1, 4, BLACK);
   p.fillCell(2, 5, BLACK);

   assert (p.getCaptures(BLACK) == 1);
   assert (p.getCaptures(WHITE) == 1);

   //   p.make_move(Vhat);
   
   cout << p.toString() << endl;


   // If we made it this far, we're golden.
   cout << "Ok." << endl;
   return 0;

}

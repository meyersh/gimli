#include <iostream>
#include "assert.h"
#include "Pente.hpp"

using namespace std;

int main() {
   Pente p;

   // Test coords
   assert (p.isValidCoords(-1,0) == false);
   assert (p.isValidCoords(0,-1) == false);
   assert (p.isValidCoords(19,0) == false);
   assert (p.isValidCoords(0,19) == false);

   assert (p.isValidCoords(0,0) == true);

   // Test colors
   assert (p.isValidColor('B') == true);
   assert (p.isValidColor('b') == true);
   assert (p.isValidColor('W') == true);
   assert (p.isValidColor('w') == true);

   assert (p.isValidColor('x') == false);

   cout << "fillCell()" << endl;
   p.fillCell(2,12, WHITE);
   cout << "search(WHITE) -> " << p.getFilled(WHITE).size() << endl;
   cout << "search(BLACK) -> " << p.getFilled(BLACK).size() << endl;
   cout << "search(EMPTY) -> (not implemented.)" << endl; 
   //<< p.getFilled(EMPTY).size() << endl;

   cout << "isEmpty(2,12) -> " << p.isEmpty(2,12) << endl;
   cout << "isEmpty(2,13) -> " << p.isEmpty(2,13) << endl;

   cout << p.toString() << endl;
   cout << p.serialize() << endl;

   ifstream infile("game.txt");
   p.deserialize(infile);
   infile.close();
   cout << p.serialize() << endl;


   ofstream outfile("game.txt");

   outfile << p.serialize() << endl;

   outfile.close();

   p.make_move(Vhat);
   
   cout << p.toString() << endl;


   // If we made it this far, we're golden.
   cout << "Ok." << endl;
   return 0;

}

#include <iostream>
#include "shauns_Pente.hpp"

using namespace std;

int main() {

   Pente p;

   cout << "fillCell() -> " << p.fillCell(2,12, 1) << endl;
   cout << "search(WHITE) -> " << p.getAll(WHITE).size() << endl;
   cout << "search(BLACK) -> " << p.getAll(BLACK).size() << endl;
   cout << "search(EMPTY) -> " << p.getAll(EMPTY).size() << endl;


   cout << p.toString() << endl;
   cout << p.serialize() << endl;

   ifstream infile("game.txt");
   p.deserialize(infile);
   infile.close();
   cout << p.serialize() << endl;


   ofstream outfile("game.txt");

   outfile << p.serialize() << endl;

   outfile.close();


   return 0;
}

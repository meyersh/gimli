#include <iostream>
#include "shauns_Pente.hpp"

using namespace std;

int main() {

   Pente p;

   cout << "fillCell() -> " << p.fillCell(2,12, 1) << endl;

   cout << p.toString() << endl;

   return 0;
}

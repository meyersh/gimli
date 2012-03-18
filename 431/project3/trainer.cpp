#include <iostream>
#include "assert.h"
#include "Pente.hpp"
#include "wlm.hpp"

using namespace std;

int main(int argc, char **argv) {

  Pente p1, p2;

  p1.players[0] = p2.players[1] = "COMPUTER";
  p1.players[1] = p2.players[0] = "OTHER";

  p1.fillCell(9,9, WHITE);
  p2.fillCell(9,9, WHITE);

  while (p1.gameOutcome(WHITE) == 0) {
	// Black moves
	p2.make_move(Vhat);

	// Copy the black move into p1
	p1.fillCell(p2.gametrace.back()->r, p2.gametrace.back()->c, BLACK);

	// White moves
	p1.make_move(Vhat);

	// Copy the white move into p2
	p2.fillCell(p1.gametrace.back()->r, p1.gametrace.back()->c, WHITE);

	// Show some progress to the screen.
	cout << ".";
	cout.flush();
	
  }

  cout << endl;

  cout << "p1.gameOutcome(WHITE) -> " << p1.gameOutcome(WHITE) << endl
	   << "whtCaps -> " << p1.whtCaps << " blkCaps -> " << p1.blkCaps << endl
	   << p2.toString() << endl << endl;

  return 0;

}

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

	// Play a game with 0 players.
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


	// Print the summary...
	cout << endl;
	cout << "p1.gameOutcome(WHITE) -> " << p1.gameOutcome(WHITE) << endl
		 << "whtCaps -> " << p1.whtCaps 
		 << " blkCaps -> " << p1.blkCaps << endl
		 << p2.toString() << endl << endl;

	// Analyze the game and adjust weights accordingly.


	/* Vtrain(b) <- V(Successor(b)) */
	/* Adjust weights with wi <- wi + n(Vtrain(b) - V(b))*xi */
	Weight weight; /* Vhat() loads the weight file automatically, but
					  we want to keep it up to date between calculations
					  so I'll use this has a pointer. */

	Pente *games[] = {&p1, &p2};
	char colors[] = {WHITE, BLACK};

	for (int i = 0; i < 2; i++) {
		cout << "Analyzing P" << i << endl;
		Pente *game = games[i];

		// Our end-game state
		State b = game->toState();
		State successor = game->toState(); 

		double error = (game->gameOutcome(colors[i])*100) - Vhat(b);
		weight.adjust(b, error);
		weight.save();

		while (game->gametrace.size()) {
			successor = game->toState();
		
			// remember + remove the last move.
			Pente::cell* tCell = game->gametrace.back();
			game->gametrace.pop_back();
			game->clearCell(tCell->r, tCell->c);

			b = game->toState();

			error = Vhat(successor) - Vhat(b);

			weight.adjust(game->toState(), error);
			weight.save();
			if (error)
				cout << "Error => " << error << endl;
		}
	   
	}

	return 0;

}

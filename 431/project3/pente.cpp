#include <iostream>
#include "assert.h"
#include "Pente.hpp"

using namespace std;

int main() {
    Pente game;

    // Test coords
    assert (game.isValidCoords(-1,0) == false);
    assert (game.isValidCoords(0,-1) == false);
    assert (game.isValidCoords(19,0) == false);
    assert (game.isValidCoords(0,19) == false);

    assert (game.isValidCoords(0,0) == true);

    // Test colors
    assert (game.isValidColor('B') == true);
    assert (game.isValidColor('b') == true);
    assert (game.isValidColor('W') == true);
    assert (game.isValidColor('w') == true);

    assert (game.isValidColor('x') == false);

    // If we made it this far, we're golden.
    cout << "Ok." << endl;
    return 0;

}

/* 
 * File:   Pente.hpp
 * Author: mab015
 * Errors introduced by Shaun Meyer
 *
 * Created on February 28, 2012, 11:27 PM
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <stdexcept>
#include "wlm.hpp"

using namespace std;

enum {
    W, NW, N, NE, E, SE, S, SW
}; // Directions
int reverse_dir[] = {E, SE, S, SW, W, NW, N, NE}; // reverse mapping

enum {
    BLACK = 'B', WHITE = 'W', EMPTY = '*'
};

class Pente {
public:

    struct cell {
        bool filled;
        char color;
        int r, c;
        cell * neighbors[8];
    };

    vector<cell*> Board;

    /* Data Members */
    int board_size;
    int turn; // turn number.
    string players[2];
    int blkCaps;
    int whtCaps;
    vector<cell*> gametrace;

    /* Member Functions */
    Pente() {
        players[0] = players[1] = "WAITING";
        board_size = 19;
        turn = 0;
        _initBoard_();
        blkCaps = whtCaps = 0;
    }

    ~Pente() {
        _killBoard_();
    }

    void reset() {
        _killBoard_();
        _initBoard_();
    }

    void _initBoard_();
    void _killBoard_();
    bool isValidCoords(int r, int c);
    bool isValidColor(char color);
    cell *getCell(int r, int c);
    cell *getCell(int i);

    bool isEmpty(int r, int c);
    void fillCell(int r, int c, char color);
    void clearCell(int r, int c);
    vector<cell*> getFilled(char color);
    int getPossible(int &possD, int &possT, int &possQ, int &possWins, char color);
    int getCertain(int &certD, int &certT, int &certQ, char color);
    int getCaptures(char color);
    int chkCapture(int r, int c, char color);
    string toString();
    string serialize();
    void deserialize(ifstream &f);
    vector<cell*> getEmpty();

    int playerNumber(string sessionid);
    char playerColor(string sessionid);
    int nInARow(int n, char color = WHITE);
    void playToken(int r, int c, char color);
    int gameOutcome(char color);


    State toState();
    State tryMove(int r, int c, char color);
    void make_move(Weight &weight);


};

void Pente::_initBoard_() {
    cell *tCell; //zombies!
    int row, col;

    for (int r = 0; r < 19; r++) {
        for (int c = 0; c < 19; c++) {
            tCell = new cell;
            tCell->r = r;
            tCell->c = c;
            tCell->filled = false;
            tCell->color = '*';
            Board.push_back(tCell);
        }
    }

    for (int i = 0; i < Board.size(); i++) {
        tCell = Board[i];
        for (int dir = 0; dir < 8; dir++) {
            row = tCell->r;
            col = tCell->c;
            switch (dir) {
                case W:
                    col--;
                    break;
                case NW:
                    row--;
                    col--;
                    break;
                case N:
                    row--;
                    break;
                case NE:
                    row--;
                    col++;
                    break;
                case E:
                    col++;
                    break;
                case SE:
                    row++;
                    col++;
                    break;
                case S:
                    row++;
                    break;
                case SW:
                    row++;
                    col--;
                    break;
            }
            if ((row < 0) || (row >= 19) || (col < 0) || (col >= 19))
                tCell->neighbors[dir] = NULL;
            else
                tCell->neighbors[dir] = Board[row * 19 + col];
        }
    }
}

void Pente::_killBoard_() {
    for (int i = 0; i < Board.size(); i++) {
        delete Board[i];
        Board[i] = NULL;
    }
    Board.clear();
    gametrace.clear();
}

bool Pente::isValidCoords(int r, int c) {
    return ((r >= 0) && (r < 19) && (c >= 0) && (c < 19));
}

bool Pente::isValidColor(char color) {
    return (toupper(color) == 'B' || toupper(color) == 'W');
}

bool Pente::isEmpty(int r, int c) {
    if (!isValidCoords(r, c))
        return false;
    return !getCell(r, c)->filled;
}

Pente::cell *Pente::getCell(int r, int c) {
    // Get cell by coords

    if (!isValidCoords(r, c))
        throw out_of_range("getCell(r,c): row or col out_of_range error.");

    return Board[r * 19 + c];
}

Pente::cell *Pente::getCell(int i) {
    // Get cell by index.

    if (i < 0 || i > Board.size())
        throw out_of_range("getCell(i): out_of_range.");

    return Board[i];
}

void Pente::fillCell(int r, int c, char color) {
    color = toupper(color);

    if (!isValidCoords(r, c))
        throw runtime_error("Cell or Row out of bounds.");

    if (isValidColor(color)) {
        getCell(r, c)->filled = true;
        getCell(r, c)->color = toupper(color);
    }
}

void Pente::clearCell(int r, int c) {

    if (!isValidCoords(r, c))
        throw runtime_error("Cell or Row out of bounds.");

    getCell(r, c)->filled = false;
    getCell(r, c)->color = '*';
}

vector<Pente::cell*> Pente::getFilled(char color) {
    cell *t;
    vector<cell*> filled;

    if (!isValidColor(color))
        throw runtime_error("Invalid color.");

    for (int i = 0; i < Board.size(); i++) {
        t = Board[i];
        if ((t->filled == true) && (t->color == toupper(color)))
            filled.push_back(t);
    }

    return filled;
}

int Pente::getPossible(int &possD, int &possT, int &possQ, int &possWins, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count = 1, blank = 0;

    if (filled.size() == 0)
        return 0;


    for (int i = 0; i < filled.size(); i++) {
        tCell = filled[i];
        for (int j = 4; j < 8; j++) {
            nxt = tCell->neighbors[j];
            while ((nxt != NULL) && (blank < 2)) {
                if ((nxt->filled == true) && (nxt->color == color))
                    count++;
                else if (nxt->filled == false)
                    blank++;
                else if ((nxt->filled == true) && (nxt->color != color))
                    break;
                nxt = nxt->neighbors[j];
            }
            if ((tCell->neighbors[j - 4] != NULL) && (tCell->neighbors[j - 4]->filled == true)) {
                ;
            } else if ((tCell->neighbors[j - 4] == NULL) || (tCell->neighbors[j - 4]->filled == false)) {
                switch (count) {
                    case 2:
                        possT++;
                        break;
                    case 3:
                        possQ++;
                        break;
                    case 4:
                        possWins++;
                        break;
                    default:
                        break;
                }
            }
            count = 1;
            blank = 0;
        }
    }

    return 0;

}

int Pente::getCertain(int &certD, int &certT, int &certQ, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count = 1;

    certD = certT = certQ = 0; // Initialize the values

    if (!isValidColor(color))
        throw runtime_error("Invalid color");

    if (filled.size() == 0)
        return 0;

    for (int i = 0; i < filled.size(); i++) {
        tCell = filled[i];
        for (int j = 4; j < 8; j++) {
            nxt = tCell->neighbors[j];
            while ((nxt != NULL) && (nxt->filled == true)) {
                if (nxt->color == color)
                    count++;
                else
                    break;
                nxt = nxt->neighbors[j];
            }

            // Skip cells we've already visited.
            if ((tCell->neighbors[j - 4] != NULL)
                    && (tCell->neighbors[j - 4]->filled == true)
                    && (tCell->neighbors[j - 4]->color == color)) {
                count = 1;
                continue;
            }

            switch (count) {
                case 2:
                    certD++;
                    break;
                case 3:
                    certT++;
                    break;
                case 4:
                    certQ++;
                    break;
                default:
                    break;
            }

            count = 1;
        }
    }

    return 0;
}

int Pente::getCaptures(char color) {
    // Check for the number of captures we can get away with
    // (WBB_ is one, for instance.

    int caps = 0;

    char opposite_color = (color == WHITE) ? BLACK : WHITE;

    for (int i = 0; i < Board.size(); i++) {
        cell *tCell = getCell(i);

        // Skip the other color.
        if (tCell->color != color)
            continue;

        // only search from E to SW since we've come from the NW.
        for (int dir = W; dir <= SW; dir++) {
            cell * cellSet[] = {tCell, NULL, NULL, NULL};
            bool setOK = true;

            for (int c = 1; c < 4 && setOK; c++) {
                if (cellSet[c - 1])
                    cellSet[c] = cellSet[c - 1]->neighbors[dir];


                if (cellSet[c] == NULL)
                    setOK = false;
            }

            if (!setOK)
                continue;

            // Require the inner cells to have pieces
            if (!cellSet[1]->filled || !cellSet[2]->filled)
                continue;

            // Require the inner cells to be matching colors
            if (cellSet[1]->color != cellSet[2]->color)
                continue;

            // Require the inner cells to be the opposite color from tCell.
            if (cellSet[1]->color != opposite_color)
                continue;

            // Require the end cell to be open.
            if (cellSet[3]->filled)
                continue;

            // By now, we have a situtation where we can capture.
            caps++;

        }

    }

    return caps;

}

int Pente::chkCapture(int r, int c, char color) {
    cell *tCell, *one, *two, *end;
    char eColor = ((color == 'B') ? 'W' : 'B');
    tCell = Board[r * 19 + c];
    int caps = 0;
    for (int j = 0; j < 8; j++) {
        one = tCell->neighbors[j];
        if ((one == NULL) || (one->filled == false) || (one->color != eColor))
            continue;
        two = one->neighbors[j];
        if ((two == NULL) || (two->filled == false) || (two->color != eColor))
            continue;
        end = two->neighbors[j];
        if ((end == NULL) || (end->filled == false))
            continue;
        else if ((end->filled == true) && (end->color == color)) {
            caps++;
            one->filled = two->filled = false;
            one->color = two->color = '*';
        }
    }

    if (color == 'B')
        blkCaps = caps;
    else
        whtCaps = caps;

    return 0;
}

string Pente::toString() {
    stringstream ss;

    for (int r = 0; r < board_size; r++) {
        for (int c = 0; c < board_size; c++) {
            char contents = getCell(r, c)->color;

            if (!getCell(r, c)->filled)
                ss << "_";
            else if (contents == WHITE)
                ss << "W";
            else if (contents == BLACK)
                ss << "B";

            ss << " ";
        }
        ss << endl;
    }

    return ss.str();

}

string Pente::serialize() {
    // A simple string serialization of the board.
    // All of the occupied cells in {W|B} <row> <col> format seems fine.

    stringstream ss;

    ss << turn << endl;
    ss << players[0] << endl;
    ss << players[1] << endl;
    ss << whtCaps << endl;
    ss << blkCaps << endl;

    for (vector<cell*>::iterator c = gametrace.begin(); c != gametrace.end(); c++) {
        char contents = (*c)->color;

        ss << (contents == BLACK ? 'B' : 'W')
                << " " << (*c)->r << " " << (*c)->c << endl;
    }

    return ss.str();
}

void Pente::deserialize(ifstream &f) {
    // read the file `f` (already open) for our serial format:
    // {W|B} <row> <col>

    string line;
    char color;
    int row, col;

    f >> turn; // first line is the turn.
    f >> players[0]; // second line is session id (or COMPUTER) of the white player.
    f >> players[1]; //third line is the session id (or COMPUTER) of the black player.
    f >> whtCaps;
    f >> blkCaps;

    while (getline(f, line)) {
        // Skipping empty lines
        if (line == "")
            continue;

        stringstream ss(line);

        ss >> color >> row >> col; // space separated...

        if (!isValidColor(color))
            continue; // invalid row.

        if (!isValidCoords(row, col))
            continue;

        // Everything is good. Insert the piece.
        fillCell(row, col, (color == 'B' ? BLACK : WHITE));
        gametrace.push_back(getCell(row,col));

    }

}

vector<Pente::cell*> Pente::getEmpty() {
    vector<cell*> emT;
    cell *tmp;

    for (int i = 0; i < Board.size(); i++) {
        tmp = Board[i];
        if (tmp->filled == false) {
            emT.push_back(tmp);
        }
    }

    return emT;
}

int Pente::playerNumber(string sessionid) {
    // Given a playerid return the index of players[]
    // or -1 for invalid id.
    for (int i = 0; i < 2; i++)
        if (players[i] == sessionid)
            return i;

    return -1;
}

char Pente::playerColor(string sessionid) {
    // Return the color char of a given player
    // (or empty for none.)
    switch (playerNumber(sessionid)) {
        case -1:
            return EMPTY;
        case 0:
            return WHITE;
        case 1:
            return BLACK;
    }
}

int Pente::nInARow(int n, char color) {
    // Return how instances a given color has of n-length in a row.

    int found = 0;

    vector<cell*> filled = getFilled(color);

    for (int i = 0; i < filled.size(); i++) {
        Pente::cell *tCell = filled[i];

        for (int dir = 4; dir < 8; dir++) {
            int count = 1;
            Pente::cell *nxt = tCell->neighbors[dir];

            while (nxt && nxt->filled) {
                if (nxt->color == color)
                    count++;
                else
                    break;
                nxt = nxt->neighbors[dir];
            }

            if (tCell->neighbors[dir - 4]
                    && tCell->neighbors[dir - 4]->filled
                    && tCell->neighbors[dir - 4]->color == color) // <-- bug fix, maybe?
                continue;

            if (count == n)
                found++;

        }
    }

    return found;

}

void Pente::playToken(int r, int c, char color) {
    int found;

    fillCell(r, c, color);
    chkCapture(r, c, color);
    found = nInARow(5, color);
    gametrace.push_back(getCell(r, c));

    turn++; // increment the move counter.
}

int Pente::gameOutcome(char color) {
    // Has a given color won, lost, or neither (yet)?
    // Return 1, -1, 0 accordingly.

    enum {
        LOST = -1, UNDETERMINED = 0, WON = 1
    };

    char theirColor = (color == WHITE) ? BLACK : WHITE;

    int myCaps, theirCaps;
    if (color == WHITE) {
        myCaps = whtCaps;
        theirCaps = blkCaps;
    } else {
        myCaps = blkCaps;
        theirCaps = whtCaps;
    }

    if (theirCaps >= 5)
        return LOST;

    if (myCaps >= 5)
        return WON;

    if (nInARow(5, color) > 0)
        return WON;
    if (nInARow(5, theirColor) > 0)
        return LOST;

    return UNDETERMINED;
}

State Pente::toState() {
    /* X variables:
       0: doubles (ours)
       1: triples (ours)
       2: quads (ours)
       3: possible captures (ours)
       
       4: doubles (theirs)
       5: triples (theirs)
       6: quads (theirs)
       7: possible captures (theirs)
       
       8: Our color (0 = white, 1 = black)
     */

    // Construct and return the state of the game.
    State s(9);
    if(playerNumber("COMPUTER")==0) {
        // Figure for white pieces...
        getCertain(s[0], s[1], s[2], WHITE);
        s[3] = getCaptures(WHITE);

        // Figure for white pieces.
        getCertain(s[4], s[5], s[6], BLACK);
        s[7] = getCaptures(BLACK);
    } else if(playerNumber("COMPUTER")==1) {
        // Figure for black pieces...
        getCertain(s[0], s[1], s[2], BLACK);
        s[3] = getCaptures(BLACK);

        // Figure for white pieces.
        getCertain(s[4], s[5], s[6], WHITE);
        s[7] = getCaptures(WHITE);
    } 
        
    //s[8] = (playerColor("COMPUTER"));

    return s;
}

State Pente::tryMove(int r, int c, char color) {
    // Fill our imagionary cell.
    fillCell(r, c, color);

    State s = toState();

    //Undo that move
    clearCell(r, c);

    return s;
}

void Pente::make_move(Weight &weight) {
    // Make a computerized move.
    srand(time(NULL));
    vector<cell*> possible_moves = getEmpty();
    random_shuffle(possible_moves.begin(), possible_moves.end());
    cell* best_move = possible_moves[0];
    // Figure out our color
    char computer_color;
    for (int i = 0; i < 2; i++)
        if (players[i] == "COMPUTER")
            computer_color = (i == 0) ? WHITE : BLACK;


    // Assume the best
    int best_state = weight.Vhat(tryMove(best_move->r, best_move->c, computer_color));

    // Compare every other move
    for (int i = 0; i < possible_moves.size(); i++) {
        // Try a move
        State fantasy_move = tryMove(possible_moves[i]->r,
                possible_moves[i]->c,
                computer_color);

        // Remember a better move
        if (weight.Vhat(fantasy_move) > best_state) {
            best_move = possible_moves[i];
            best_state = weight.Vhat(toState());
        }

    } // end for

    // Make the _BEST_ move.
    playToken(best_move->r, best_move->c, computer_color);
    
}

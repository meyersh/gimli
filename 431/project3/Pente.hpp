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
    vector< vector<cell*> > captureBuffer;

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
    int getCertainSpaces(int &D, int &T, int &Q, int &P, char color);
    int getCertain(int &certD, int &certT, int &certQ, int &certP, char color);
    int getCaptures(char color);
    int chkTotalCapture(char color);
    int chkCapture(int r, int c, char color, bool remove = false);
    int chkTotalBlocks(int &Block3, int &Block4, int &Block5, char color);
    int chkBlocks(int &Block3, int &Block4, int &Block5, char color = EMPTY);
    double getProximity(char color, int radius);
    string toString();
    string serialize();
    void deserialize(ifstream &f);
    vector<cell*> getEmpty();

    int playerNumber(string sessionid);
    char playerColor(string sessionid);
    int nInARow(int n, char color = WHITE);
    void playToken(int r, int c, char color);
    void unPlayToken();
    int gameOutcome(char color);


    State toState();    
    State toStateOld();
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
int Pente::getCertainSpaces(int &D, int &T, int &Q, int &P, char color) {
    // getCertain with a twist: we require space on either end.
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count = 1;

    D = T = Q = P = 0; // Initialize the values

    if (!isValidColor(color))
        throw runtime_error("Invalid color");

    if (filled.size() == 0)
        return 0;

    for (int i = 0; i < filled.size(); i++) {
        tCell = filled[i];
        for (int j = 4; j < 8; j++) {
            bool has_beginning_space = false;
            bool has_ending_space = false;

            // Check for leading space
            if (tCell->neighbors[j-4] && !tCell->neighbors[j-4]->filled)
                has_beginning_space = true;

            // Skip cells we've already visited.
            if ((tCell->neighbors[j - 4] != NULL)
                && (tCell->neighbors[j - 4]->filled == true)
                && (tCell->neighbors[j - 4]->color == color)) {
                count = 1;
                continue;
            }

            nxt = tCell->neighbors[j];
            while ((nxt != NULL) && (nxt->filled == true)) {
                if (nxt->color == color)
                    count++;
                else {
                    break;
                    nxt = nxt->neighbors[j];
                }
                nxt = nxt->neighbors[j];
            }
            
            if (nxt && !nxt->filled)
                has_ending_space = true;
            
            // no beginning and no ending spaces:
            if (!has_beginning_space && !has_ending_space)
                continue;

            if (nxt && (!nxt->filled || nxt->color != color))
                switch (count) {
                case 2:
                    D += has_beginning_space + has_ending_space;
                    break;
                case 3:
                    T += has_beginning_space + has_ending_space;
                    break;
                case 4:
                    Q += has_beginning_space + has_ending_space;
                    break;
                case 5:
                    P += has_beginning_space + has_ending_space;
                    break;
                default:
                    break;
                }

            count = 1;
        }
    }

    return 0;
}

int Pente::getCertain(int &certD, int &certT, int &certQ, int &certP, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count = 1;

    certD = certT = certQ = certP = 0; // Initialize the values

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
            case 5:
                certP++;
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
    // Check for the number of captures available to `color` on the board.
    // (WBB_ is one for white, for instance.

    int caps = 0;

    char opposite_color = (color == WHITE) ? BLACK : WHITE;

    for (int i = 0; i < Board.size(); i++) {
        cell *tCell = getCell(i);

        // Skip the empty cells and cells which arn't `color`.
        if (tCell->color != color || !tCell->filled)
            continue;

        // Search all directions for our CNN_ pattern
        for (int dir = W; dir <= SW; dir++) {
            cell * cellSet[] = {tCell, NULL, NULL, NULL};
            bool setOK = true;

            for (int c = 1; c < 4 && setOK; c++) {
                if (cellSet[c - 1])
                    cellSet[c] = cellSet[c - 1]->neighbors[dir];

                if (cellSet[c] == NULL) {
                    setOK = false;
                    break;
                }
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

            // By now, we have a situtation where `color` can capture.
            caps++;

        }

    }

    return caps;

}

int Pente::chkTotalCapture(char color) {
    int n = 0;

    vector<Pente::cell*> filled = getFilled(color);
    
    for (int i = 0; i < filled.size(); i++) {
        n += chkCapture(filled[i]->r, filled[i]->c, color, false);
    }

    return n;
}

int Pente::chkCapture(int r, int c, char color, bool remove) {
    /* placing a piece in r,c of color `color`, if this would
       result in any capture, record the number. If `remove` is true,
       update the scores and remove the captured pieces.

       always return the number of captures at r,c for `color`. */

    cell *tCell, *one, *two, *end;
    char eColor = ((color == 'B') ? 'W' : 'B');
    tCell = getCell(r,c);
    int caps = 0;

    captureBuffer.push_back( vector<cell*>() ); // Add a slot onto the end of captureBuffer.

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
            if(remove) {
                vector<cell*> removals;

                removals.push_back(one);
                removals.push_back(two);
                captureBuffer[turn-1]=removals;

                one->filled = two->filled = false;
                one->color = two->color = '*';
            }
        }
    }

    if (remove) 
        if (color == 'B')
            blkCaps += caps;
        else
            whtCaps += caps;

    return caps;
}

int Pente::chkTotalBlocks(int& Block3, int& Block4, int& Block5, char color) {
    // Check to see how many 3, 4, and 5 in-a-row's have been blocked.

    int fdir, bdir, f=0, b=0, tot;
    vector<cell*> filled = getFilled(color);
    cell *tCell;
    cell *nxt;

    Block3 = Block4 = Block5 = 0;

    for (int i = 0; i < filled.size(); i++) {
        tCell = getCell(i);
        for(fdir=E;fdir<=SW;fdir++) {
            bdir = fdir-4;
            //check forwards
            nxt = tCell->neighbors[fdir];
            while(nxt && (nxt->color == color) && (nxt->filled == true)) {
                f++;
                nxt = nxt->neighbors[fdir];
            }
            //check backwards
            nxt = tCell->neighbors[bdir];
            while(nxt && (nxt->color == color) && (nxt->filled == true)) {
                b++;
                nxt = nxt->neighbors[bdir];
            }
        }

        tot = f+b;  tot = (tot>5)?5:tot;
        switch(tot) {
        case 3:
            Block3++;
            break;
        case 4:
            Block4++;
            break;
        case 5:
            Block5++;
            break;
        default:
            break;
        }
    }
    return 0;
}


int Pente::chkBlocks(int& Block3, int& Block4, int& Block5, char color) {
    // Check the last move to see how many 3, 4, and 5 in-a-row's it blocked.
    if(gametrace.empty())
        return 0;
    color = (color == EMPTY) ? gametrace.back()->color  : color;
    int fdir, bdir, f=0, b=0, tot;
    cell *tCell = gametrace.back();
    cell *nxt;

    Block3 = Block4 = Block5 = 0;

    for(fdir=E;fdir<=SW;fdir++) {
        bdir = fdir-4;
        //check forwards
        nxt = tCell->neighbors[fdir];
        while(nxt && (nxt->color == color) && (nxt->filled == true)) {
            f++;
            nxt = nxt->neighbors[fdir];
        }
        //check backwards
        nxt = tCell->neighbors[bdir];
        while(nxt && (nxt->color == color) && (nxt->filled == true)) {
            b++;
            nxt = nxt->neighbors[bdir];
        }
    }

    tot = f+b;  tot = (tot>5)?5:tot;
    switch(tot) {
    case 3:
        Block3++;
        break;
    case 4:
        Block4++;
        break;
    case 5:
        Block5++;
        break;
    default:
        break;
    }

    return 0;
}

double Pente::getProximity(char color, int radius) {
	cell *tCell, *nxt;
	vector<Pente::cell*> filled = getFilled(color);
	int c = 0, prox = 0;
	
	for(int i=0;i<filled.size();i++) {
		tCell = filled[i];
		for(int dir=E;dir<=SW;dir++) {
			nxt = tCell->neighbors[dir];
			while(nxt && (c<radius)) {
				if(nxt->filled && nxt->color == color)
					prox++;
				nxt = nxt->neighbors[dir];
				c++;
			}
			c = 0;
		}
	}
	
	return (double)(prox/(filled.size()+1));
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

    ss << players[0] << endl;
    ss << players[1] << endl;

    for (vector<cell*>::iterator c = gametrace.begin(); c != gametrace.end(); c++) {
        char color = (*c)->color;

        ss << (*c)->color
           << " " << (*c)->r 
           << " " << (*c)->c << endl;
    }

    return ss.str();
}

void Pente::deserialize(ifstream &f) {
    // read the file `f` (already open) for our serial format:
    // {W|B} <row> <col>

    string line;
    char color;
    int row, col;

    f >> players[0]; // second line is session id (or COMPUTER) of the white player.
    f >> players[1]; //third line is the session id (or COMPUTER) of the black player.

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
        playToken(row, col, (color == 'B' ? BLACK : WHITE));

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

    turn++; // increment the move counter.

    fillCell(r, c, color);
    gametrace.push_back(getCell(r, c)); 

    // this will put cell* in captureBuffer[turn] for each capture.
    chkCapture(r, c, color, true); 

}

void Pente::unPlayToken() {
    // Undo the last move.
    int last_turn = turn - 1;

    // remove the piece.
    clearCell(gametrace.back()->r, gametrace.back()->c);

    // replace captured pieces.
    // TODO: change capture count BACKwards.
    if (!captureBuffer[last_turn].empty())
        {
            Pente::cell *tCell;
            for (int c = 0; c < captureBuffer[last_turn].size(); c++)
                {
                    tCell = captureBuffer[last_turn][c];
                    
                    fillCell(tCell->r, tCell->c, tCell->color);
                    
                }
        }


    // undo our memory.
    gametrace.pop_back();

    // undo that the turn ever happened...
    captureBuffer.pop_back();

    turn = last_turn;

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
    int certD, certT, certQ, certP;
    int possT, possQ, possP;
    int totCaps;
    State s(6);

    char ours, theirs;

    ours = playerColor("COMPUTER");
    theirs = (ours == WHITE) ? BLACK : WHITE;

    int ourCaps = (playerColor("COMPUTER") == WHITE) ? whtCaps : blkCaps;
    int theirCaps = (theirs == WHITE) ? whtCaps : blkCaps;

    /* Now, rationalize the cert* variables, where 
       a certD is 20% of a possible win, certT is 60%,
       certQ is 80% and certP is (of course) 100% of a win.

       with these ratios, find what % of our pieces are 
       contibuting...

    */

    // For us...
    getCertainSpaces(certD, certT, certQ, certP, ours);
    s[0] = (certD*.20+1) * (certT*.60+1) * (certQ*.80+1) * (certP*1.00+1) / (turn*.1);

    // Now do the same for THEM...
    getCertainSpaces(certD, certT, certQ, certP, theirs);
    s[1] = (certD*.20+1) * (certT*.60+1) * (certQ*.80+1) * (certP*1.00+1) / (turn*.1);

    /* In keeping with ratio idea, lets look at how many available 
       captures are available. */
    s[2] = getCaptures(ours)*.20*(ourCaps+1);
    s[3] = getCaptures(theirs)*.20*(theirCaps+1);

    // Rationalize our captures in terms of %-of-a-win.
    s[4] = ourCaps/5;
    s[5] = theirCaps/5;
    

    /* Rationalize any blocks we may consider...
     */
    /*
    chkTotalBlocks(possT, possQ, possP, ours);
    s[6] = (possT*.60+1) * (possQ*.80+1) * (possP*1.00+1);
    
    chkTotalBlocks(possT, possQ, possP, theirs);
    s[7] = (possT*.60+1) * (possQ*.80+1) * (possP*1.00+1);
    */

    /* Compare blocks with our certains */
    /*
    s[8] = s[0]*s[6]; // ours
    s[9] = s[1]*s[7]; // theirs
    */
    return s;

}

State Pente::toStateOld() {
    /* X variables:
       0: doubles (ours)
       1: triples (ours)
       2: quads (ours)
       3: pentes (ours) // useful when looking at potential moves
       4: possible captures (ours)
       5: Resulting captures from the last move (ours).
       6: Resulting 3x blocks (ours)
       7: Resulting 4x blocks (ours)
       8: Resulting 5x blocks (ours)
       9: Proximity(ours, 2)
       
       10: doubles (theirs)
       11: triples (theirs)
       12: quads (theirs)
       13: pentes (theirs) // useful when looking at potential moves
       14: possible captures (theirs)
       15: resulting captures from the last move (theirs)
       16: Resulting 3x blocks (ours)
       17: Resulting 4x blocks (ours)
       18: Resulting 5x blocks (ours)
       19: Proximity(theirs, 2)

       
    */

    // Construct and return the state of the game.
    State s(20);

    char ours, theirs;

    if (playerNumber("COMPUTER") == 0) {
        ours = WHITE;
        theirs = BLACK;
    } else {
        ours = BLACK;
        theirs = WHITE;
    }
            

    /*    if(playerNumber("COMPUTER")==0) {
     */
       
    // Figure for our pieces...
    getCertain(s[0], s[1], s[2], s[3], ours);
    s[4] = getCaptures(ours);
    s[5] = (gametrace.empty()) ? 0 : 
        chkCapture( gametrace.back()->r, gametrace.back()->c,
                    ours );

    chkBlocks(s[6], s[7], s[8], ours);

    s[9] = getProximity(ours, 2);
    
    // Figure for their pieces.
    getCertain(s[10], s[11], s[12], s[13], theirs);
    s[14] = getCaptures(theirs);
    s[15] = (gametrace.empty()) ? 0 :
        chkCapture( gametrace.back()->r, gametrace.back()->c,
                    theirs );

    chkBlocks(s[16], s[17], s[18], theirs);

    s[19] = getProximity(theirs, 2);

    /*
      } else if(playerNumber("COMPUTER")==1) {
      // Figure for black pieces...
      getCertain(s[0], s[1], s[2], s[3], BLACK);
      s[4] = getCaptures(BLACK);
      s[5] = (gametrace.empty()) ? 0 : 
      chkCapture( gametrace.back()->r, gametrace.back()->c,
      gametrace.back()->color );
      // Figure for white pieces.
      getCertain(s[6], s[7], s[8], s[9], WHITE);
      s[10] = getCaptures(WHITE);
      s[11] = gametrace.empty() ? 0 :
      chkCapture( gametrace.back()->r, gametrace.back()->c,
      gametrace.back()->color );
      } 
        
      //s[8] = (playerColor("COMPUTER"));
      */
    return s;
}

State Pente::tryMove(int r, int c, char color) {

    // Try a move
    playToken(r, c, color);
    
    // Record the state
    State s = toState();

    //Undo the fantasy.
    unPlayToken();


    return s;
}

void Pente::make_move(Weight &weight) {
    // Make a computerized move.
    // srand(time(NULL));
    vector<cell*> possible_moves = getEmpty();
    //random_shuffle(possible_moves.begin(), possible_moves.end());
    cell* best_move = possible_moves[0];
    // Figure out our color
    char computer_color = playerColor("COMPUTER");

    // Assume the best
    double best_state = weight.Vhat(tryMove(best_move->r, 
                                            best_move->c, 
                                            computer_color));

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

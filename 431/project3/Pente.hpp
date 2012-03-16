/* 
 * File:   Pente.hpp
 * Author: mab015
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

enum                {W, NW, N, NE, E, SE, S, SW}; // Directions
int reverse_dir[] = {E, SE, S, SW, W, NW, N, NE}; // reverse mapping

enum {BLACK='B', WHITE='W', EMPTY='*'};

class Pente {
    struct cell {
        bool filled;
        char color;
        int r, c;
        cell* neighbors[8];
    };

    vector<cell*> Board;

public:

	/* Data Members */
	int board_size;
	int turn; // turn number.
	string players[2];
	int blkCaps;
        int whtCaps;
	vector<cell*> gametrace;

    /* Member Functions */
    Pente() { players[0] = players[1] = "WAITING"; board_size = 19; turn=0; _initBoard_(); }
    ~Pente() { _killBoard_(); }
    void _initBoard_();
    void _killBoard_();
    bool isValidCoords(int r, int c);
    bool isValidColor(char color);
    cell *getCell(int r, int c);
	cell *getCell(int i);

	bool isEmpty(int r, int c);
    void fillCell(int r, int c, char color, bool real_move=true);
    void clearCell(int r, int c);
    vector<cell*> getFilled(char color);
    int getPossible(int &possD, int &possT, int &possQ, int &possWins, char color);
    int getCertain(int &certD, int &certT, int &certQ, char color);
    int getCaptures(int &caps, int &possCaps, char color);
    string toString();
    string serialize();
    void deserialize(ifstream &f);
	vector<cell*> getEmpty();

    State toState();
	State tryMove(int r, int c, char color);
	void make_move(int (*Vhat)(State));
	

};

void Pente::_initBoard_() {
    cell *tCell;  //zombies!
    int row, col;

    for(int r=0;r<19;r++) {
        for(int c=0;c<19;c++) {
            tCell = new cell;
            tCell->r = r;
            tCell->c = c;
            tCell->filled = false;
            tCell->color = '*';
            Board.push_back(tCell);
        }
    }

    for(int i=0;i<Board.size();i++) {
        tCell = Board[i];
        for(int dir=0;dir<8;dir++) {
            row = tCell->r;
            col = tCell->c;
            switch(dir) {
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
            if((row<0)||(row>19)||(col<0)||(col>19))
               tCell->neighbors[dir] = NULL;
            else
               tCell->neighbors[dir] = Board[row*19+col];
        }
    }
}

void Pente::_killBoard_() {
    for(int i=0;i<Board.size();i++) {
        delete Board[i];      
        Board[i] = NULL;
    }
}

bool Pente::isValidCoords(int r, int c) {
    return ((r>=0)&&(r<19)&&(c>=0)&&(c<19));
}

bool Pente::isValidColor(char color) {
    return (toupper(color) == 'B' || toupper(color) == 'W');
}

bool Pente::isEmpty(int r, int c) {
	if (!isValidCoords(r,c))
		return false;
	return !getCell(r,c)->filled;
}


Pente::cell *Pente::getCell(int r, int c) {
	// Get cell by coords

    if (!isValidCoords(r,c))
	throw out_of_range("getCell(r,c): row or col out_of_range error.");

    return Board[r*19+c];
}

Pente::cell *Pente::getCell(int i) {
	// Get cell by index.

	if (i < 0 || i > Board.size())
		throw out_of_range("getCell(i): out_of_range.");

	return Board[i];
}

void Pente::fillCell(int r, int c, char color, bool real_move) {
    color = toupper(color);
    
    if(!isValidCoords(r,c))
		throw runtime_error("Cell or Row out of bounds.");

    if(isValidColor(color)) {
		getCell(r, c)->filled = true;
		getCell(r, c)->color = toupper(color);
		if (real_move)
			gametrace.push_back(getCell(r, c));
    }
}

void Pente::clearCell(int r, int c) {

    if(!isValidCoords(r,c))
        throw runtime_error("Cell or Row out of bounds.");

    getCell(r, c)->filled = false;    
    getCell(r, c)->color = '*';
}

vector<Pente::cell*> Pente::getFilled(char color) {
    cell *t;
    vector<cell*> filled;

    if (!isValidColor(color)) 
	throw runtime_error("Invalid color.");

    for(int i=0;i<Board.size();i++) {
	t = Board[i];
	if((t->filled==true)&&(t->color==toupper(color)))
	    filled.push_back(t);
    }

    return filled;
}

int Pente::getPossible(int &possD, int &possT, int &possQ, int &possWins, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count=1, blank=0;

    if(filled.size() == 0)
        return 0;


    for(int i=0;i<filled.size();i++) {
        tCell = filled[i];
        for(int j=4;j<8;j++) {
            nxt = tCell->neighbors[j];
            while((nxt != NULL)&&(blank<2)) {
                if((nxt->filled==true)&&(nxt->color==color))
                    count++;
                else if(nxt->filled==false)
                    blank++;
                else if((nxt->filled==true)&&(nxt->color!=color))
                    break;
                nxt = nxt->neighbors[j];
            }
            if((tCell->neighbors[j-4]!=NULL)&&(tCell->neighbors[j-4]->filled==true)) {
                ;
            } else if((tCell->neighbors[j-4]==NULL)||(tCell->neighbors[j-4]->filled==false)){
                switch(count) {
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
            count = 1; blank = 0;
        }
    }

    return 0;

}

int Pente::getCertain(int &certD, int &certT, int &certQ, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count = 1;

    if (!isValidColor(color))
	throw runtime_error("Invalid color");

    if(filled.size() == 0) 
        return 0;

    for(int i=0;i<filled.size();i++) {
        tCell = filled[i];
        for(int j=4;j<8;j++) {
            nxt = tCell->neighbors[j];
            while((nxt!=NULL)&&(nxt->filled==true)) {
                if(nxt->color==color)
                    count++;
                else
                    break;
                nxt = nxt->neighbors[j];
            }
            if((tCell->neighbors[j-4]!=NULL)&&(tCell->neighbors[j-4]->filled==true)) {
                ;
            } else if((tCell->neighbors[j-4]==NULL)||(tCell->neighbors[j-4]->filled==false)){
                switch(count) {
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
            }
            count = 1;
        }
    }

    return 0;
}

int Pente::getCaptures(int &caps, int &possCaps, char color) {
    cell *tCell, *one, *two, *end;
    char eColor = ((color=='B')?'W':'B');
    vector<cell*> filled = getFilled(color);
    int count = 1;

    for(int i=0;i<filled.size();i++) {
        tCell = filled[i];
        for(int j=0;j<8;j++) {
            one = tCell->neighbors[j];
            if((one==NULL)||(one->filled==false)||(one->color!=eColor))
                continue;
            two = one->neighbors[j];
            if((two==NULL)||(two->filled==false)||(two->color!=eColor))
                continue;
            end = two->neighbors[j];
            if(end==NULL)
                continue;
            else if(end->filled==false)
                possCaps++;
            else if((end->filled==true)&&(end->color==color))
                caps++;
        }
    }

    caps = ((caps>0)?caps/2:caps);
    return 0;
}	

string Pente::toString() {
  stringstream ss;

  for (int r = 0; r < board_size; r++) {
	for (int c  = 0; c < board_size; c++) {
		char contents = getCell(r,c)->color;

		if (!getCell(r,c)->filled)
			ss << "_";
		else if (contents == WHITE)
			ss << "W";
		else if (contents == BLACK)
			ss << "B";
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

  for (vector<cell*>::iterator c = gametrace.begin(); c != gametrace.end(); c++)
	  {
		  char contents = (*c)->color;

		  ss << ( contents == BLACK ? 'B' : 'W' ) 
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
	stringstream ss(line);
	
	ss >> color >> row >> col; // space separated...

	if (!isValidColor(color))
	  continue; // invalid row.

	if (!isValidCoords(row, col))
		continue; 
	
	// Everything is good. Insert the piece.
	fillCell(row, col, (color == 'B' ? BLACK: WHITE));
	
  }

}


vector<Pente::cell*> Pente::getEmpty() {
    vector<cell*> emT;
    cell *tmp;

    for(int i=0;i<Board.size();i++) {
        tmp = Board[i];
        if(tmp->filled==false) {
            emT.push_back(tmp);
        }
    }

    return emT;
}

State Pente::toState() {
	// Construct and return the state of the game.
	State s(10);

	// Figure for black pieces...
	getCertain(s[0], s[1], s[2], BLACK);
	getCaptures(s[3], s[4], BLACK);

	// Figure for white pieces.
	getCertain(s[5], s[6], s[7], WHITE);
	getCaptures(s[8], s[9], WHITE);
	
	return s;
}

State Pente::tryMove(int r, int c, char color) {
	// Fill our imagionary cell.
	fillCell( r, c, color, false );
	
	State s = toState();
	
	//Undo that move
	clearCell(r,c);

	return s;
}

void Pente::make_move(int (*Vhat)(State)) {
// Make a computerized move.
	vector<cell*> possible_moves = getEmpty();
        random_shuffle(possible_moves.begin(), possible_moves.end());
	cell* best_move = possible_moves[0];
	// Figure out our color
	char computer_color;
	for (int i = 0; i < 2; i++)
		if (players[i] == "COMPUTER")
			computer_color = (i == 0) ? WHITE : BLACK;


	// Assume the best 
	int best_state = Vhat( tryMove(best_move->r, best_move->c, computer_color) );	
	
    // Compare every other move
	for (int i = 0; i < possible_moves.size(); i++) {
		// Try a move
		State fantasy_move = tryMove( possible_moves[i]->r, 
									  possible_moves[i]->c,
									  computer_color);

		// Remember a better move
		if ( Vhat(fantasy_move) > best_state) {
			best_move = possible_moves[i];
			best_state = Vhat( toState() );
		}

	} // end for

	// Make the _BEST_ move.
	fillCell( best_move->r, best_move->c, computer_color );

}

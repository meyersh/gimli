/* 
 * File:   Pente.hpp
 * Author: mab015
 *
 * Created on February 28, 2012, 11:27 PM
 */
#include <iostream>
#include <iomanip>
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

    /* Member Functions */
    Pente() { players[0] = players[1] = "WAITING"; board_size = 19; turn=0; _initBoard_(); }
    ~Pente() { _killBoard_(); }
    void _initBoard_();
    void _killBoard_();
    bool isValidCoords(int r, int c);
    bool isValidColor(char color);
    cell *getCell(int r, int c);
	cell *getCell(int i);

    void fillCell(int r, int c, char color);
    void clearCell(int r, int c);
    vector<cell*> getFilled(char color);
    int getPossible(int &possD, int &possT, int &possQ, int &possWins, char color);
    int getCertain(int &certD, int &certT, int &certQ, char color);

	string toString();
	string serialize();
	void deserialize(ifstream &f);
	State toState();

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

void Pente::fillCell(int r, int c, char color) {
    color = toupper(color);
    
    if(!isValidCoords(r,c))
		throw runtime_error("Cell or Row out of bounds.");

    if(isValidColor(color)) {
		getCell(r, c)->filled = true;
		getCell(r, c)->color = toupper(color);
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
    int count, blank;

    if(filled.size() == 0)
        return 0;


    for(int i=0;i<filled.size();i++) {
        tCell = filled[i];
        for(int j=4;i<8;i++) {
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
    }

    return 0;

}

int Pente::getCertain(int &certD, int &certT, int &certQ, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled = getFilled(color);
    int count;

    if (!isValidColor(color))
	throw runtime_error("Invalid color");

    if(filled.size() == 0) 
        return 0;

    for(int i=0;i<filled.size();i++) {
        tCell = filled[i];
        for(int j=4;i<8;i++) {
            nxt = tCell->neighbors[j];
            while((nxt!=NULL)&&(nxt->filled==true)) {
                if(nxt->color==color)
                    count++;
                else
                    break;
                nxt = nxt->neighbors[j];
            }
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
    }

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

  for (int r = 0; r < board_size; r++)
	for (int c = 0; c < board_size; c++)
	  {
		  if (!getCell(r,c)->filled)
			  continue;

		  char contents = getCell(r,c)->color;

		  ss << ( contents == BLACK ? 'B' : 'W' ) 
			 << " " << r << " " << c << endl;
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

State Pente::toState() {
	// Construct and return the state of the game.
	State s;

	return s;
}

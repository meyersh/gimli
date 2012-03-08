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
#include "wlm.hpp"

using namespace std;

enum {N, NE, E, SE, S, SW, W, NW};
int inverse_dir[] = {S, SW, W, NW, N, NE, E, SE}; // inverse mapping

enum {EMPTY, WHITE, BLACK}; // Cell contents

class Pente {
  struct cell {
	int r, c;
	

	cell *CL, *TL, *TC, *TR; // Not sure what
	cell *CR, *BR, *BC, *BL; // these are for. 

	cell *neighbors[8]; // Cardinal directions, NW, NE, etc.
	int contents; // contain one of {EMPTY, WHITE, BLACK}

	cell() {
	  // initialize neighbors
	  for (int i = 0; i < 8; i++)
		neighbors[i] = NULL;

	  // initially empty.
	  contents = EMPTY;
	}

  };

  vector<vector<cell> > Board;

public:

  int turn;
  string white_player;
  string black_player;

  Pente() {
	Board.resize(19);
	for (int i = 0; i < Board.size(); i++)
	  Board[i].resize(19);

	_initBoard_(); 

	turn = EMPTY;
	white_player = black_player = "WAITING";
}
  ~Pente() {};
  void _initBoard_();

  int fillCell(int r, int c, int contents);
  int clearCell(int r, int c);
  vector<cell*> getAll(int search);
  string toString();
  string serialize();
  void deserialize(ifstream &f);
  State toState();

};

void Pente::_initBoard_() {
  cell *tCell;  //zombies!
  int row, col;

  for(int r=1;r<Board.size()-1;r++) {
	for(int c=1;c<Board.size()-1;c++) {
	  tCell = &Board[r][c]; 

	  tCell->r = r;
	  tCell->c = c;

	  tCell->neighbors[S]  = &Board[r][c-1];
	  tCell->neighbors[N]  = &Board[r][c+1];
	  tCell->neighbors[NW] = &Board[r-1][c-1];
	  tCell->neighbors[W]  = &Board[r-1][c];
	  tCell->neighbors[NE] = &Board[r-1][c+1];
	  tCell->neighbors[SW] = &Board[r+1][c-1];
	  tCell->neighbors[E]  = &Board[r+1][c];
	  tCell->neighbors[SE] = &Board[r+1][c+1];

	  // Map the reverse back to us. This is a little silly but 
	  // catches the sides + edges.
	  for (int dir = 0; dir < 8; dir++)
		tCell->neighbors[dir]->neighbors[inverse_dir[dir]] = tCell;
		  
	}
  }
}

int Pente::fillCell(int r, int c, int contents) {
  if((r<0)||(r>=Board.size())||(c<0)||(c>=Board.size()))
	return 1;
  if(contents != EMPTY && contents != WHITE && contents != BLACK)
	return 2;

  Board[r][c].contents = contents;
  return 0;
}

int Pente::clearCell(int r, int c) {
  int idx;

  if((r<0)||(r>=Board.size())||(c<0)||(c>=Board.size()))
	return 1;

  Board[r][c].contents = EMPTY;

  return 0;
}

vector<Pente::cell*> Pente::getAll(int search) {
  // Return cells matching a search (EMPTY, WHITE, BLACK).
  vector<Pente::cell*> results;

  if(search != WHITE && search != BLACK && search != EMPTY)
	return results;
  
  for(int r=0;r<Board.size();r++) 
	for (int c=0;c<Board.size();c++)
	  if (Board[r][c].contents == search)
		results.push_back( &Board[r][c] );
  
  return results;
}

string Pente::toString() {
  stringstream ss;

  for (int r = 0; r < Board.size(); r++) {
	for (int c  = 0; c < Board.size(); c++) {
	  int contents = Board[r][c].contents;

	  if (contents == EMPTY)
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
  ss << white_player << endl;
  ss << black_player << endl;

  for (int r = 0; r < Board.size(); r++)
	for (int c = 0; c < Board.size(); c++)
	  {
		int contents = Board[r][c].contents;
		if (contents) // non-zero for WHITE or BLACK...
		  ss << ( contents == BLACK ? 'B' : 'W' ) 
			 << " " << r << " " << c << endl;
	  }

  return ss.str();
}

void Pente::deserialize(ifstream &f) {
  // read the file `f` (already open) for our serial format:
  // {W|B} <row> <col>

  string line;
  string color;
  int row, col, contents;

  f >> turn; // first line is the turn.
  f >> white_player; // second line is session id (or COMPUTER) of the white player.
  f >> black_player; //third line is the session id (or COMPUTER) of the black player.

  while (getline(f, line)) {
	stringstream ss(line);
	
	ss >> color >> row >> col; // space separated...

	if (color != "W" && color != "B")
	  continue; // invalid row.

	contents = (color == "B" ? BLACK : WHITE);

	if (row < 0 || row >= Board.size())
	  continue; // invalid row.

	if (col < 0 || col >= Board.size())
	  continue; // invalid col.

	
	// Everything is good. Insert the piece.
	Board[row][col].contents = contents;
	
  }

}


State Pente::toState() {
  // Construct and return the state of the game.
  State s;
  s.insert(getAll(EMPTY).size());

  return s;
}

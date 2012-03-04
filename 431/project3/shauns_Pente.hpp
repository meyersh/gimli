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


	cell *CL, *TL, *TC, *TR;
	cell *CR, *BR, *BC, *BL;

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

  Pente() {
	Board.resize(19);
	for (int i = 0; i < Board.size(); i++)
	  Board[i].resize(19);

	_initBoard_(); 
}
  ~Pente() {};
  void _initBoard_();

  int fillCell(int r, int c, int contents);
  int clearCell(int r, int c);
  vector<cell*> getAll(int search);
  string toString();

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

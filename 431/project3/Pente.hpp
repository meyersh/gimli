/* 
 * File:   Pente.hpp
 * Author: mab015
 *
 * Created on February 28, 2012, 11:27 PM
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <stdexcept>
#include "wlm.hpp"

using namespace std;

class Pente {
    struct cell {
        bool filled;
        char color;
        int r, c;
        cell* neighbors[8];
    };

    vector<cell*> Board;
public:
    Pente() { _initBoard_(); }
    ~Pente() { _killBoard_(); }
    void _initBoard_();
    void _killBoard_();
    bool isValidCoords(int r, int c);
    bool isValidColor(char color);
    cell *getCell(int r, int c);
    void fillCell(int r, int c, char color);
    void clearCell(int r, int c);
    vector<cell*> getFilled(char color);
    int getPossible(int &possD, int &possT, int &possQ, int &possWins, char color);
    int getCertain(int &certD, int &certT, int &certQ, char color);

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
                case 0:
                   col--;
                   break;
                case 1:
                   row--;
                   col--;
                   break;
                case 2:
                   row--;
                   break;
                case 3:
                   row--;
                   col++;
                   break;
                case 4:
                   col++;
                   break;
                case 5:
                   row++;
                   col++;
                   break;
                case 6:
                   row++;
                   break;
                case 7:
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
    return Board[r*19+c];
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



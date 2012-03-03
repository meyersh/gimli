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
//#include "wlm.hpp"

using namespace std;

class Pente {
    struct cell {
        bool filled;
        char color;
        int r, c;
        cell *CL, *TL, *TC, *TR;
        cell *CR, *BR, *BC, *BL;
    };

    vector<cell> Board;

    Pente() { _initBoard_(); }
    ~Pente();
    void _initBoard_();
    int fillCell(int r, int c, char color);
    int clearCell(int r, int c);
    int getFilled(char color, vector<cell*> &filled);

};

void _initBoard_() {
    cell tCell;  //zombies!
    int row, col;

    for(int r=0;r<19;r++) {
        for(int c=0;c<19;c++) {
            tCell.r = r;
            tCell.c = c;
            tCell.filled = false;
            tCell.color = '*';
            Board.push_back(tCell);
        }
    }

    for(int i=0;i<Board.size();i++) {
        tCell = Board[i];
        for(int dir=0;dir<8;dir++) {
            switch(dir) {
                case 0:
                   row = tCell.r;
                   col = tCell.c-1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.CL = NULL;
                   else
                       tCell.CL = Board[row*19+col];
                   break;
                case 1:
                   row = tCell.r-1;
                   col = tCell.c-1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.TL = NULL;
                   else
                       tCell.TL = Board[row*19+col];
                   break;
                case 2:
                   row = tCell.r-1;
                   col = tCell.c;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.TC = NULL;
                   else
                       tCell.TC = Board[row*19+col];
                   break;
                case 3:
                   row = tCell.r-1;
                   col = tCell.c+1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.TR = NULL;
                   else
                       tCell.TR = Board[row*19+col];
                   break;
                case 4:
                   row = tCell.r;
                   col = tCell.c+1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.CR = NULL;
                   else
                       tCell.CR = Board[row*19+col];
                   break;
                case 5:
                   row = tCell.r+1;
                   col = tCell.c+1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.BR = NULL;
                   else
                       tCell.BR = Board[row*19+col];
                   break;
                case 6:
                   row = tCell.r+1;
                   col = tCell.c;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.BC = NULL;
                   else
                       tCell.BC = Board[row*19+col];
                   break;
                case 7:
                   row = tCell.r+1;
                   col = tCell.c-1;
                   if((row<0)||(row>19)||(col<0)||(col>19))
                       tCell.BL = NULL;
                   else
                       tCell.BL = Board[row*19+col];
                   break;
            }
        }
    }
}

int Pente::fillCell(int r, int c, char color) {
    int idx;
    if((row<0)||(row>19)||(col<0)||(col>19))
        return 0;
    if((toupper(color)!='B')||(toupper(color)!='W'))
        return 0;
    idx = r*19+c;
    Board[idx].filled = true;
    Board[idx].color = color;
    return 1;
}

int Pente::clearCell(int r, int c) {
    int idx;
    if((row<0)||(row>19)||(col<0)||(col>19))
        return 0;
    idx = r*19+c;
    Board[idx].filled = false;
    Board[idx].color = '*';
    return 1;
}

int Pente::getFilled(char color, vector<cell*> &filled) {
    cell *t;

    if((toupper(color)!='B')||(toupper(color)!='W'))
        return 0;
    for(int i=0;i<Board.size();i++) {
        t = Board[i];
        if((t->filled==true)&&(t->color==toupper(color)))
            filled.push_back(t);
    }

    return 1;
}
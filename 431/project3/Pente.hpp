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
        cell* neighbors[8];
    };

    vector<cell*> Board;
public:
    Pente() { _initBoard_(); }
    ~Pente() { _killBoard_(); }
    void _initBoard_();
    void _killBoard_();
    int fillCell(int r, int c, char color);
    int clearCell(int r, int c);
    int getFilled(char color, vector<cell*> &filled);
    int getPossible(int &possD, int &possT, int &possQ, int &possWins, char color);
    int getCertain(int &certD, &certT, int &certQ, char color);

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

int Pente::fillCell(int r, int c, char color) {
    int idx;
    color = toupper(color);
    if((r<0)||(r>19)||(c<0)||(c>19))
        return 0;
    if((color=='B')||(color=='W')) {
        idx = r*19+c;
        Board[idx]->filled = true;
        Board[idx]->color = color;
        return 1;
    }
    return 0;
}

int Pente::clearCell(int r, int c) {
    int idx;
    if((r<0)||(r>19)||(c<0)||(c>19))
        return 0;
    idx = r*19+c;
    Board[idx]->filled = false;
    Board[idx]->color = '*';
    return 1;
}

int Pente::getFilled(char color, vector<cell*> &filled) {
    cell *t;
    color = toupper(color);
    cout << color << endl;

    if((color=='B')||(color=='W')) {
        for(int i=0;i<Board.size();i++) {
            t = Board[i];
            if((t->filled==true)&&(t->color==toupper(color)))
                filled.push_back(t);
        }
        return 1;
    }
    return 0;
}

int Pente::getPossible(int &possT, int &possQ, int &possWins, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled;
    int count, blank;

    if(!getFilled(color, filled)) {
        cout << "ERROR:  Invalid color: getVars()" << endl;
        return 0;
    }

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

    return 1;

}

int Pente::getCertain(int &certD, &certT, int &certQ, char color) {
    cell *tCell, *nxt;
    vector<cell*> filled;
    int count;

    if(!getFilled(color, filled)) {
        cout << "ERROR:  Invalid color: getVars()" << endl;
        return 0;
    }

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

    return 1;
}

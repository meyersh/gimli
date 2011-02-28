// mcsci.hpp
// Randy Campbell
// Classes for running an mcsci virtual machine
#ifndef MCSCI_HPP
#define MCSCI_HPP	

#include <string>
#include <vector>

using namespace std;

//*** SIZE LIMITS
#define MCSTORESZ 1023
#define GENREGSZ  8
#define MAINMEMSZ 0X01000000

//*** DETERMINE WHETHER OR NOT TO LOG THE CYCLE (COMMENT OUT #define TO NOT LOG)
#define LOGCYCLE
int MCSCI_DoCycle(
#ifdef LOGCYCLE
string &log,
const vector<int> &primMem
#endif
);

void MCSCI_LoadMicroProgram(const vector <unsigned long long> &mProg);

void MCSCI_SetPC(unsigned newVal); 
void MCSCI_SetIR(unsigned newVal);

void MCSCI_SetGregs(const vector<int> &regVals);

void MCSCI_SetMemLoc(int addr, int val);

#endif


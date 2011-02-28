#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "mcsci.hpp"

using namespace std;

main()
{ string ibuf, // Input buffer
         logStr; // String holding logging information
  vector <unsigned long long> microProg;
  vector <int> regs;
  vector <int> memView;

//*** DO THE CONTENT TYPE HEADER (BEFORE I FORGET TO DO IT :)
cout << "Content-Type: text/plain\n\n" << endl;

//*** READ IN DATA AND USE IT TO SET UP THE PROCESSOR
//****** READ MICROINSTRUCTION FOR TESTING AND STORE IT
getline(cin, ibuf); 
microProg.push_back(strtoull(ibuf.c_str(), NULL, 16));
MCSCI_LoadMicroProgram(microProg);

//****** READ AND STORE PC
getline(cin, ibuf); 
MCSCI_SetPC((unsigned) strtoul(ibuf.c_str(), NULL, 10));

//****** READ AND STORE IR
getline(cin, ibuf); 
MCSCI_SetIR((unsigned) strtoul(ibuf.c_str(), NULL, 16));

//****** READ AND STORE GENERAL REGISTERS
for (int i = 0; i < GENREGSZ; i++)
   {
   getline(cin, ibuf); 
	regs.push_back((int) strtol(ibuf.c_str(), NULL, 10));
	MCSCI_SetGregs(regs);
   }

//****** READ MEMORY LOC AND VALUE, STORE VALUE & SAVE LOC FOR INSPECTION
getline(cin, ibuf); 
memView.push_back((int) strtol(ibuf.c_str(), NULL, 10));

getline(cin, ibuf);
MCSCI_SetMemLoc(memView[0], (unsigned) strtoul(ibuf.c_str(), NULL, 10));

//****** READ ADDITIONAL MEMORY LOC FOR INSPECTION & SAVE IT
getline(cin, ibuf); 
memView.push_back((int) strtol(ibuf.c_str(), NULL, 10));


//*** RUN SIMULATION AND GET RESULTS
MCSCI_DoCycle(logStr, memView);

//*** OUTPUT RESULTS
cout << logStr << endl;
}


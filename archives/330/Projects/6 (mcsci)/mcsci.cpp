// mcsci.cpp
// Randy Campbell
// Class for running an mcsci virtual machine

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <sstream>

#include "mcsci.hpp"

using namespace std;

//*** REGISTER INDEXES
enum { R0, R1, R2, R3, R4, R5, R6, R7, NUMREG};

//*** MASKS
#define MICROADDRMASK	0X000003FF
#define GENOADDRMASK		0X00FFFFFF
#define MIRMASK4			0X000000000000000FULL
#define MIRMASK2			0X0000000000000003ULL
#define MIRMASK20			0X00000000000FFFFFULL
#define MIRMASK8			0X00000000000000FFULL
#define ZONEMASK3			0X00000007
#define ZONEMASK2			0X00000003
#define BUSMASK24			0X00FFFFFF
#define BUSMASK32			0XFFFFFFFF
#define IRCHKMASK       0X80000000
#define IRSHRTMASK      0XF8000000
#define IRLONGMASK      0XFF000000
#define ADDRTESTMASK    0X00000200


//*********************************************
// Machine components
//*********************************************

//*** DATAPATH
static int genRegs[NUMREG], //*** General registers
    		  aBus,
    		  bBus,
    		  cBus,
    		  mdb,
    		  mab;
    		  
static unsigned pc, ir;

//*** MICROCONTROL
static unsigned long long mcStore[MCSTORESZ], //*** Microprogram store
                          mir;					 //*** Microinstruction Register
static unsigned mpc = 0;                        	 //*** Microprogram counter
           
static long long maxRunCycles = 50,						//*** CPU cycle limit			
                 curCycles = 0;							//*** Current CPU cycle

//*** GENERAL SYSTEM INFO
static int mainMem[MAINMEMSZ];          	//*** Main Memory
static char loaded = 0;                  	//*** If NZ then data has been loaded


//*********************************************
//*********************************************
// Private functions
//*********************************************
//*********************************************

//*********************************************
// Support Functions for each phase of executing one micro instruction
//*********************************************

static void ValueToBus(int &bus, unsigned srcField, unsigned busMask)
/* 
Uses the src field to determine what value to place on the bus given as the
first parameter
bus: bus receiving the value
srcField: Source field from microinstruction indicating source of the bus value
busMask: All ones if A, B or MDB, only ones in 24 rightmost bits if MAB
*/
{ unsigned zoneSelect;

if (srcField < 8) //*** BUS VALUE FROM THE REGISTER FILE
   bus = genRegs[srcField] & busMask;
else if (srcField == 14)
   bus = ir & busMask;
else if (srcField == 15)
   bus = pc & busMask;
else if (srcField == 12)
   bus = 0;
else if (srcField == 13)
   bus = 1;
else //*** BUS VALUE FROM THE REGISTER FILE INDEXED BY ir ZONE
   {
   zoneSelect = srcField & ZONEMASK2;
   bus = genRegs[(ir >> (15 + (zoneSelect * 3))) & ZONEMASK3] & busMask;
   }
}

//**************************************************************************
//**************************************************************************

#define ENA  0X00000008
#define ENB  0X00000004
#define INVA 0X00000002
#define FUNC 0X00000003
#define SIGNCHECK 0X80000000

static int ALUShift(unsigned aIn, unsigned bIn, unsigned aluControl)
/* 
Does ALU and Shift work, returns output of ALU and Shifter
bus: bus receiving the value
srcField: Source field from microinstruction indicating source of the bus value
busMask: All ones if A, B or MDB, only ones in 24 rightmost bits if MAB
*/
{ unsigned temp,
           output;

//*** HANDLE INPUTS (ENABLED/NOT ENABLED AND WHETHER OR NOT A IS INVERTED)
if (!(aluControl & ENA)) //*** NOT ENABLED MEANS 0
   aIn = 0;
if (!(aluControl & ENB)) //*** NOT ENABLED MEANS 0 
   bIn = 0;
if (aluControl & INVA) //*** INVERT THE A INPUT
	aIn = ~aIn;

//*** DO ALU CALCULATION
switch ((aluControl >> 4) & FUNC)
   {
   case 0: //*** A AND B
      output = aIn & bIn;
      break;
      
   case 1: //*** A OR B
      output = aIn | bIn;
      break;
      
   case 2: //*** NOT B
      output = ~bIn;
      break;
      
   case 3: //*** A + B
      output = aIn + bIn + (aluControl & 1); //*** Last is INC bit
      break;
   
   default: break;
   } //---END SWITCH
   
//*** DO SHIFT
switch ((aluControl >> 4) & FUNC)
   {
   case 1: //*** ASR
      temp = output & 0x80000000;
      output >>= 1;
      output |= temp;      
      break;
      
   case 2: //*** LSL
      output <<= 1;
      break;
      
   case 3: //*** LSR
      output >>= 1;
      break;
   
   default: break;
   } //---END SWITCH
   
return (int) output;
}

//*********************************************
// Functions for each phase of executing one micro instruction
//*********************************************

static void Phase1(unsigned long long &tempmir)
//*** LOAD NEW MICROINSTRUCTION AND BEGIN EXECUTION BY PLACING VALUES ON THE
//    A, B, MAB, AND (POSSIBLY) MDB
//    tempmir INITIAL HOLDS A COPY OF THE MIR BUT PIECES ARE REMOVED DURING USE
{ unsigned tempSrc;

//*** LOAD NEW MICROINSTRUCTION, tempmir HOLDS VALUE THAT GETS DESTRUCTED IN USE
tempmir = mir = mcStore[mpc];

//*** VALUES ON THE A, B, MAB, AND (POSSIBLY) MDB)
tempSrc = (unsigned int) tempmir & MIRMASK4; //*** A-BUS
tempmir >>= 4;
ValueToBus(aBus, tempSrc, BUSMASK32);

tempSrc = (unsigned int) tempmir & MIRMASK4; //*** B-BUS
tempmir >>= 4;
ValueToBus(bBus, tempSrc, BUSMASK32);

tempSrc = (unsigned int) tempmir & MIRMASK4; //*** MDB
tempmir >>= 4;
ValueToBus(mdb, tempSrc, BUSMASK32); //*** THIS IS OVERWRITTEN IF MEMORY READ

tempSrc = (unsigned int) tempmir & MIRMASK4; //*** MAB
tempmir >>= 4;
ValueToBus(mab, tempSrc, BUSMASK24); 
} 

//**************************************************************************
//**************************************************************************

static void IntermediateStore(unsigned long long &tempmir,
                       unsigned &primMem, 
					   unsigned &registerRead, 
					   unsigned &zoneRead)
//*** STORE VALUES NEEDED IN LAST PART OF CYCLE BEFORE DOING MIDDLE PART
{
primMem = (unsigned int) tempmir & MIRMASK2;
tempmir >>= 2;

registerRead = (unsigned int) tempmir & MIRMASK20;
tempmir >>= 20;

zoneRead = (unsigned int) tempmir & MIRMASK4;
tempmir >>= 4;
}

//**************************************************************************
//**************************************************************************

static void Phase2(unsigned long long &tempmir,
                      unsigned  primMem)
//*** PART 2 OF THE CYCLE: ALU & SHIFTER DO THEIR WORK, RESULT ON C BUS
//    IF READING, VALUE FROM MEM ACCESS IF INDICATED
{
cBus = ALUShift(aBus, bBus, tempmir & MIRMASK8); //*** ALU WORK
tempmir >>= 8; //*** JMP BITS RIGHT MOST AT THIS POINT

if (primMem & 2) //*** MEMORY ACCESS TO DO
	if (primMem & 1) //*** DO WRITE
	   mainMem[mab & BUSMASK24] = mdb;
	else //*** DO READ
	   mdb = mainMem[mab & BUSMASK24];

//*** PART 2 OF THE CYCLE: LAST PART... NEW VALUE IN MPC
mpc = tempmir >> 4; //*** SET INITIAL NEXT VALUE OF mpc FOR FUTURE USE
if (tempmir & 1) //*** PART OF ADDRESS COMES FROM THE IR
   {
   if (ir & IRCHKMASK) //*** LONG VERSION
      mpc = mpc | ((ir & IRLONGMASK) >> 23);
   else //*** SHORT VERSION
      mpc = mpc | ((ir & IRSHRTMASK) >> 26);
   }
    
if (!((tempmir >> 4) & ADDRTESTMASK) && 
      (((cBus < 0) && (tempmir & 8)) || ((cBus == 0) && (tempmir & 4)) || 
      ((cBus > 0) && (tempmir & 2)))) //*** SET LEFTMOST BIT TO 1
   mpc |= 512;
}

//**************************************************************************
//**************************************************************************

static void Phase3(unsigned long long &tempmir,
                      unsigned  zoneRead,
					  unsigned  registerRead)
//*** PART 3 OF THE CYCLE: RETRIEVE VALUES FROM C BUS AND MDB
{ unsigned tmask;

for (int i = 0; i < 4; i++)  //*** CHECK ZONE READS
	if (zoneRead & (1 << i)) //*** THIS ZONE DESIGNATES A GEN REG TO RECEIVE VAL
	   registerRead |= 2 << (2 * (ir >> (15 + (i * 3))) & ZONEMASK3);
	   
for (int i = 0, tmask = 2; i < 10; i++, tmask <<= 2)
   if (tmask & registerRead) //*** REGISTER RETAINS VALUE OR
                             //    GETS VALUE FROM C-BUS OR MDB
      {
      if (i < 8) //*** GEN REG
         genRegs[i] = ((tmask >> 1) & registerRead) ? mdb : cBus;
      else if (i == 8) //*** IR
         ir = (((tmask >> 1) & registerRead) ? mdb : cBus) & BUSMASK24;
      else //*** PC
      	pc = (((tmask >> 1) & registerRead) ? mdb : cBus) & BUSMASK24;
      }
}

//*********************************************
// Template and Functions to help with logging
//*********************************************

template <class A>
static void writeItem(ostream &ofl, 
                 const string &label, 
                             A item, 
                           int labIndex = -1)
{
ofl << label;
if (labIndex > -1)
   ofl << '[' << labIndex << ']';
ofl << ": " << item << " [HEX: " << hex << item <<"]" << dec;
}

//**************************************************************************
//**************************************************************************

static void writeRegisters(ostream &ofl)
{
writeItem(ofl, "MPC", mpc);
ofl << endl;

writeItem(ofl, "MIR", mir);
ofl << endl;

writeItem(ofl, "PC", pc);
ofl << endl;

writeItem(ofl, "IR", ir);
ofl << endl;

for (int i = 0; i < NUMREG; i++)
   {
   writeItem(ofl, "GR", genRegs[i], i);
   ofl << endl;
   }
}


//**************************************************************************
//**************************************************************************

static void writeBuses(ostream &ofl)
{
writeItem(ofl, "Abus", aBus);
ofl << endl;

writeItem(ofl, "Bbus", bBus);
ofl << endl;

writeItem(ofl, "Cbus", cBus);
ofl << endl;

writeItem(ofl, "MDB", mdb);
ofl << endl;

writeItem(ofl, "MAB", mab);
ofl << endl;
}

//**************************************************************************
//**************************************************************************

static void writeMemoryLocs(ostream &ofl, vector<int> locations)
{
for (int i = 0; i < locations.size(); i++)
   {
   writeItem(ofl, "PriMem", mainMem[locations[i]], locations[i]);
   ofl << endl;
   }
}

//*********************************************
//*********************************************
// Public functions
//*********************************************
//*********************************************

#define Log

int MCSCI_DoCycle(
#ifdef LOGCYCLE
string &log,
const vector<int> &pMem
#endif
)
{
unsigned long long tempmir;    // temp holding microinstruction
                   
unsigned      registerRead,    // register read fields
              zoneRead,        // zone read bits
              primMem;         // primary memory access bits   

#ifdef LOGCYCLE
ostringstream logHere;

logHere << "CYCLE LOG\n\n\n\nBefore Phase 1...\n\n";
writeRegisters(logHere);
writeBuses(logHere);
writeMemoryLocs(logHere, pMem);
#endif        

Phase1(tempmir);
IntermediateStore(tempmir, primMem, registerRead, zoneRead);

#ifdef LOGCYCLE
logHere << "\n\nBetween Phase 1 & Phase 2 (after intermediate store)...\n\n";
writeRegisters(logHere);
writeBuses(logHere);
writeMemoryLocs(logHere, pMem);
#endif    

Phase2(tempmir, primMem);

#ifdef LOGCYCLE
logHere << "\n\nBetween Phase 2 & Phase 3...\n\n";
writeRegisters(logHere);
writeBuses(logHere);
writeMemoryLocs(logHere, pMem);
#endif    

Phase3(tempmir, zoneRead, registerRead);

#ifdef LOGCYCLE
logHere << "\n\nAfter Phase 3...\n\n";
writeRegisters(logHere);
writeBuses(logHere);
writeMemoryLocs(logHere, pMem);

log = logHere.str();
#endif 
}

//*********************************************
// SET (ALSO CALLED LOAD FOR MULTI-VALUED INPUT) & GET FUNCTIONS
//*********************************************

void MCSCI_LoadMicroProgram(const vector <unsigned long long> &mProg)
{ int lim = MCSTORESZ;

if (mProg.size() < lim)
   lim = mProg.size();
   
for (int i = 0; i < lim; i++)
   mcStore[i] = mProg[i];
}
//**************************************************************************
//**************************************************************************

void MCSCI_SetPC(unsigned newVal){pc = newVal;} 
//**************************************************************************
//**************************************************************************

void MCSCI_SetIR(unsigned newVal){ir = newVal;}
//**************************************************************************
//**************************************************************************

void MCSCI_SetGregs(const vector<int> &regVals)
{ int lim = GENREGSZ;

if (regVals.size() < lim)
   lim = regVals.size();
   
for (int i = 0; i < lim; i++)
   genRegs[i] = regVals[i];
}
//**************************************************************************
//**************************************************************************

void MCSCI_SetMemLoc(int addr, int val)
{ 

if ((-1 < addr) && (addr < MAINMEMSZ))
   mainMem[addr] = val;
}



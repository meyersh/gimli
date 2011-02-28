#include <iostream>
#include <fstream>

using namespace std;

#define SEEDSIZE 8
   
void randomString(string &rstr, int slen, const string &basechars)
{ unsigned long param;
  char seed[SEEDSIZE];
  ifstream randSource;
  int i;

// *** GET SEED VALUE  
randSource.open("/dev/urandom", ios::in|ios::binary);
randSource.read(seed, SEEDSIZE);
randSource.close();

// *** SET SEED
param = 0L;
for (i = 0; i < SEEDSIZE; i++)
   {
   param <<= 8;
   param += seed[i];
   } // -- END FOR
   
// *** GENERATE THE RANDOM STRING
rstr = "";
for (i = 0; i < slen; i++)
   {
   param = 1664525L * param + 1013904223L;
   rstr += basechars[static_cast<unsigned int>(param >> 16) % basechars.size()];
   } // -- END FOR
} // -- END 
    
    


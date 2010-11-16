/******************************************************************************
 * CSCI 330 Slice 5 Project
 * Convert input into 64-bit machine code
 * Shaun Meyer, Nov 2010
 *****************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

typedef unsigned long long int ulli;

/*
 Instruction layout:

 [ NEXT INSTR (10 p.64) ] [ JMP (4 p.54) ] [ ALU CONTR (8 p.50) ] [ ZONE RD (4 p.42) ] [ REGISTER RD (20 p.38) ] 
 [ PRM MEM (2 p.18) ] [ MAD SRC (4 p.16) ] [ MDB SRC (4 p.12) ] [ B SRC (4 p.8) ] [ A SRC (4 p.4) ]
*/

/* The webpage should have fields for the following:

 * Next Instruction (accept binary or decimal... first character should be a 
   B or D to indicate which)

 * JMP (4 binary digits)

 * ALU Control (8 binary digits)

 * Zone Read (4 bits)

 * Register Read (10 fields, each labeled for a register, 2 binary digits per 
   field)

 * Primary Memory (2 bits)

 * MAD source ( accept binary or decimal... first character should be a B or D 
   to indicate which)

 * MDB source ( accept binary or decimal... first character should be a B or D 
   to indicate which)

 * B source ( accept binary or decimal... first character should be a B or D 
   to indicate which)

 * A  source ( accept binary or decimal... first character should be a B or D 
   to indicate which)

 * For JMP, ALU control, Zone Read, each of the Register Read subfields, and Primary Memory 
 you may use individual bit labels w/ radio buttons, check boxes, and/or select lists if you prefer.

*/






/*
 * binary_to_str(ulli) - given a ulli representation of a number, 
 * return a string of that binary.
 */

string binary_to_str(ulli binary)
{
  string ret;
  for (ulli i = ((ulli)1 << 63); i; i >>= 1)
    {
      ret += (binary & i) ? '1' : '0';
    }
  return ret;
}

/* 
 * d_str_to_binary(string) - given a string with a leading D (for decimal)
 * return a ulli representing that number.
 */

ulli d_str_to_binary(string inpt)
{
  
  /* erase first character [ B | D ] */
  inpt.erase(inpt.begin());
  
  return (ulli)atoi( inpt.c_str() );

}

/*
 * b_str_to_binary(string) - given a string with a leading B (for binary)
 * return a ulli representing that number. 
 */

ulli b_str_to_binary(string inpt)
{
  /* erase first character [ B | D ] */
  inpt.erase(inpt.begin());
  
  ulli binary = 0;

  for (int i = 0; i < inpt.size(); i++)
    {
      binary <<= 1;
      if (inpt[i] == '1')
	binary += 1;
    }

  return binary;
}

ulli str_to_binary(string inpt)
{
  switch ( toupper(inpt[0]) ) 
    {
    case 'D':
      return d_str_to_binary(inpt);
      break;
      
    case 'B':
      return b_str_to_binary(inpt);
      break;
      
    default:
      return 0;
    }
}


/*
  given something like '000011' if we want this entire thing moved to a given
  location, where the leading 0 would be at `pos` we simply left shift by 
  (pos-len).
  
  pos = position. 0 is the least significant (right-most) bit
  len = length of our input binary.
*/

ulli set_binary_pos(ulli binary, int pos, int len)
{
  return binary << (pos-len);
}
	   
      

int main()
{

/* Microinstruction format positions: */
  enum {
    NEXT_INSTR = 64,
    JMP = 54,
    ALU_CONTR = 50,
    ZONE_RD = 42,
    REG_RD = 38,
    PRM_MEM = 18,
    MAD_SRC = 16,
    MDB_SRC = 12,
    B_SRC = 8,
    A_SRC = 4
};

  /* inputs */

  string s_next_instruction, 
    s_jmp, 
    s_alu_control, 
    s_zone_read, 
    s_register_read,
    s_primary_memory,
    s_MAD_source, 
    s_MDB_source, 
    s_B_source,
    s_A_source;

  cin >> s_next_instruction 
      >> s_jmp 
      >> s_alu_control
      >> s_zone_read
      >> s_register_read
      >> s_primary_memory
      >> s_MAD_source
      >> s_MDB_source
      >> s_B_source
      >> s_A_source;
  
  ulli binary = 0; /* this is where our result is going */
  
  binary += set_binary_pos(str_to_binary(s_next_instruction), NEXT_INSTR, 10);
  binary += set_binary_pos(str_to_binary(s_jmp), JMP, 4);
  binary += set_binary_pos(str_to_binary(s_alu_control), ALU_CONTR, 8);
  binary += set_binary_pos(str_to_binary(s_zone_read), ZONE_RD, 4);
  binary += set_binary_pos(str_to_binary(s_register_read), REG_RD, 20);
  binary += set_binary_pos(str_to_binary(s_primary_memory), PRM_MEM, 2);
  binary += set_binary_pos(str_to_binary(s_MAD_source), MAD_SRC, 4);
  binary += set_binary_pos(str_to_binary(s_MDB_source), MDB_SRC, 4);
  binary += set_binary_pos(str_to_binary(s_B_source), B_SRC, 4);
  binary += set_binary_pos(str_to_binary(s_A_source), A_SRC, 4);
  
  cout << "Content-Type: text/plain\n\n";
  cout << binary_to_str(binary) << endl;

  /*
  cout << binary << endl
       << binary_to_str(binary) << endl
       << binary_to_str(set_binary_pos(15, 4, 4)) << endl
       << binary_to_str(set_binary_pos(15, 64, 4)) << endl;
  */
  return 0;

}

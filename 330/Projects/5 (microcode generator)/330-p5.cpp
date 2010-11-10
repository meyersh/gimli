/******************************************************************************
 * CSCI 330 Slice 5 Project
 * Convert input into 64-bit machine code
 * Shaun Meyer, Nov 2010
 *****************************************************************************/

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

/*
 Instruction layout:

 [ NEXT INSTR (10) ] [ JMP (4) ] [ ALU CONTR (8) ] [ ZONE RD (4) ] [ REGISTER RD (20) ] [ PRM MEM (4) ]
 [ MAD SRC (4) ] [MDB SRC (4) ] [ B SRC (4) ] [ A SRC (4) ]

*/


unsigned long long int d_str_to_binary(string inpt)
{
  
  /* erase first character [ B | D ] */
  inpt.erase(inpt.begin());
  
  return (unsigned long long int)atoi( inpt.c_str() );

}

unsigned long long int b_str_to_binary(string inpt)
{
  /* erase first character [ B | D ] */
  inpt.erase(inpt.begin());
  
  unsigned long long int binary = 0;

  for (int i = 0; i < inpt.size(); i++)
    {
      if (inpt[i] == '1')
	binary += 1;
      binary <<= 1;
    }

  return binary;
}

int main()
{

  /* inputs */

  string s_next_instruction, s_jmp, s_alu_control, s_zone_read, s_register_read,
    s_primary_memory, s_MAD_source, s_MDB_source, s_B_source, s_A_source;

  /*
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
  
  */
  cin >> s_B_source >> s_A_source;

  cout << b_str_to_binary(s_B_source) << endl 
       << d_str_to_binary(s_A_source) << endl;

  return 0;

}

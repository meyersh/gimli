/******************************************************************************
 * CSCI 330 Slice 5 Project
 * Convert input into 64-bit machine code
 * Shaun Meyer, Nov 2010
 *****************************************************************************/

#include <iostream>
#include <string>
#include <ccstring>

/*
 Instruction layout:

 [ NEXT INSTR (10) ] [ JMP (4) ] [ ALU CONTR (8) ] [ ZONE RD (4) ] [ REGISTER RD (20) ] [ PRM MEM (4) ]
 [ MAD SRC (4) ] [MDB SRC (4) ] [ B SRC (4) ] [ A SRC (4) ]

*/

int main()
{

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


    

  return 0;

}

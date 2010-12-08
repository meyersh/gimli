#!/usr/bin/ruby
#

SRC = {
  :reg0 => 0b0000,
  :reg1 => 0b0001,
  :reg2 => 0b0010,
  :reg3 => 0b0011,
  :reg4 => 0b0100,
  :reg5 => 0b0101,
  :reg6 => 0b0110,
  :reg7 => 0b0111,
  :ir => 0b1110,
  :pc => 0b1111,
  :zone1 => 0b1000,
  :zone2 => 0b1001,
  :zone3 => 0b1010,
  :zone4 => 0b10011,
  :zero => 0b1100,
  :one => 0b1101
}

PRM_MEM = {
  :access => 0b10,
  :rw => 0b01
}

REG_RD = {
  :access => 0b10,
  :busselect => 0b01
}

JMP = {
  :jamn => 0b1000,
  :jamz => 0b0100,
  :jamp => 0b0010,
  :jamc => 0b0001
}

ALU_SHIFT = {
  :noshift => 0b00,
  :a_rs => 0b01,
  :left_shift => 0b10,
  :right_shift => 0b11
}

ALU_CNTR = {
  :a => 0b011000,
  :b => 0b010100,
  :not_a => 0b011010,
  :not_b => 0b101100,
  :a_plus_b => 0b111100,
  :a_plus_b_plus_1 => 0b111101,
  :inc_a => 0b111001,
  :inc_b => 0b110101,
  :b_sub_a => 0b111111,
  :dec_b => 0b110110,
  :neg_a => 0b111011,
  :a_and_b => 0b001100,
  :a_or_b => 0b011100,
  :zero => 0b010000,
  :one =>  0b110010,
  :neg_1 => 0b110010
}
  

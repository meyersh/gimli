#!/usr/bin/ruby
#

# SOURCES
class Src 
  REG0 = 0b0000
  REG1 = 0b0001
  REG2 = 0b0010
  REG3 = 0b0011
  REG4 = 0b0100
  REG5 = 0b0101
  REG6 = 0b0110
  REG7 = 0b0111
  IR = 0b1110
  PC = 0b1111
  ZONE0 = 0b1000
  ZONE1 = 0b1001
  ZONE2 = 0b1010
  ZONE3 = 0b1011
  ZERO = 0b1100
  ONE = 0b1101
end

# BUS / MEMORY SELECTORS
rw = 0b01
access = 0b10
busselect = 0b01

class Reg
  class PC
    Access = 0x80000
    Select = 0x40000
  end
  class IR
    Access = 0x20000
    Select = 0x10000
  end
  class REG7
    Access = 0x08000
    Select = 0x04000
  end
  class REG6
    Access = 0x02000
    Select = 0x01000
  end
  class REG5
    Access = 0x00800
    Select = 0x00400
  end
  class REG4
    Access = 0x00200
    Select = 0x00100
  end
  class REG3
    Access = 0x00080
    Select = 0x00040
  end
  class REG2
    Access = 0x00020
    Select = 0x00010
  end
  class REG1
    Access = 0x00008
    Select = 0x00004
  end
  class REG0
    Access = 0x00002
    Select = 0x00001
  end
end

class Zone
  ZONE3 = 0b1000
  ZONE2 = 0b0100
  ZONE1 = 0b0010
  ZONE0 = 0b0001
end

# JAM Bits
class Jam
  N = 0b1000
  Z = 0b0100
  P = 0b0010
  C = 0b0001
end

# ALU Shifter
class Shift
  NO = 0b00
  A_RS = 0b01
  LEFT_SHIFT = 0b10
  RIGHT_SHIFT = 0b11
end

# ALU Control
class Alu
  A = 0b011000
  B = 0b010100
  NOT_A = 0b011010
  NOT_B = 0b101100
  ADD = 0b111100
  A_PLUS_B_PLUS_1 = 0b111101
  INC_A = 0b111001
  INC_B = 0b110101
  B_SUB_A = 0b111111
  DEC_B = 0b110110
  NEG_A = 0b111011
  A_AND_B = 0b001100
  A_OR_B = 0b011100
  ZERO = 0b010000
  ONE =  0b110010
  NEG_1 = 0b110010
end
  
class Microinstruction
  attr_accessor :opcode, :symbol, :next_instr, :jmp, :alu_shift, :alu_control, :zone_rd, :reg_rd, :prm_mem, :mad_src, :mdb_src, :b_src, :a_src

  def initialize(params)

    # Set default parameters to something sane(?)
    params = {
      :opcode => 0,
      :symbol => nil,
      :next_instr => 0, 
      :jmp => 0, 
      :alu_shift => 0, 
      :alu_control => 0, 
      :zone_rd => 0, 
      :reg_rd => 0,
      :prm_mem => 0, 
      :mad_src => 0, 
      :mdb_src => 0, 
      :b_src => 0, 
      :a_src => 0 
    }.merge(params)

    # Set all of our attributes:
    @opcode = params[:opcode]
    @symbol = params[:symbol]
    @next_instr = params[:next_instr] & 1023 # Mask out any more than 10 bits
    @jmp = params[:jmp]
    @alu_shift = params[:alu_shift]
    @alu_control = params[:alu_control]
    @zone_rd = params[:zone_rd]
    @reg_rd = params[:reg_rd]
    @prm_mem = params[:prm_mem]
    @mad_src = params[:mad_src]
    @mdb_src = params[:mdb_src]
    @b_src = params[:b_src]
    @a_src = params[:a_src]
      end

  # Return a string representation (64-bit binary, in our case)
  def binary
    return "%010b%04b%02b%06b%04b%020b%02b%04b%04b%04b%04b" % [
       @next_instr, @jmp, @alu_shift, @alu_control, @zone_rd, @reg_rd, @prm_mem, 
       @mad_src, @mdb_src, @b_src, @a_src]
  end

  def hex
    return "%016x" % String("0b" + self.binary)
  end

  def to_s
    return self.binary
  end

end

instructions = []

instructions.push Microinstruction.new(
                                       :symbol => "JUMP",
                                       :a_src => Src::PC,
                                       :alu_control => Alu::INC_A,
                                       :reg_rd => Reg::PC::Access
)

instructions.push Microinstruction.new(
                         :symbol => "LOAD_IMM1",
                         :next_instr => 0b0000000001,
                         :alu_control => Alu::A,
                         :mdb_src => Src::IR,
                         :reg_rd => Reg::IR::Access,
                         :zone_rd => Src::ZONE3
)

instructions.push Microinstruction.new(
                                       :symbol => "ADD",
                                       :opcode => 0b10000100,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ADD,
                                       :a_src => Src::ZONE0,
                                       :b_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.each do |mi|
  puts mi.symbol << ":"
  puts "%8s    %s" % ["Opcode", "Machine instruction"]
  puts "%08b => %s" % [ mi.opcode, mi.to_s ]
  puts "Hex => " << mi.hex
  puts
end


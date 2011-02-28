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
  ZONE0 = 0b0001
  ZONE1 = 0b0010
  ZONE2 = 0b0100
  ZONE3 = 0b1000
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
  AND = 0b001100
  OR = 0b011100
  ZERO = 0b010000
  ONE =  0b110010
  NEG_1 = 0b110010
end
  
class Microinstruction
  attr_accessor :opcode, :micro_addr, :symbol, :next_instr, :jmp, :alu_shift, :alu_control, :zone_rd, :reg_rd, :prm_mem, :mad_src, :mdb_src, :b_src, :a_src

  def initialize(params)

    # Set default parameters to something sane(?)
    params = {
      :opcode => 0,
      :micro_addr => 0,
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
    @micro_addr = params[:micro_addr]
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
                                       :symbol => "MAIN",
                                       :micro_addr => 0b0000000000,
                                       :next_instr => 0b0000000001,
                                       :a_src => Src::ONE,
                                       :mad_src => Src::ZERO,
                                       :alu_control => Alu::A,
                                       :reg_rd => Reg::IR::Access | Reg::IR::Select | Reg::PC::Access,
                                       :jmp => Jam::C,
                                       :prm_mem => access
)

instructions.push Microinstruction.new(
                                       :symbol => "DISPATCH",
                                       :micro_addr => 0b0000000001,
                                       :next_instr => 0b0000000000,
                                       :a_src => Src::PC,
                                       :alu_control => Alu::INC_A,
                                       :mad_src => Src::PC,
                                       :reg_rd => Reg::IR::Access | Reg::IR::Select | Reg::PC::Access,
                                       :jmp => Jam::C,
                                       :prm_mem => access
)

instructions.push Microinstruction.new(
                                       :symbol => "JUMP",
                                       :micro_addr => 0b0000000010,
                                       :next_instr => 0b0000000000,
                                       :alu_control => Alu::A,
                                       :mad_src => Src::PC,
                                       :reg_rd => Reg::IR::Access | Reg::IR::Select,
                                       :jmp => Jam::C
)

#
## (LOAD IMMEDIATE) Place value into Z3 Register.
#

instructions.push Microinstruction.new(
                                       :symbol => "LOAD_IMMEDIATE",
                                       :opcode => 0b00001,
                                       :micro_addr => 0b0000010000,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::A,
                                       :a_src => Src::IR,
                                       :zone_rd => Src::ZONE3
)

instructions.push Microinstruction.new(
                                       :symbol => "ADD",
                                       :opcode => 0b10000100,
                                       :micro_addr => 0b0100001000,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ADD,
                                       :a_src => Src::ZONE0,
                                       :b_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => "LOAD_DIRECT",
                                       :next_instr => 0b0000000001,
                                       :micro_addr => 0b00000100000,
                                       :alu_control => Alu::A,
                                       :mad_src => Src::IR,
                                       :reg_rd => Reg::IR::Access,
                                       :zone_rd => Src::ZONE3,
                                       :prm_mem => access
)

instructions.push Microinstruction.new(
                                       :symbol => "STORE_DIRECT",
                                       :opcode => 0b00011,
                                       :next_instr => 0b0000000001,
                                       :micro_addr => 0b0000110000,
                                       :alu_control => Alu::A,
                                       :mad_src => Src::IR,
                                       :reg_rd => Reg::IR::Access,
                                       :zone_rd => Src::ZONE3,
                                       :prm_mem => access | rw
)

instructions.push Microinstruction.new(
                                       :symbol => "SUB",
                                       :opcode => 0b10000101,
                                       :micro_addr => 0b0100001010,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::B_SUB_A,
                                       :a_src => Src::ZONE0,
                                       :b_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => "AND",
                                       :opcode => 0b10001100,
                                       :micro_addr => 0b0100001100,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::AND,
                                       :a_src => Src::ZONE0,
                                       :b_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => "OR",
                                       :opcode => 0b10001110,
                                       :micro_addr => 0b0100001110,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::OR,
                                       :a_src => Src::ZONE0,
                                       :b_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => "NOT",
                                       :opcode => 0b100010000,
                                       :micro_addr => 0b0100010000,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::NOT_A,
                                       :a_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)
instructions.push Microinstruction.new(
                                       :symbol => "NEG",
                                       :opcode => 0b100010010,
                                       :micro_addr => 0b0100010010,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::NEG_A,
                                       :a_src => Src::ZONE1,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => "NOP",
                                       :opcode => 0b11111110,
                                       :micro_addr => 0b0111111100,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::A,
                                       :a_src => Src::ZERO,
)

instructions.push Microinstruction.new(
                                       :symbol => "HALT",
                                       :opcode => 0b11111111,
                                       :micro_addr => 0b0111111110,
                                       :next_instr => 0b1111111111,
                                       :alu_control => Alu::A,
                                       :a_src => Src::ZERO
)

##
# CMPLT
##

instructions.push Microinstruction.new(
                                       :symbol => 'cmplt1',
                                       :micro_addr => 0b0100010100,
                                       :opcode => 0b10001010,
                                       :next_instr => 0b0100010100,
                                       :alu_control => Alu::B_SUB_A,
                                       :b_src => Src::ZONE1,
                                       :a_src => Src::ZONE0,
                                       :jmp => Jam::N
)

instructions.push Microinstruction.new(
                                       :symbol => 'cmplt-true',
                                       :micro_addr => 0b1100010101,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ONE,
                                       :zone_rd => Zone::ZONE2
                                       )

instructions.push Microinstruction.new(
                                       :symbol => 'cmplt-false',
                                       :micro_addr => 0b0100010101,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ZERO,
                                       :zone_rd => Zone::ZONE2
)

##
# CMPGT
##

instructions.push Microinstruction.new(
                                       :symbol => 'cmpgt1',
                                       :micro_addr => 0b0100010110,
                                       :opcode => 0b10001010,
                                       :next_instr => 0b0100010100,
                                       :alu_control => Alu::B_SUB_A,
                                       :b_src => Src::ZONE1,
                                       :a_src => Src::ZONE0,
                                       :jmp => Jam::P
)

instructions.push Microinstruction.new(
                                       :symbol => 'cmpgt-true',
                                       :micro_addr => 0b1100010111,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ONE,
                                       :zone_rd => Zone::ZONE2
                                       )

instructions.push Microinstruction.new(
                                       :symbol => 'cmpgt-false',
                                       :micro_addr => 0b0100010111,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ZERO,
                                       :zone_rd => Zone::ZONE2
)

## 
# CMPEQ
##

instructions.push Microinstruction.new(
                                       :symbol => 'cmpeq1',
                                       :micro_addr => 0b0100011000,
                                       :opcode => 0b10001100,
                                       :next_instr => 0b0100011001,
                                       :alu_control => Alu::B_SUB_A,
                                       :b_src => Src::ZONE1,
                                       :a_src => Src::ZONE0,
                                       :jmp => Jam::Z
)

instructions.push Microinstruction.new(
                                       :symbol => 'cmpeq-true',
                                       :micro_addr => 0b1100011001,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ONE,
                                       :zone_rd => Zone::ZONE2
                                       )

instructions.push Microinstruction.new(
                                       :symbol => 'cmpeq-false',
                                       :micro_addr => 0b0100011001,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ZERO,
                                       :zone_rd => Zone::ZONE2
)

##
# CMPNEQ
##

instructions.push Microinstruction.new(
                                       :symbol => 'cmpneq',
                                       :micro_addr => 0b0100011010,
                                       :opcode => 0b10001101,
                                       :next_instr => 0b0100011011,
                                       :alu_control => Alu::B_SUB_A,
                                       :b_src => Src::ZONE1,
                                       :a_src => Src::ZONE0,
                                       :jmp => Jam::N | Jam::P
)

instructions.push Microinstruction.new(
                                       :symbol => 'cmpneq-true',
                                       :micro_addr => 0b1100011011,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ONE,
                                       :zone_rd => Zone::ZONE2
                                       )

instructions.push Microinstruction.new(
                                       :symbol => 'cmpneq-false',
                                       :micro_addr => 0b0100011011,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ZERO,
                                       :zone_rd => Zone::ZONE2
)

instructions.push Microinstruction.new(
                                       :symbol => 'JUMP_DIRECT',
                                       :opcode => 0b00100,
                                       :micro_addr => 0b0001000000,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::A,
                                       :mad_src => Src::IR,
                                       :reg_rd => Reg::IR::Select | Reg::PC::Access,
                                       :prm_mem => access
)
                                       
##
# CONDITIONAL JUMP REGISTER LT
##

instructions.push Microinstruction.new(
                                       :symbol => 'CJRLT1',
                                       :micro_addr => 0b0100011100,
                                       :opcode => 0b10001110,
                                       :next_instr => 0b0100011100,
                                       :alu_control => Alu::B_SUB_A,
                                       :b_src => Src::ZONE1,
                                       :a_src => Src::ZONE0,
                                       :jmp => Jam::N
)

instructions.push Microinstruction.new(
                                       :symbol => 'cjrlt-true',
                                       :micro_addr => 0b1100011101,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ONE,
                                       :jmp => Jam::C,
                                       :reg_rd => Reg::PC::Access | Reg::PC::Select | Reg::IR::Access | Reg::IR::Select,
                                       :zone_rd => Zone::ZONE2
                                       )

instructions.push Microinstruction.new(
                                       :symbol => 'cjrlt-false',
                                       :micro_addr => 0b0100011101,
                                       :next_instr => 0b0000000001,
                                       :alu_control => Alu::ZERO,
)


instructions.each do |mi|
  puts "b%010b:%s" % [ mi.micro_addr, mi.hex ] 
end


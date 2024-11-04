#include "6502.h"

#include "utils.h"

#include <stdint.h>
#include <stdio.h>

/**
 * Parses the location in memory pointed to by 'pc' and creates an Instruction
 *
 * @param mem - The byte array serving as system memory
 * @param pc - The program counter (points to the current instruction in memory)
 *
 * @returns An Instruction with all the information needed about it
 */
Instruction parse_instruction(uint8_t* mem, uint16_t pc) {
    Instruction instruction;

    instruction.opcode = mem[pc];

    switch (instruction.opcode) {
        // ---------- ADC ----------
        case 0x69:  // nice
            instruction.name = "ADC";
            instruction.addr_mode = IMM;
            break;
        case 0x65:
            instruction.name = "ADC";
            instruction.addr_mode = ZP;
            break;
        case 0x75:
            instruction.name = "ADC";
            instruction.addr_mode = ZPX;
            break;
        case 0x6D:
            instruction.name = "ADC";
            instruction.addr_mode = ABS;
            break;
        case 0x7D:
            instruction.name = "ADC";
            instruction.addr_mode = ABSX;
            break;
        case 0x79:
            instruction.name = "ADC";
            instruction.addr_mode = ABSY;
            break;
        case 0x61:
            instruction.name = "ADC";
            instruction.addr_mode = INDX;
            break;
        case 0x71:
            instruction.name = "ADC";
            instruction.addr_mode = INDY;
            break;
        // ---------- AND ----------
        case 0x29:
            instruction.name = "AND";
            instruction.addr_mode = IMM;
            break;
        case 0x25:
            instruction.name = "AND";
            instruction.addr_mode = ZP;
            break;
        case 0x35:
            instruction.name = "AND";
            instruction.addr_mode = ZPX;
            break;
        case 0x2D:
            instruction.name = "AND";
            instruction.addr_mode = ABS;
            break;
        case 0x3D:
            instruction.name = "AND";
            instruction.addr_mode = ABSX;
            break;
        case 0x39:
            instruction.name = "AND";
            instruction.addr_mode = ABSY;
            break;
        case 0x21:
            instruction.name = "AND";
            instruction.addr_mode = INDX;
            break;
        case 0x31:
            instruction.name = "AND";
            instruction.addr_mode = INDY;
            break;
        // ---------- ASL ----------
        case 0x0A:
            instruction.name = "ASL";
            instruction.addr_mode = ACCUM;
            break;
        case 0x06:
            instruction.name = "ASL";
            instruction.addr_mode = ZP;
            break;
        case 0x16:
            instruction.name = "ASL";
            instruction.addr_mode = ZPX;
            break;
        case 0x0E:
            instruction.name = "ASL";
            instruction.addr_mode = ABS;
            break;
        case 0x1E:
            instruction.name = "ASL";
            instruction.addr_mode = ABSX;
            break;
        // ---------- BCC ----------
        case 0x90:
            instruction.name = "BCC";
            instruction.addr_mode = REL;
            break;
        // ---------- BCS ----------
        case 0xB0:
            instruction.name = "BCS";
            instruction.addr_mode = REL;
            break;
        // ---------- BEQ ----------
        case 0xF0:
            instruction.name = "BEQ";
            instruction.addr_mode = REL;
            break;
        // ---------- BIT ----------
        case 0x24:
            instruction.name = "BIT";
            instruction.addr_mode = ZP;
            break;
        case 0x2C:
            instruction.name = "BIT";
            instruction.addr_mode = ABS;
            break;
        // ---------- BMI ----------
        case 0x30:
            instruction.name = "BMI";
            instruction.addr_mode = REL;
            break;
        // ---------- BNE ----------
        case 0xD0:
            instruction.name = "BNE";
            instruction.addr_mode = REL;
            break;
        // ---------- BPL ----------
        case 0x10:
            instruction.name = "BPL";
            instruction.addr_mode = REL;
            break;
        // ---------- BRK ----------
        case 0x00:
            instruction.name = "BRK";
            instruction.addr_mode = IMPL;
            break;
        // ---------- BVC ----------
        case 0x50:
            instruction.name = "BVC";
            instruction.addr_mode = REL;
            break;
        // ---------- BVS ----------
        case 0x70:
            instruction.name = "BVS";
            instruction.addr_mode = REL;
            break;
        // ---------- CLC ----------
        case 0x18:
            instruction.name = "CLC";
            instruction.addr_mode = IMPL;
            break;
        // ---------- CLD ----------
        case 0xD8:
            instruction.name = "CLD";
            instruction.addr_mode = IMPL;
            break;
        // ---------- CLI ----------
        case 0x58:
            instruction.name = "CLI";
            instruction.addr_mode = IMPL;
            break;
        // ---------- CLV ----------
        case 0xB8:
            instruction.name = "CLV";
            instruction.addr_mode = IMPL;
            break;
        // ---------- CMP ----------
        case 0xC9:
            instruction.name = "CMP";
            instruction.addr_mode = IMM;
            break;
        case 0xC5:
            instruction.name = "CMP";
            instruction.addr_mode = ZP;
            break;
        case 0xD5:
            instruction.name = "CMP";
            instruction.addr_mode = ZPX;
            break;
        case 0xCD:
            instruction.name = "CMP";
            instruction.addr_mode = ABS;
            break;
        case 0xDD:
            instruction.name = "CMP";
            instruction.addr_mode = ABSX;
            break;
        case 0xD9:
            instruction.name = "CMP";
            instruction.addr_mode = ABSY;
            break;
        case 0xC1:
            instruction.name = "CMP";
            instruction.addr_mode = INDX;
            break;
        case 0xD1:
            instruction.name = "CMP";
            instruction.addr_mode = INDY;
            break;
        // ---------- CPX ----------
        case 0xE0:
            instruction.name = "CPX";
            instruction.addr_mode = IMM;
            break;
        case 0xE4:
            instruction.name = "CPX";
            instruction.addr_mode = ZP;
            break;
        case 0xEC:
            instruction.name = "CPX";
            instruction.addr_mode = ABS;
            break;
        // ---------- CPY ----------
        case 0xC0:
            instruction.name = "CPY";
            instruction.addr_mode = IMM;
            break;
        case 0xC4:
            instruction.name = "CPY";
            instruction.addr_mode = ZP;
            break;
        case 0xCC:
            instruction.name = "CPY";
            instruction.addr_mode = ABS;
            break;
        // ---------- DEC ----------
        case 0xC6:
            instruction.name = "DEC";
            instruction.addr_mode = ZP;
            break;
        case 0xD6:
            instruction.name = "DEC";
            instruction.addr_mode = ZPX;
            break;
        case 0xCE:
            instruction.name = "DEC";
            instruction.addr_mode = ABS;
            break;
        case 0xDE:
            instruction.name = "DEC";
            instruction.addr_mode = ABSX;
            break;
        // ---------- DEX ----------
        case 0xCA:
            instruction.name = "DEX";
            instruction.addr_mode = IMPL;
            break;
        // ---------- DEY ----------
        case 0x88:
            instruction.name = "DEY";
            instruction.addr_mode = IMPL;
            break;
        // ---------- EOR ----------
        case 0x49:
            instruction.name = "EOR";
            instruction.addr_mode = IMM;
            break;
        case 0x45:
            instruction.name = "EOR";
            instruction.addr_mode = ZP;
            break;
        case 0x55:
            instruction.name = "EOR";
            instruction.addr_mode = ZPX;
            break;
        case 0x4D:
            instruction.name = "EOR";
            instruction.addr_mode = ABS;
            break;
        case 0x5D:
            instruction.name = "EOR";
            instruction.addr_mode = ABSX;
            break;
        case 0x59:
            instruction.name = "EOR";
            instruction.addr_mode = ABSY;
            break;
        case 0x41:
            instruction.name = "EOR";
            instruction.addr_mode = INDX;
            break;
        case 0x51:
            instruction.name = "EOR";
            instruction.addr_mode = INDY;
            break;
        // ---------- INC ----------
        case 0xE6:
            instruction.name = "INC";
            instruction.addr_mode = ZP;
            break;
        case 0xF6:
            instruction.name = "INC";
            instruction.addr_mode = ZPX;
            break;
        case 0xEE:
            instruction.name = "INC";
            instruction.addr_mode = ABS;
            break;
        case 0xFE:
            instruction.name = "INC";
            instruction.addr_mode = ABSX;
            break;
        // ---------- INX ----------
        case 0xE8:
            instruction.name = "INX";
            instruction.addr_mode = IMPL;
            break;
        // ---------- INY ----------
        case 0xC8:
            instruction.name = "INY";
            instruction.addr_mode = IMPL;
            break;
        // ---------- JMP ----------
        case 0x4C:
            instruction.name = "JMP";
            instruction.addr_mode = ABS;
            break;
        case 0x6C:
            instruction.name = "JMP";
            instruction.addr_mode = IND;
            break;
        // ---------- JSR ----------
        case 0x20:
            instruction.name = "JSR";
            instruction.addr_mode = ABS;
            break;
        // ---------- LDA ----------
        case 0xA9:
            instruction.name = "LDA";
            instruction.addr_mode = IMM;
            break;
        case 0xA5:
            instruction.name = "LDA";
            instruction.addr_mode = ZP;
            break;
        case 0xB5:
            instruction.name = "LDA";
            instruction.addr_mode = ZPX;
            break;
        case 0xAD:
            instruction.name = "LDA";
            instruction.addr_mode = ABS;
            break;
        case 0xBD:
            instruction.name = "LDA";
            instruction.addr_mode = ABSX;
            break;
        case 0xB9:
            instruction.name = "LDA";
            instruction.addr_mode = ABSY;
            break;
        case 0xA1:
            instruction.name = "LDA";
            instruction.addr_mode = INDX;
            break;
        case 0xB1:
            instruction.name = "LDA";
            instruction.addr_mode = INDY;
            break;
        // ---------- LDX ----------
        case 0xA2:
            instruction.name = "LDX";
            instruction.addr_mode = IMM;
            break;
        case 0xA6:
            instruction.name = "LDX";
            instruction.addr_mode = ZP;
            break;
        case 0xB6:
            instruction.name = "LDX";
            instruction.addr_mode = ZPY;
            break;
        case 0xAE:
            instruction.name = "LDX";
            instruction.addr_mode = ABS;
            break;
        case 0xBE:
            instruction.name = "LDX";
            instruction.addr_mode = ABSY;
            break;
        // ---------- LDY ----------
        case 0xA0:
            instruction.name = "LDY";
            instruction.addr_mode = IMM;
            break;
        case 0xA4:
            instruction.name = "LDY";
            instruction.addr_mode = ZP;
            break;
        case 0xB4:
            instruction.name = "LDY";
            instruction.addr_mode = ZPX;
            break;
        case 0xAC:
            instruction.name = "LDY";
            instruction.addr_mode = ABS;
            break;
        case 0xBC:
            instruction.name = "LDY";
            instruction.addr_mode = ABSX;
            break;
        // ---------- LSR ----------
        case 0x4A:
            instruction.name = "LSR";
            instruction.addr_mode = ACCUM;
            break;
        case 0x46:
            instruction.name = "LSR";
            instruction.addr_mode = ZP;
            break;
        case 0x56:
            instruction.name = "LSR";
            instruction.addr_mode = ZPX;
            break;
        case 0x4E:
            instruction.name = "LSR";
            instruction.addr_mode = ABS;
            break;
        case 0x5E:
            instruction.name = "LSR";
            instruction.addr_mode = ABSX;
            break;
        // ---------- NOP ----------
        case 0xEA:
            instruction.name = "NOP";
            instruction.addr_mode = IMPL;
            break;
        // ---------- ORA ----------
        case 0x09:
            instruction.name = "ORA";
            instruction.addr_mode = IMM;
            break;
        case 0x05:
            instruction.name = "ORA";
            instruction.addr_mode = ZP;
            break;
        case 0x15:
            instruction.name = "ORA";
            instruction.addr_mode = ZPX;
            break;
        case 0x0D:
            instruction.name = "ORA";
            instruction.addr_mode = ABS;
            break;
        case 0x1D:
            instruction.name = "ORA";
            instruction.addr_mode = ABSX;
            break;
        case 0x19:
            instruction.name = "ORA";
            instruction.addr_mode = ABSY;
            break;
        case 0x01:
            instruction.name = "ORA";
            instruction.addr_mode = INDX;
            break;
        case 0x11:
            instruction.name = "ORA";
            instruction.addr_mode = INDY;
            break;
        // ---------- PHA ----------
        case 0x48:
            instruction.name = "PHA";
            instruction.addr_mode = IMPL;
            break;
        // ---------- PHP ----------
        case 0x08:
            instruction.name = "PHP";
            instruction.addr_mode = IMPL;
            break;
        // ---------- PLA ----------
        case 0x68:
            instruction.name = "PLA";
            instruction.addr_mode = IMPL;
            break;
        // ---------- PLP ----------
        case 0x28:
            instruction.name = "PLP";
            instruction.addr_mode = IMPL;
            break;
        // ---------- ROL ----------
        case 0x2A:
            instruction.name = "ROL";
            instruction.addr_mode = ACCUM;
            break;
        case 0x26:
            instruction.name = "ROL";
            instruction.addr_mode = ZP;
            break;
        case 0x36:
            instruction.name = "ROL";
            instruction.addr_mode = ZPX;
            break;
        case 0x2E:
            instruction.name = "ROL";
            instruction.addr_mode = ABS;
            break;
        case 0x3E:
            instruction.name = "ROL";
            instruction.addr_mode = ABSX;
            break;
        // ---------- ROR ----------
        case 0x6A:
            instruction.name = "ROR";
            instruction.addr_mode = ACCUM;
            break;
        case 0x66:
            instruction.name = "ROR";
            instruction.addr_mode = ZP;
            break;
        case 0x76:
            instruction.name = "ROR";
            instruction.addr_mode = ZPX;
            break;
        case 0x6E:
            instruction.name = "ROR";
            instruction.addr_mode = ABS;
            break;
        case 0x7E:
            instruction.name = "ROR";
            instruction.addr_mode = ABSX;
            break;
        // ---------- RTI ----------
        case 0x40:
            instruction.name = "RTI";
            instruction.addr_mode = IMPL;
            break;
        // ---------- RTS ----------
        case 0x60:
            instruction.name = "RTS";
            instruction.addr_mode = IMPL;
            break;
        // ---------- SBC ----------
        case 0xE9:
            instruction.name = "SBC";
            instruction.addr_mode = IMM;
            break;
        case 0xE5:
            instruction.name = "SBC";
            instruction.addr_mode = ZP;
            break;
        case 0xF5:
            instruction.name = "SBC";
            instruction.addr_mode = ZPX;
            break;
        case 0xED:
            instruction.name = "SBC";
            instruction.addr_mode = ABS;
            break;
        case 0xFD:
            instruction.name = "SBC";
            instruction.addr_mode = ABSX;
            break;
        case 0xF9:
            instruction.name = "SBC";
            instruction.addr_mode = ABSY;
            break;
        case 0xE1:
            instruction.name = "SBC";
            instruction.addr_mode = INDX;
            break;
        case 0xF1:
            instruction.name = "SBC";
            instruction.addr_mode = INDY;
            break;
        // ---------- SEC ----------
        case 0x38:
            instruction.name = "SEC";
            instruction.addr_mode = IMPL;
            break;
        // ---------- SED ----------
        case 0xF8:
            instruction.name = "SED";
            instruction.addr_mode = IMPL;
            break;
        // ---------- SEI ----------
        case 0x78:
            instruction.name = "SEI";
            instruction.addr_mode = IMPL;
            break;
        // ---------- STA ----------
        case 0x85:
            instruction.name = "STA";
            instruction.addr_mode = ZP;
            break;
        case 0x95:
            instruction.name = "STA";
            instruction.addr_mode = ZPX;
            break;
        case 0x8D:
            instruction.name = "STA";
            instruction.addr_mode = ABS;
            break;
        case 0x9D:
            instruction.name = "STA";
            instruction.addr_mode = ABSX;
            break;
        case 0x99:
            instruction.name = "STA";
            instruction.addr_mode = ABSY;
            break;
        case 0x81:
            instruction.name = "STA";
            instruction.addr_mode = INDX;
            break;
        case 0x91:
            instruction.name = "STA";
            instruction.addr_mode = INDY;
            break;
        // ---------- STX ----------
        case 0x86:
            instruction.name = "STX";
            instruction.addr_mode = ZP;
            break;
        case 0x96:
            instruction.name = "STX";
            instruction.addr_mode = ZPY;
            break;
        case 0x8E:
            instruction.name = "STX";
            instruction.addr_mode = ABS;
            break;
        // ---------- STY ----------
        case 0x84:
            instruction.name = "STY";
            instruction.addr_mode = ZP;
            break;
        case 0x94:
            instruction.name = "STY";
            instruction.addr_mode = ZPX;
            break;
        case 0x8C:
            instruction.name = "STY";
            instruction.addr_mode = ABS;
            break;
        // ---------- TAX ----------
        case 0xAA:
            instruction.name = "TAX";
            instruction.addr_mode = IMPL;
            break;
        // ---------- TAY ----------
        case 0xA8:
            instruction.name = "TAY";
            instruction.addr_mode = IMPL;
            break;
        // ---------- TSX ----------
        case 0xBA:
            instruction.name = "TSX";
            instruction.addr_mode = IMPL;
            break;
        // ---------- TXA ----------
        case 0x8A:
            instruction.name = "TXA";
            instruction.addr_mode = IMPL;
            break;
        // ---------- TXS ----------
        case 0x9A:
            instruction.name = "TXS";
            instruction.addr_mode = IMPL;
            break;
        // ---------- TYA ----------
        case 0x98:
            instruction.name = "TYA";
            instruction.addr_mode = IMPL;
            break;
        default:
            fprintf(stderr, "ERROR: Invalid opcode 0x%02x\n", instruction.opcode);
            break;
    }

    switch (instruction.addr_mode) {
        case IMPL:
            instruction.length = 1;
            break;
        case ACCUM:
            instruction.length = 1;
            break;
        case IMM:
            instruction.length = 2;
            instruction.imm.imm = mem[pc + 1];
            break;
        case ZP:
            instruction.length = 2;
            instruction.zp.addr = mem[pc + 1];
            break;
        case ZPX:
            instruction.length = 2;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case ZPY:
            instruction.length = 2;
            instruction.zpy.addr = mem[pc + 1];
            break;
        case REL:
            instruction.length = 2;
            instruction.rel.offset = mem[pc + 1];
            break;
        case ABS:
            instruction.length = 3;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case ABSX:
            instruction.length = 3;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case ABSY:
            instruction.length = 3;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case IND:
            instruction.length = 3;
            instruction.ind.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case INDX:
            instruction.length = 2;
            instruction.indx.addr = mem[pc + 1];
            break;
        case INDY:
            instruction.length = 2;
            instruction.indy.addr = mem[pc + 1];
            break;
    }

    return instruction;
}

/**
 * Execute the given instruction and set the appropriate flags
 *
 * @param instr - The instruction to execute
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void execute_instruction(Instruction instr, uint8_t** mem, Processor* processor) {
    uint16_t val;

    switch (instr.opcode) {
        // ---------- ADC ----------
        case 0x69:  // Immediate
        case 0x65:  // Zero Page
        case 0x75:  // Zero Page X
        case 0x6D:  // Absolute
        case 0x7D:  // Absolute X
        case 0x79:  // Absolute Y
        case 0x61:  // Indirect X (Indexed Indirect)
        case 0x71:  // Indirect Y (Indirect Indexed)
            val = processor->A + get_val(instr, *mem, *processor);

            // Set the "Carry" flag
            if (val >= 256) {
                val -= 256;
                set_flag('C', 1, processor);
            }

            processor->A = (uint8_t)val;
            break;
        // ---------- LDA ----------
        case 0xA9:  // Immediate
        case 0xA5:  // Zero Page
        case 0xB5:  // Zero Page X
        case 0xAD:  // Absolute
        case 0xBD:  // Absolute X
        case 0xB9:  // Absolute Y
        case 0xA1:  // Indirect X (Indexed Indirect)
        case 0xB1:  // Indirect Y (Indirect Indexed)
            val = get_val(instr, *mem, *processor);

            // Set the "Zero" flag
            if (val == 0) {
                set_flag('Z', 1, processor);
            } else {
                set_flag('Z', 0, processor);
            }

            // Set the "Negative flag"
            if ((val & 0x80) >> 7 == 1) {
                set_flag('N', 1, processor);
            } else {
                set_flag('N', 0, processor);
            }

            processor->A = val;
            break;
        // ---------- STA ----------
        case 0x85:  // Zero Page
        case 0x95:  // Zero Page X
        case 0x8D:  // Absolute
        case 0x9D:  // Absolute X
        case 0x99:  // Absolute Y
        case 0x81:  // Indirect X
        case 0x91:  // Indirect Y
            (*mem)[get_addr(instr, *mem, *processor)] = processor->A;
            break;
    }
}

/**
 * Set the specified flag to the specified value
 *
 * @param flag - The flag to set (N, V, B, D, I, Z, C)
 * @param val - The bit value to set the given flag to
 */
void set_flag(char flag, bool val, Processor* processor) {
    switch (flag) {
        case 'N':
            if (val == 1) {
                processor->P |= 0b10000000;
            } else {
                processor->P &= 0b01111111;
            }
            break;
        case 'V':
            if (val == 1) {
                processor->P |= 0b01000000;
            } else {
                processor->P &= 0b10111111;
            }
            break;
        case 'B':
            if (val == 1) {
                processor->P |= 0b00010000;
            } else {
                processor->P &= 0b11101111;
            }
            break;
        case 'D':
            if (val == 1) {
                processor->P |= 0b00001000;
            } else {
                processor->P &= 0b11110111;
            }
            break;
        case 'I':
            if (val == 1) {
                processor->P |= 0b00000100;
            } else {
                processor->P &= 0b11111011;
            }
            break;
        case 'Z':
            if (val == 1) {
                processor->P |= 0b00000010;
            } else {
                processor->P &= 0b11111101;
            }
            break;
        case 'C':
            if (val == 1) {
                processor->P |= 0b00000001;
            } else {
                processor->P &= 0b11111110;
            }
            break;
        default:
            fprintf(stderr, "ERROR: Invalid flag %c", flag);
            break;
    }
}

/**
 * Return the required value based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The byte of data required by the instruction
 */
uint8_t get_val(Instruction instr, uint8_t* mem, Processor processor) {
    uint8_t val = 0;
    uint8_t lsb_addr;
    uint8_t msb_addr;

    switch (instr.addr_mode) {
        case IMM:
            val = instr.imm.imm;
            break;
        case ZP:
            val = mem[instr.zp.addr];
            break;
        case ZPX:
            val = mem[instr.zpx.addr + processor.X];
            break;
        case ZPY:
            // TODO: Implement get_val for ZPY
            break;
        case REL:
            // TODO: Implement get_val for REL
        case ABS:
            val = mem[instr.abs.addr];
            break;
        case ABSX:
            val = mem[instr.absx.addr + processor.X];
            break;
        case ABSY:
            val = mem[instr.absy.addr + processor.Y];
            break;
        case IND:
            // TODO: Implement get_val for IND
            break;
        case INDX:
            lsb_addr = mem[instr.indx.addr + processor.X];
            msb_addr = mem[instr.indx.addr + processor.X + 1];
            val = mem[concatenate_bytes(msb_addr, lsb_addr)];
            break;
        case INDY:
            lsb_addr = mem[instr.indy.addr];
            msb_addr = mem[instr.indy.addr + 1];
            val = mem[concatenate_bytes(msb_addr, lsb_addr) + processor.Y];
            break;
        default:
            fprintf(stderr, "ERROR: Addressing mode doesn't return a value");
            break;
    }

    return val;
}

/**
 * Return the required memory address based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The memory address required by the instruction
 */
uint16_t get_addr(Instruction instr, uint8_t* mem, Processor processor) {
    uint16_t addr = 0;
    uint8_t lsb_addr;
    uint8_t msb_addr;

    switch (instr.addr_mode) {
        case ZP:
            addr = instr.zp.addr;
            break;
        case ZPX:
            addr = instr.zpx.addr + processor.X;
            break;
        case ZPY:
            // TODO: Implement get_addr for ZPY
            break;
        case REL:
            // TODO: Implement get_addr for REL
        case ABS:
            addr = instr.abs.addr;
            break;
        case ABSX:
            addr = instr.absx.addr + processor.X;
            break;
        case ABSY:
            addr = instr.absy.addr + processor.Y;
            break;
        case IND:
            // TODO: Implement get_addr for IND
            break;
        case INDX:
            lsb_addr = mem[instr.indx.addr + processor.X];
            msb_addr = mem[instr.indx.addr + processor.X + 1];
            addr = concatenate_bytes(msb_addr, lsb_addr);
            break;
        case INDY:
            lsb_addr = mem[instr.indy.addr];
            msb_addr = mem[instr.indy.addr + 1];
            addr = concatenate_bytes(msb_addr, lsb_addr) + processor.Y;
            break;
        default:
            fprintf(stderr, "ERROR: Instruction doesn't support that addressing mode");
            break;
    }

    return addr;
}
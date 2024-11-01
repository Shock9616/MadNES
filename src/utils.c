#include "utils.h"

#include "types.h"

#include <stdint.h>
#include <stdio.h>

uint16_t concatenate_bytes(uint16_t ms_byte, uint16_t ls_byte);

/**
 * Parses the location in memory pointed to by 'pc' and creates an Instruction
 *
 * @param mem - The byte array serving as system memory
 * @param pc - The program counter (points to the current instruction in memory)
 *
 * @returns An Instruction with all the information needed about it
 */
Instruction parse_instruction(uint8_t* mem, Register pc) {
    Instruction instruction;

    instruction.opcode = mem[pc];

    switch (instruction.opcode) {
        // ---------- ADC ----------
        case 0x69:  // nice
            instruction.name = "ADC";
            instruction.addr_mode = IMM;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0x65:
            instruction.name = "ADC";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x75:
            instruction.name = "ADC";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x6D:
            instruction.name = "ADC";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x7D:
            instruction.name = "ADC";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x79:
            instruction.name = "ADC";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x61:
            instruction.name = "ADC";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0x71:
            instruction.name = "ADC";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- AND ----------
        case 0x29:
            instruction.name = "AND";
            instruction.addr_mode = IMM;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0x25:
            instruction.name = "AND";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x35:
            instruction.name = "AND";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x2D:
            instruction.name = "AND";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x3D:
            instruction.name = "AND";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x39:
            instruction.name = "AND";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x21:
            instruction.name = "AND";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0x31:
            instruction.name = "AND";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- ASL ----------
        case 0x0A:
            instruction.name = "ASL";
            instruction.addr_mode = ACCUM;
            break;
        case 0x06:
            instruction.name = "ASL";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x16:
            instruction.name = "ASL";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x0E:
            instruction.name = "ASL";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x1E:
            instruction.name = "ASL";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        // ---------- BCC ----------
        case 0x90:
            instruction.name = "BCC";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BCS ----------
        case 0xB0:
            instruction.name = "BCS";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BEQ ----------
        case 0xF0:
            instruction.name = "BEQ";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BIT ----------
        case 0x24:
            instruction.name = "BIT";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x2C:
            instruction.name = "BIT";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        // ---------- BMI ----------
        case 0x30:
            instruction.name = "BMI";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BNE ----------
        case 0xD0:
            instruction.name = "BNE";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BPL ----------
        case 0x10:
            instruction.name = "BPL";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
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
            instruction.rel.offset = mem[pc + 1];
            break;
        // ---------- BVS ----------
        case 0x70:
            instruction.name = "BVS";
            instruction.addr_mode = REL;
            instruction.rel.offset = mem[pc + 1];
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
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0xC5:
            instruction.name = "CMP";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xD5:
            instruction.name = "CMP";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0xCD:
            instruction.name = "CMP";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xDD:
            instruction.name = "CMP";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xD9:
            instruction.name = "CMP";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xC1:
            instruction.name = "CMP";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0xD1:
            instruction.name = "CMP";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- CPX ----------
        case 0xE0:
            instruction.name = "CPX";
            instruction.addr_mode = IMM;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0xE4:
            instruction.name = "CPX";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xEC:
            instruction.name = "CPX";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        // ---------- CPY ----------
        case 0xC0:
            instruction.name = "CPY";
            instruction.addr_mode = IMM;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0xC4:
            instruction.name = "CPY";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xCC:
            instruction.name = "CPY";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        // ---------- DEC ----------
        case 0xC6:
            instruction.name = "DEC";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xD6:
            instruction.name = "DEC";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0xCE:
            instruction.name = "DEC";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xDE:
            instruction.name = "DEC";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
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
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0x45:
            instruction.name = "EOR";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x55:
            instruction.name = "EOR";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x4D:
            instruction.name = "EOR";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x5D:
            instruction.name = "EOR";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x59:
            instruction.name = "EOR";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x41:
            instruction.name = "EOR";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0x51:
            instruction.name = "EOR";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- INC ----------
        case 0xE6:
            instruction.name = "INC";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xF6:
            instruction.name = "INC";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0xEE:
            instruction.name = "INC";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xFE:
            instruction.name = "INC";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
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
        // ---------- LDA ----------
        case 0xA9:
            instruction.name = "LDA";
            instruction.addr_mode = IMM;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0xA5:
            instruction.name = "LDA";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xB5:
            instruction.name = "LDA";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0xAD:
            instruction.name = "LDA";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xBD:
            instruction.name = "LDA";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xB9:
            instruction.name = "LDA";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xA1:
            instruction.name = "LDA";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0xB1:
            instruction.name = "LDA";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- STA ----------
        case 0x85:
            instruction.name = "STA";
            instruction.addr_mode = ZP;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x95:
            instruction.name = "STA";
            instruction.addr_mode = ZPX;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x8D:
            instruction.name = "STA";
            instruction.addr_mode = ABS;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x9D:
            instruction.name = "STA";
            instruction.addr_mode = ABSX;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x99:
            instruction.name = "STA";
            instruction.addr_mode = ABSY;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x81:
            instruction.name = "STA";
            instruction.addr_mode = INDX;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0x91:
            instruction.name = "STA";
            instruction.addr_mode = INDY;
            instruction.indy.addr = mem[pc + 1];
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
            break;
        case ZP:
            instruction.length = 2;
            break;
        case ZPX:
            instruction.length = 2;
            break;
        case ZPY:
            instruction.length = 2;
            break;
        case REL:
            instruction.length = 2;
            break;
        case ABS:
            instruction.length = 3;
            break;
        case ABSX:
            instruction.length = 3;
            break;
        case ABSY:
            instruction.length = 3;
            break;
        case IND:
            instruction.length = 3;
            break;
        case INDX:
            instruction.length = 2;
            break;
        case INDY:
            instruction.length = 2;
            break;
    }

    return instruction;
}

/**
 * Prints the given instruction as it would be written in 6502 assembly
 *
 * @param instr - The instruction to print
 */
void print_instruction(Instruction instr) {
    switch (instr.addr_mode) {
        case IMPL:
            printf(IMPL_FORMAT, instr.name);
            break;
        case ACCUM:
            printf(ACCUM_FORMAT, instr.name);
            break;
        case IMM:
            printf(IMM_FORMAT, instr.name, instr.imm.imm);
            break;
        case ZP:
            printf(ZP_FORMAT, instr.name, instr.zp.addr);
            break;
        case ZPX:
            printf(ZPX_FORMAT, instr.name, instr.zpx.addr);
            break;
        case ZPY:
            printf(ZPY_FORMAT, instr.name, instr.zpy.addr);
            break;
        case REL:
            printf(REL_FORMAT, instr.name, instr.rel.offset);
            break;
        case ABS:
            printf(ABS_FORMAT, instr.name, instr.abs.addr);
            break;
        case ABSX:
            printf(ABSX_FORMAT, instr.name, instr.absx.addr);
            break;
        case ABSY:
            printf(ABSY_FORMAT, instr.name, instr.absy.addr);
            break;
        case IND:
            printf(IND_FORMAT, instr.name, instr.ind.addr);
            break;
        case INDX:
            printf(INDX_FORMAT, instr.name, instr.indx.addr);
            break;
        case INDY:
            printf(INDY_FORMAT, instr.name, instr.indy.addr);
            break;
    }
}

/**
 * Concatenate the most siginificant byte and the least significant byte
 *
 * @param ms_byte - The most significant byte (e.x. 1010 -> 1010xxxx)
 * @param ls_byte - The least significant byte (e.x. 0101 -> xxxx0101)
 *
 * @returns The two bytes concatenated (e.x. ms_byte = 1010, ls_byte = 0101 returns 10100101)
 */
uint16_t concatenate_bytes(uint16_t ms_byte, uint16_t ls_byte) {
    return (ms_byte << 8) | ls_byte;
}

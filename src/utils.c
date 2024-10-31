#include "utils.h"

#include "types.h"

#include <stdint.h>
#include <stdio.h>

uint16_t concatenate_bytes(uint16_t ms_byte, uint16_t ls_byte) {
    return (ms_byte << 8) | ls_byte;
}

Instruction parse_instruction(uint8_t* mem, Register pc) {
    Instruction instruction;

    instruction.opcode = mem[pc];

    switch (instruction.opcode) {
        // ---------- LDA ----------
        case 0xA9:
            instruction.name = "LDA";
            instruction.addr_mode = IMM;
            instruction.length = 2;
            instruction.imm.imm = mem[pc + 1];
            break;
        case 0xA5:
            instruction.name = "LDA";
            instruction.addr_mode = ZP;
            instruction.length = 2;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0xB5:
            instruction.name = "LDA";
            instruction.addr_mode = ZPX;
            instruction.length = 2;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0xAD:
            instruction.name = "LDA";
            instruction.addr_mode = ABS;
            instruction.length = 3;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xBD:
            instruction.name = "LDA";
            instruction.addr_mode = ABSX;
            instruction.length = 3;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xB9:
            instruction.name = "LDA";
            instruction.addr_mode = ABSY;
            instruction.length = 3;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0xA1:
            instruction.name = "LDA";
            instruction.addr_mode = INDX;
            instruction.length = 2;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0xB1:
            instruction.name = "LDA";
            instruction.addr_mode = INDY;
            instruction.length = 2;
            instruction.indy.addr = mem[pc + 1];
            break;
        // ---------- STA ----------
        case 0x85:
            instruction.name = "STA";
            instruction.addr_mode = ZP;
            instruction.length = 2;
            instruction.zp.addr = mem[pc + 1];
            break;
        case 0x95:
            instruction.name = "STA";
            instruction.addr_mode = ZPX;
            instruction.length = 2;
            instruction.zpx.addr = mem[pc + 1];
            break;
        case 0x8D:
            instruction.name = "STA";
            instruction.addr_mode = ABS;
            instruction.length = 3;
            instruction.abs.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x9D:
            instruction.name = "STA";
            instruction.addr_mode = ABSX;
            instruction.length = 3;
            instruction.absx.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x99:
            instruction.name = "STA";
            instruction.addr_mode = ABSY;
            instruction.length = 3;
            instruction.absy.addr = concatenate_bytes(mem[pc + 2], mem[pc + 1]);
            break;
        case 0x81:
            instruction.name = "STA";
            instruction.addr_mode = INDX;
            instruction.length = 2;
            instruction.indx.addr = mem[pc + 1];
            break;
        case 0x91:
            instruction.name = "STA";
            instruction.addr_mode = INDY;
            instruction.length = 2;
            instruction.indy.addr = mem[pc + 1];
            break;
        default:
            fprintf(stderr, "ERROR: Invalid opcode 0x%02x\n", instruction.opcode);
            break;
    }

    return instruction;
}

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

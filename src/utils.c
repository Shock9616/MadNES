#include "utils.h"

#include "types.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * Prints the given instruction as it would be written in 6502 assembly
 *
 * @param instr - The instruction to print
 */
void printInstruction(Instruction instr) {
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
uint16_t concatenateBytes(uint16_t ms_byte, uint16_t ls_byte) {
    return (ms_byte << 8) | ls_byte;
}

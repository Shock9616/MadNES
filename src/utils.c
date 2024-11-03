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

/**
 * Load a byte of memory from the given address in memory
 *
 * @param addr - The address in memory to load the value of
 * @param mem - The byte array serving as system memory
 *
 * @returns The byte of data stored at the given address
 */
uint8_t load(uint16_t addr, uint8_t* mem) {
    return mem[addr];
}

/**
 * Store a byte of memory to the given address in memory
 *
 * @param addr - The address in memory to store to
 * @param val - The value to store to memory
 * @param mem - The byte array serving as system memory
 */
void store(uint16_t addr, uint8_t val, uint8_t** mem) {
    *mem[addr] = val;
}

#include "types.h"

#include <stdint.h>

// String formats to be filled in when printing 6502 instructions
#define IMPL_FORMAT  "%s\n"
#define ACCUM_FORMAT "%s  A\n"
#define IMM_FORMAT   "%s  #$%02x\n"
#define ZP_FORMAT    "%s  $%02x\n"
#define ZPX_FORMAT   "%s  $%02x,X\n"
#define ZPY_FORMAT   "%s  $%02x,Y\n"
#define REL_FORMAT   "%s  *+%02x\n"
#define ABS_FORMAT   "%s  $%04x\n"
#define ABSX_FORMAT  "%s  $%04x,X\n"
#define ABSY_FORMAT  "%s  $%04x,Y\n"
#define IND_FORMAT   "%s  ($%04x)\n"
#define INDX_FORMAT  "%s  ($%02x,X)\n"
#define INDY_FORMAT  "%s  ($%02x,Y)\n"

/**
 * Parses the location in memory pointed to by 'pc' and creates an Instruction
 *
 * @param mem - The byte array serving as system memory
 * @param pc - The program counter (points to the current instruction in memory)
 *
 * @returns An Instruction with all the information needed about it
 */
Instruction parse_instruction(uint8_t* mem, Register pc);

/**
 * Prints the given instruction as it would be written in 6502 assembly
 *
 * @param instr - The instruction to print
 */
void print_instruction(Instruction instr);

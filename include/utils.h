#include "types.h"

// String formats to be filled in when printing 6502 instructions
#define IMPL_FORMAT  "%s\n"
#define ACCUM_FORMAT "%s A\n"
#define IMM_FORMAT   "%s #$%02x\n"
#define ZP_FORMAT    "%s $%02x\n"
#define ZPX_FORMAT   "%s $%02x,X\n"
#define ZPY_FORMAT   "%s $%02x,Y\n"
#define REL_FORMAT   "%s *+%02x\n"
#define ABS_FORMAT   "%s $%04x\n"
#define ABSX_FORMAT  "%s $%04x,X\n"
#define ABSY_FORMAT  "%s $%04x,Y\n"
#define IND_FORMAT   "%s ($%04x)\n"
#define INDX_FORMAT  "%s ($%02x,X)\n"
#define INDY_FORMAT  "%s ($%02x),Y\n"

/**
 * Prints the given instruction as it would be written in 6502 assembly
 *
 * @param instr - The instruction to print
 */
void printInstruction(Instruction instr);

/**
 * Concatenate the most siginificant byte and the least significant byte
 *
 * @param ms_byte - The most significant byte (e.x. 1010 -> 1010xxxx)
 * @param ls_byte - The least significant byte (e.x. 0101 -> xxxx0101)
 *
 * @returns The two bytes concatenated (e.x. ms_byte = 1010, ls_byte = 0101 returns 10100101)
 */
uint16_t concatenateBytes(uint16_t ms_byte, uint16_t ls_byte);

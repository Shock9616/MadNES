#include "types.h"

#include <stdint.h>

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

Instruction parse_instruction(uint8_t* mem, Register pc);
void print_instruction(Instruction instr);

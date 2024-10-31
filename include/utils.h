#include "types.h"

#include <stdint.h>

#define IMM_FORMAT "%s  #$%02x\n"
#define ABS_FORMAT "%s  $%04x\n"

Instruction parse_instruction(uint8_t* mem, Register pc);
void print_instruction(Instruction instr);

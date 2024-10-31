#ifndef P6502_H
#define P6502_H

#include "types.h"

#include <stdint.h>

void decode_instruction(Byte instr_byte, Byte* mem);
void execute_instruction(Word instruction_bits, Processor* processor, Byte* memory);
void store(Byte* memory, Address address, Word value);
Word load(Byte* memory, Address address);

#endif

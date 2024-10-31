#include "6502.h"

#include "types.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void decode_instruction(Byte instr_byte, Byte* mem) {
    Instruction instruction = parse_instruction(instr_byte, mem);
    switch (instruction.opcode) {
        case 0xA9:
            // LDA (Immediate)
            printf(IMM_FORMAT, "LDA", instruction.arg);
            break;
        case 0x8D:
            // STA (Absolute)
            printf(ABS_FORMAT, "STA", instruction.arg);
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

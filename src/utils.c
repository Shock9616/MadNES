#include "utils.h"

#include "types.h"

#include <stdint.h>
#include <stdio.h>

Instruction parse_instruction(uint8_t* mem, Register pc) {
    Instruction instruction;

    instruction.opcode = mem[pc];

    switch (instruction.opcode) {
        // ----- LDA -----
        case 0xA9:
            // Immediate addressing
            instruction.addr_mode = IMM;
            instruction.name = "LDA";
            instruction.length = 2;

            instruction.imm.imm = mem[pc + 1];
            break;
        // ----- STA -----
        case 0x8D:
            // Absolute addressing
            instruction.addr_mode = ABS;
            instruction.name = "STA";
            instruction.length = 3;

            uint16_t ls_byte = mem[pc + 1];
            uint16_t ms_byte = mem[pc + 2];
            instruction.abs.addr = (ms_byte << 8) | ls_byte;
            break;
        default:
            fprintf(stderr, "ERROR: Invalid opcode 0x%02x\n", instruction.opcode);
            break;
    }

    return instruction;
}

void print_instruction(Instruction instr) {
    switch (instr.addr_mode) {
        case IMM:
            printf(IMM_FORMAT, instr.name, instr.imm.imm);
            break;
        case ABS:
            printf(ABS_FORMAT, instr.name, instr.abs.addr);
            break;
        default:
            break;
    }
}

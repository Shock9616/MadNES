#include "utils.h"

#include "types.h"

#include <stdio.h>
#include <stdlib.h>

int sign_extend_number(unsigned int field, unsigned int n) {
    return (int)(field << (16 - n)) >> (16 - n);
}

Instruction parse_instruction(Byte instr_byte, uint8_t* mem) {
    Instruction instruction;

    instruction.opcode = opcode_bits;

    switch (instruction.opcode) {
        case 0xA9:
            instruction.length = 2;
            break;
        case 0x8D:
            instruction.length = 3;
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }

    return instruction;
}

void print_instruction(char* name, Byte arg, ADDR_MODE addr_mode) {
    switch (addr_mode) {
        case ACCUM:
            printf(ACCUM_FORMAT, name);
            break;
        case ABS:
            printf(ABS_FORMAT, name, arg);
            break;
        case ABSX:
            printf(ABSX_FORMAT, name, arg);
            break;
        case ABSY:
            printf(ABSY_FORMAT, name, arg);
            break;
        case IMM:
            printf(IMM_FORMAT, name, arg);
            break;
        case IMPL:
            printf(IMPL_FORMAT, name);
            break;
        case IND:
            printf(IND_FORMAT, name, arg);
            break;
        case INDX:
            printf(INDX_FORMAT, name, arg);
            break;
        case INDY:
            printf(INDY_FORMAT, name, arg);
            break;
        case REL:
            printf(REL_FORMAT, name, arg);
            break;
        case ZP:
            printf(ZP_FORMAT, name, arg);
            break;
        case ZPX:
            printf(ZPX_FORMAT, name, arg);
            break;
        case ZPY:
            printf(ZPY_FORMAT, name, arg);
            break;
    }
}

void handle_invalid_instruction(Instruction instruction) {
    printf("ERROR: Invalid instruction: 0x%04x\n", instruction.opcode);
}

void handle_invalid_read(Address address) {
    printf("ERROR: Bad read. Address: 0x%04x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("ERROR: Bad write. Address: 0x%04x\n", address);
    exit(-1);
}

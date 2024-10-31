#include "6502.h"
#include "types.h"

#include <_string.h>
#include <assert.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Byte* memory;
#define MAX_SIZE 64000

void execute(Processor* processor, int print) {
    // uint32_t instruction_bits = load(memory, processor->PC);
    // execute_instruction(instruction_bits, processor, memory);
}

int load_file(uint8_t* mem, size_t memsize, int startaddr, const char* filename, int disassemble) {
    FILE* file = fopen(filename, "r");
    printf("%s", filename);
    char line[MAX_SIZE];
    int programsize = 0;
    int offset = 0;
    Byte instr_byte;

    if (!file) {
        perror("File open failed");
        return -1;
    }

    while (fgets(line, MAX_SIZE, file) != NULL) {
        // Read 8-bit instruction as a hexadecimal value
        instr_byte = (Byte)strtol(line, NULL, 16);
        programsize++;

        // Store the single byte at the appropriate memory location
        mem[startaddr + offset] = instr_byte;

        if (disassemble) {
            printf("%04x: ", startaddr + offset);
            decode_instruction((Byte)instr_byte, mem);
        }

        offset += 1;  // Move by 1 byte for 8-bit instructions
    }

    fclose(file);
    return programsize;
}

int main(int argc, char** argv) {
    int opt_disassemble = 0, opt_run = 0;

    Processor processor;
    // Zero out all registers
    processor.A = 0;
    processor.X = 0;
    processor.Y = 0;
    processor.PC = 0;
    processor.SP = 0x01FF;  // Set the stack pointer to the top of the memory array

    int c;
    while ((c = getopt(argc, argv, "dvrites:a:")) != -1) {
        switch (c) {
            case 'd':
                opt_disassemble = 1;
                break;
            case 'r':
                opt_run = 1;
                break;
            default:
                fprintf(stderr, "ERROR: Invalid flag %c\n", c);
                return -1;
        }
    }

    if (argc <= optind) {
        fprintf(stderr, "ERROR: Please provide a path to an executable\n");
        return -1;
    }

    // Load executable into memory
    assert(memory == NULL);
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t));
    assert(memory != NULL);
    int prog_instr_count = 0;
    processor.PC = 0x0600;

    prog_instr_count = load_file(memory, MEMORY_SPACE, processor.PC, argv[optind], opt_disassemble);

    if (opt_disassemble == 1) {
        return 0;
    } else if (opt_run == 1) {
        int sim_instr_count = 0;
        while (sim_instr_count < prog_instr_count) {
        }
        printf("Run\n");
    } else {
        printf("No valid arguments provided\n");
    }

    printf("Done!\n");
    return 0;
}

#include "6502.h"
#include "types.h"
#include "utils.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Pointer to simulator memory
uint8_t* memory;
#define MAX_SIZE 50

int loadFile(uint8_t* mem, int start_addr, const char* file_path);

int main(int argc, char** argv) {
    bool opt_disassemble = false, opt_run = false;

    Processor processor;
    // Set registers to default values
    processor.PC = 0x0600;
    processor.S = 0xFF;
    processor.P = 0x40;
    processor.A = 0x0;
    processor.X = 0x0;
    processor.Y = 0x0;

    char* data_file = NULL;

    int arg;
    while ((arg = getopt(argc, argv, "d:r:")) != -1) {
        switch (arg) {
            case 'd':
                opt_disassemble = true;
                data_file = optarg;
                break;
            case 'r':
                opt_run = true;
                data_file = optarg;
                break;
            default:
                fprintf(stderr, "ERROR: Invalid argument: %c\n", optopt);
                return -1;
        }
    }

    // Allocate system memory
    assert(memory == NULL);
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t));
    assert(memory != NULL);
    int prog_line_count = 0;

    if (data_file != NULL) {
        prog_line_count = loadFile(memory, processor.PC, data_file);
    }

    if (opt_disassemble) {
        while (prog_line_count > 0) {
            Instruction instr = parse_instruction(memory, processor.PC);
            print_instruction(instr);
            processor.PC += instr.length;
            prog_line_count -= instr.length;
        }
    }
    if (opt_run) {
        memory[0x24] = 0x74;
        memory[0x25] = 0x10;
        memory[0x1074] = 10;
        processor.X = 4;

        while (processor.PC < 0x0600 + prog_line_count) {
            Instruction instr = parse_instruction(memory, processor.PC);
            execute_instruction(instr, &memory, &processor);
            processor.PC += instr.length;
        }

        printf("A: 0x%02x\n", processor.A);
        printf("X: 0x%02x\n", processor.X);
        printf("Y: 0x%02x\n", processor.Y);
        printf("P: 0x%02x\n", processor.P);
    }
}

/**
 * Load the contents of a file into emulated system memory
 *
 * @param mem - The byte array to load instructions into.
 * @param start_addr - The start address of the code block in memory.
 * @param file_path - The path to the file to load into memory
 *
 * @returns The number of lines in the file
 */
int loadFile(uint8_t* mem, int start_addr, const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file.\n");
        return -1;
    }

    char line[MAX_SIZE];
    int offset = 0;
    int program_size = 0;

    while (fgets(line, MAX_SIZE, file) != NULL) {
        uint8_t byte = (uint8_t)strtol(line, NULL, 16);
        mem[start_addr + offset] = byte;
        offset++;
        program_size++;
    }

    fclose(file);
    return program_size;
}

#include "types.h"

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
    processor.A = 0x0;
    processor.X = 0x0;
    processor.Y = 0x0;
    processor.SP = 0xFF;
    processor.PC = 0x0600;

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
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t));  // allocate zeroed memory
    assert(memory != NULL);
    int prog_instr_count = 0;

    if (data_file != NULL) {
        prog_instr_count = loadFile(memory, processor.PC, data_file);
    }

    if (opt_disassemble) {
        for (int i = 0; i < prog_instr_count; i++) {
            printf("0x%02x\n", memory[processor.PC + i]);
        }
    }
    if (opt_run) {
        printf("Run: %s\n", data_file);
    }
}

/**
 * Load the contents of a file into emulated system memory
 *
 * @param mem - The byte array to load instructions into.
 * @param start_addr - The start address of the code block in memory.
 * @param file_path - The path to the file to load into memory
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

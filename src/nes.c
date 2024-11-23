#include "6502.h"
#include "cartridge.h"
#include "types.h"
#include "utils.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Pointer to simulator memory
uint8_t* memory;
#define MAX_SIZE 50

int loadFile(uint8_t* mem, int start_addr, const char* file_path);
int intToBin(uint8_t n);

#ifndef TEST

int main(int argc, char** argv) {
    bool opt_disassemble = false, opt_run = false, opt_cart = false;

    Processor processor;
    // Set registers to default values
    processor.PC = 0x0600;
    processor.S = 0xFF;
    processor.P = 0x30;
    processor.A = 0x0;
    processor.X = 0x0;
    processor.Y = 0x0;

    Cartridge cartridge;
    cartridge.trainer = NULL;
    cartridge.prg_rom = NULL;
    cartridge.chr_rom = NULL;

    char* data_file = NULL;
    char* rom_file = NULL;

    int arg;
    while ((arg = getopt(argc, argv, "d:r:c:")) != -1) {
        switch (arg) {
            case 'd':
                opt_disassemble = true;
                data_file = optarg;
                break;
            case 'r':
                opt_run = true;
                data_file = optarg;
                break;
            case 'c':
                opt_cart = true;
                rom_file = optarg;
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
            Instruction instr = parseInstruction(memory, processor.PC);
            printInstruction(instr);
            processor.PC += instr.length;
            prog_line_count -= instr.length;
        }
    }
    if (opt_run) {
        processor.halted = false;

        while (processor.PC != 0x0600 + prog_line_count && !processor.halted) {
            Instruction instr = parseInstruction(memory, processor.PC);
            executeInstruction(instr, &memory, &processor);
            processor.PC += instr.length;
        }

        printf("-------- Debug Output --------\n");
        printf("     A=$%02x  X=$%02x  Y=$%02x\n", processor.A, processor.X, processor.Y);
        printf("      SP=$%02x  PC=$%04x\n", processor.S, processor.PC);
        printf("          NV-BDIZC\n");
        printf("          %08d\n", intToBin(processor.P));
        printf("------------------------------\n");
    }
    if (opt_cart) {
        int result = loadRom(&cartridge, rom_file);
        if (result != 0) {
            fprintf(stderr, "ERROR: Failed to load rom (error code %d)\n", result);
            return EXIT_FAILURE;
        }

        printCartMetadata(&cartridge);

        // Print preview of instructions in PRG-ROM
        printf("\n------------ PRG-ROM Preview -----------\n");
        uint16_t fake_PC = 0;
        while (fake_PC < 10) {
            Instruction instr = parseInstruction(cartridge.prg_rom, fake_PC);
            printInstruction(instr);
            fake_PC += instr.length;
        }
        printf("----------------------------------------\n");
    }

    // Free dynamic memory after run
    if (memory) {
        free(memory);
    }
    if (cartridge.trainer) {
        free(cartridge.trainer);
    }
    if (cartridge.prg_rom) {
        free(cartridge.prg_rom);
    }
    if (cartridge.chr_rom) {
        free(cartridge.chr_rom);
    }

    return EXIT_SUCCESS;
}

#endif

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

/**
 * Convert the given 8-bit integer into a base 10 representation of its binary
 * form (e.x. 0xFF -> 11111111)
 * https://stackoverflow.com/a/5488546
 *
 * @param n - The integer to be converted
 *
 * @returns A base 10 representation of n's binary form
 */
int intToBin(uint8_t n) {
    return (n == 0 || n == 1 ? n : ((n % 2) + 10 * intToBin(n / 2)));
}

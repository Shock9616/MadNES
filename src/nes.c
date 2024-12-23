#include "6502.h"
#include "cartridge.h"
#include "logger.h"
#include "types.h"
#include "utils.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Pointer to simulator memory
uint8_t* memory;
#define MAX_SIZE 50

// Global cycle count variable for synchronizing the CPU, PPU, and APU
uint64_t cycles = 0;

int loadFile(uint8_t* mem, int start_addr, const char* file_path);
int intToBin(uint8_t n);
void addAdditionalCycles(Instruction* instr, Processor processor, uint16_t old_PC);
void delayCycles(int cycles);

#ifndef TEST

int main(int argc, char** argv) {
    bool opt_disassemble = false, opt_run = false, opt_cart = false, opt_emu = false;

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
    while ((arg = getopt(argc, argv, "d:r:c:e:")) != -1) {
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
            case 'e':
                opt_emu = true;
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
        // Disassemble a 6502 assembly hexdump
        while (prog_line_count > 0) {
            Instruction instr = parseInstruction(memory, processor.PC);
            printInstruction(instr);
            processor.PC += instr.length;
            prog_line_count -= instr.length;
        }
    }
    if (opt_run) {
        // Run a 6502 assembly program hexdump
        processor.halted = false;

        while (processor.PC != 0x0600 + prog_line_count && !processor.halted) {
            // Main loop
            Instruction instr = parseInstruction(memory, processor.PC);
            executeInstruction(instr, &memory, &processor);
            processor.PC += instr.length;
            cycles += instr.cycles;
        }

        printf("-------- Debug Output --------\n");
        printf("     A=$%02x  X=$%02x  Y=$%02x\n", processor.A, processor.X, processor.Y);
        printf("      SP=$%02x  PC=$%04x\n", processor.S, processor.PC);
        printf("          NV-BDIZC\n");
        printf("          %08d\n", intToBin(processor.P));
        printf("------------------------------\n");
    }
    if (opt_cart) {
        // View ROM file metadata and beginning of PRG-ROM
        int result = loadRom(&cartridge, rom_file);
        if (result != 0) {
            printLog("CART", "Failed to laod rom", "ERROR");
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
    if (opt_emu) {
        // Run full emulator
        printf("----------------------------------------\n");
        printf("     MadNES: A Crappy NES Emulator\n");
        printf("----------------------------------------\n\n");
        printLog("CART", "Loading ROM...", "INFO");

        int result = loadRom(&cartridge, rom_file);
        if (result != 0) {
            printLog("CART", "Failed to load rom", "ERROR");
            return EXIT_FAILURE;
        }

        printLog("CART", "Success! ROM data loaded!", "INFO");

        printf("\n");
        printCartMetadata(&cartridge);

        // Load the contents of PRG-ROM into memory
        printLog("CART", "Loading PRG-ROM into memory...", "INFO");
        int mapper_num = ((cartridge.flags8 & 0x0F) << 8) | ((cartridge.flags7 & 0xF0) << 4) |
                         (cartridge.flags6 & 0xF0);
        switch (mapper_num) {
            case 0:
                // Mapper 0
                for (int i = 0; i < cartridge.prg_rom_size * 16 * 1024; i++) {
                    memory[0x8000 + i] = cartridge.prg_rom[i];
                }
                printLog("CART", "Finished loading PRG-ROM into memory at $8000! (Mapper 0)",
                         "INFO");
                break;
            default:
                printf("INFO: Support for ROMs using mapper %d has not yet been implemented",
                       mapper_num);
                goto PROGRAM_EXIT;
        }

        uint16_t reset_vector = concatenateBytes(memory[0xFFFD], memory[0xFFFC]);
        processor.PC = reset_vector;

        printf("\n");
        char* prg_start_msg;
        asprintf(&prg_start_msg, "Beginning program execution at $%04x", processor.PC);
        printLog("CPU", prg_start_msg, "INFO");
        printf("\n");

        processor.halted = false;

        // Main loop
        while (!processor.halted) {
            uint16_t old_PC = processor.PC;

            Instruction instr = parseInstruction(memory, processor.PC);
            executeInstruction(instr, &memory, &processor);
            processor.PC += instr.length;

            // Add additional processor cycles if the instruction crosses a page
            addAdditionalCycles(&instr, processor, old_PC);

            cycles += instr.cycles;
            delayCycles(instr.cycles);

            // Logging messages
            printInstrLog(instr, processor);
        }
    }

PROGRAM_EXIT:
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

/**
 * Add additional cycles to the given instruction if it crosses a page
 *
 * @param instr - The instruction to potentially add cycles to
 * @param processor - The struct acting as the system processor
 * @param old_PC - The value of the PC before the instruction was executed
 */
void addAdditionalCycles(Instruction* instr, Processor processor, uint16_t old_PC) {
    // If the address mode is Absolute X, Absolute Y, Indirect Indexed, or
    // Relative, check if a page was crossed and add an extra cycle
    switch (instr->addr_mode) {
        uint16_t addr;
        case ABSX:
            addr = instr->addr;
            if (((addr + processor.X) & 0xFF00) != (addr & 0xFF00)) {
                instr->cycles++;
            }
            break;
        case ABSY:
            addr = instr->addr;
            if (((addr + processor.Y) & 0xFF00) != (addr & 0xFF00)) {
                instr->cycles++;
            }
            break;
        case INDY:
            addr = concatenateBytes(memory[instr->addr + 1], memory[instr->addr]);
            if (((addr + processor.Y) & 0xFF00) != (addr & 0xFF00)) {
                instr->cycles++;
            }
            break;
        case REL:
            if (processor.PC != old_PC + instr->length) {
                // Branch succeeded
                instr->cycles++;

                if ((processor.PC & 0xFF00) != (old_PC & 0xFF00)) {
                    // Page crossed
                    instr->cycles++;
                }
            }
        default:
            break;
    }
}

/**
 * Sleep for the given amount of time
 *
 * @param cycles - the number of cycles to delay for
 */
void delayCycles(int cycles) {
    double delay = (1.0 / 1789773) * cycles;

    struct timespec ts;
    ts.tv_sec = (time_t)delay;
    ts.tv_nsec = (long)((delay - ts.tv_sec) * 1e9);
    nanosleep(&ts, NULL);
}

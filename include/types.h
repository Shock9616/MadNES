#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SPACE (2000)  // 2kb of memory

// A 6502 processor has 5 registers: A, X, Y, the Stack Pointer, and the Program Counter
typedef struct {
    uint16_t PC;  // Program counter
    uint8_t S;    // Stack pointer
    uint8_t P;    // Status flags NV1BDIZC
    uint8_t A;    // Accumulator
    uint8_t X;    // Index register X
    uint8_t Y;    // Index register Y
    bool halted;  // Only used for debugging purposes
} Processor;

typedef struct {
    // CPU-visible registers
    uint8_t ctrl;    // PPUCTRL register (mapped to $2000, write)
    uint8_t mask;    // PPUMASK register (mapped to $2001, write)
    uint8_t status;  // PPUSTATUS register (mapped to $2002, read)

    uint8_t oam[256];  // Object Attribute Memory (64 sprites, 4 bytes each)
    uint8_t oam_addr;  // OAMADDR register (mapped to $2003, write)

    uint8_t data_buffer;  // PPUDATA (mapped to $2007 read + write)

    // Internal memory
    uint8_t vram[2048];   // Nametables and attribute tables
    uint8_t palette[32];  // Palette data (addrs $3F00-$3FFF, mirrored after $3F1F)

    // VRAM Addressing
    uint16_t v;  // Current VRAM addr
    uint16_t t;  // Temp VRAM addr
    uint8_t x;   // X scroll
    bool w;      // Write toggle

    // Timing
    int scanline;
    int cycle;

    // NMI
    bool nmi_occurred;
    bool nmi_output;
} PPU;

// All possible addressing modes in 6502 assembly
typedef enum {
    IMPL,
    ACCUM,
    IMM,
    ZP,
    ZPX,
    ZPY,
    REL,
    ABS,
    ABSX,
    ABSY,
    IND,
    INDX,
    INDY,
} AddrMode;

// Storing information about an instruction in a format that is easier to work with
typedef struct {
    uint8_t opcode;
    AddrMode addr_mode;
    char* name;  // The 3 letter mnemonic of the instruction

    // Possible types of arguments to an instruction
    uint8_t imm;
    uint16_t addr;
    int8_t offset;

    // Instruction metadata
    int length;  // Length of the instruction (in bytes)
    int cycles;  // The number of cycles the instruction takes to execute
} Instruction;

typedef struct {
    uint8_t magic_num[4];  // value of 0x4E45531A indicates a valid NES rom
    uint8_t prg_rom_size;  // 16 KB units
    uint8_t chr_rom_size;  // 8 KB units
    uint8_t flags6;
    uint8_t flags7;
    uint8_t flags8;
    uint8_t flags9;
    uint8_t flags10;
    uint8_t reserved[5];
    uint8_t* trainer;
    uint8_t* prg_rom;
    uint8_t* chr_rom;
} Cartridge;

#endif

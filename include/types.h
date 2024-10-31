#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define MEMORY_SPACE (2000)  // 2kb of memory

typedef uint16_t Register;

typedef struct {
    Register A;
    Register X;
    Register Y;
    Register SP;
    Register PC;
} Processor;

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

typedef struct {
    uint8_t opcode;
    AddrMode addr_mode;
    char* name;  // The 3 letter mnemonic of the instruction
    int length;  // Length of the instruction (in bytes)

    struct {
        uint8_t imm;
    } imm;

    // Zero Page addressing mode
    struct {
        uint8_t addr;
    } zp;

    // Zero Pagee X addressing mode
    struct {
        uint8_t addr;
    } zpx;

    // Zero Page Y addressing mode
    struct {
        uint8_t addr;
    } zpy;

    // Relative addressing mode
    struct {
        uint8_t offset;
    } rel;

    // Absolute addressing mode
    struct {
        uint16_t addr;
    } abs;

    // Absolute X addressing mode
    struct {
        uint16_t addr;
    } absx;

    // Absolute Y addressing mode
    struct {
        uint16_t addr;
    } absy;

    // Indirect addressing mode
    struct {
        uint16_t addr;
    } ind;

    // Indirect X addressing mode
    struct {
        uint8_t addr;
    } indx;

    // Indirect Y addressing mode
    struct {
        uint8_t addr;
    } indy;

} Instruction;

#endif

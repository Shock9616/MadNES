#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef uint8_t Byte;
typedef uint16_t Word;

// A memory address
typedef Word Address;

// A register value
typedef Word Register;

/*
 * The processor data:
 *     A (accumulator), X, and Y registers
 *     PC program counter
 */
typedef struct {
    Register A;
    Register X;
    Register Y;
    Register PC;
    Register SP;
} Processor;

#define MEMORY_SPACE 2000  // 2kb of Memory

typedef enum {
    ACCUM,
    ABS,
    ABSX,
    ABSY,
    IMM,
    IMPL,
    IND,
    INDX,
    INDY,
    REL,
    ZP,
    ZPX,
    ZPY,
} ADDR_MODE;

typedef struct {
    const char* name;
    int length;
    const ADDR_MODE addr_mode;
} Opcode;

typedef struct {
    Byte opcode : 1;
    Word arg : 1;
    int length;
    ADDR_MODE addr_mode;
} Instruction;

#endif

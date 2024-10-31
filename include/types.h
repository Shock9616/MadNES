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

#endif

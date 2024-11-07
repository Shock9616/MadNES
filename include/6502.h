#include "types.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * Parses the location in memory pointed to by 'pc' and creates an Instruction
 *
 * @param mem - The byte array serving as system memory
 * @param pc - The program counter (points to the current instruction in memory)
 *
 * @returns An Instruction with all the information needed about it
 */
Instruction parseInstruction(uint8_t* mem, uint16_t pc);

/**
 * Execute the given instruction and set the appropriate flags
 *
 * @param instr - The instruction to execute
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void executeInstruction(Instruction instr, uint8_t** mem, Processor* processor);

/**
 * Set the specified flag to the specified value
 *
 * @param flag - The flag to set (N, V, B, D, I, Z, C)
 * @param val - The bit value to set the given flag to
 */
void setFlag(char flag, bool val, Processor* processor);

/**
 * Return the value of the given status flag
 *
 * @param flag - The flag to return the status of (N, V, B, D, I, Z, C)
 * @param processor - The processor holding register values
 *
 * @returns The value of the given flag
 */
bool getFlag(char flag, Processor* processor);

/**
 * Return the required value based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The byte of data required by the instruction
 */
uint8_t getVal(Instruction instr, uint8_t* mem, Processor processor);

/**
 * Return the required memory address based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The memory address required by the instruction
 */
uint16_t getAddr(Instruction instr, uint8_t* mem, Processor processor);

/**
 * Push the given value onto the stack (0x01FF - 0x0100)
 *
 * @param val - The value to be pushed onto the stack
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void stackPush(uint8_t val, uint8_t** mem, Processor* processor);

/**
 * Return and remove the value at the top of the stack
 *
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
uint8_t stackPull(uint8_t** mem, Processor* processor);

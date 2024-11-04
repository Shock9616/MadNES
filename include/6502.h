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
Instruction parse_instruction(uint8_t* mem, uint16_t pc);

/**
 * Execute the given instruction and set the appropriate flags
 *
 * @param instr - The instruction to execute
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 */
void execute_instruction(Instruction instr, uint8_t** mem, Processor* processor);

/**
 * Set the specified flag to the specified value
 *
 * @param flag - The flag to set (N, V, B, D, I, Z, C)
 * @param val - The bit value to set the given flag to
 */
void set_flag(char flag, bool val, Processor* processor);

/**
 * Return the required value based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The byte of data required by the instruction
 */
uint8_t get_val(Instruction instr, uint8_t* mem, Processor processor);

/**
 * Return the required memory address based on the instruction's addressing mode
 *
 * @param instr - The instruction to use for getting the required value
 * @param mem - The byte array serving as system memory
 * @param processor - The processor holding register values
 *
 * @returns The memory address required by the instruction
 */
uint16_t get_addr(Instruction instr, uint8_t* mem, Processor processor);

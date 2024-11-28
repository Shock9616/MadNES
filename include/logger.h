#ifndef LOGGER
#define LOGGER

#include "types.h"

/**
 * Print a log message to the console
 *
 * @param component - The component of the NES the message is related to (e.x.
 *                    CPU, PPU, etc.)
 * @param msg - The log message
 */
void printLog(const char* component, const char* msg, const char* level);

/**
 * Print a log message about an executed instruction and the processor status to
 * the console
 *
 * @param instr - The executed instruction
 * @param processor - The struct serving as the system procesor
 */
void printInstrLog(Instruction instr, Processor processor);
#endif

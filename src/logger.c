#include "logger.h"

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Print a log message to the console
 *
 * @param component - The component of the NES the message is related to (e.x.
 *                    CPU, PPU, etc.)
 * @param msg - The log message
 * @param level - The level of severity related to the log message
 */
void printLog(const char* component, const char* msg, const char* level) {
    time_t now;
    time(&now);
    char* ptime = ctime(&now);
    if (ptime[strlen(ptime) - 1] == '\n') {
        ptime[strlen(ptime) - 1] = '\0';
    }
    printf("%s %s [%s]: %s\n", ptime, level, component, msg);
}

/**
 * Print a log message about an executed instruction and the processor status to
 * the console
 *
 * @param instr - The executed instruction
 * @param processor - The struct serving as the system procesor
 */
void printInstrLog(Instruction instr, Processor processor) {
    time_t now;
    time(&now);
    char* ptime = ctime(&now);
    if (ptime[strlen(ptime) - 1] == '\n') {
        ptime[strlen(ptime) - 1] = '\0';
    }

    char* instr_string;
    switch (instr.addr_mode) {
        case IMPL:
            asprintf(&instr_string, IMPL_FORMAT, instr.name);
            break;
        case ACCUM:
            asprintf(&instr_string, ACCUM_FORMAT, instr.name);
            break;
        case IMM:
            asprintf(&instr_string, IMM_FORMAT, instr.name, instr.imm);
            break;
        case ZP:
            asprintf(&instr_string, ZP_FORMAT, instr.name, instr.addr);
            break;
        case ZPX:
            asprintf(&instr_string, ZPX_FORMAT, instr.name, instr.addr);
            break;
        case ZPY:
            asprintf(&instr_string, ZPY_FORMAT, instr.name, instr.addr);
            break;
        case REL:
            asprintf(&instr_string, REL_FORMAT, instr.name, instr.offset);
            break;
        case ABS:
            asprintf(&instr_string, ABS_FORMAT, instr.name, instr.addr);
            break;
        case ABSX:
            asprintf(&instr_string, ABSX_FORMAT, instr.name, instr.addr);
            break;
        case ABSY:
            asprintf(&instr_string, ABSY_FORMAT, instr.name, instr.addr);
            break;
        case IND:
            asprintf(&instr_string, IND_FORMAT, instr.name, instr.addr);
            break;
        case INDX:
            asprintf(&instr_string, INDX_FORMAT, instr.name, instr.addr);
            break;
        case INDY:
            asprintf(&instr_string, INDY_FORMAT, instr.name, instr.addr);
            break;
    }

    printf("%s DEBUG [CPU]: $%04x: %s", ptime, processor.PC - 1, instr_string);
}

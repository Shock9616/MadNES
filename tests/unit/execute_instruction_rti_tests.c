#include "6502.h"
#include "types.h"
#include "utils.h"

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// ---------- Test Setup/Cleanup ----------

Processor processor;
uint8_t* memory;
uint64_t cycles;

static void init_test() {
    // Set registers to default values
    processor.PC = 0x0600;
    processor.S = 0xFF;
    processor.P = 0x30;
    processor.A = 0x0;
    processor.X = 0x0;
    processor.Y = 0x0;

    // Allocate system memory
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t));
}

static void clean_test() {
    free(memory);
}

static void simulateMainloop(uint8_t** memory, Processor* processor) {
    Instruction instr = parseInstruction(*memory, processor->PC);
    executeInstruction(instr, memory, processor);
    processor->PC += instr.length;

    // If the address mode is Absolute X, Absolute Y, or Indirect Indexed, check
    // if a page was crossed and add an extra cycle
    switch (instr.addr_mode) {
        uint16_t addr;
        case ABSX:
            addr = instr.addr;
            if (((addr + processor->X) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        case ABSY:
            addr = instr.addr;
            if (((addr + processor->Y) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        case INDY:
            addr = concatenateBytes((*memory)[instr.addr + 1], (*memory)[instr.addr]);
            if (((addr + processor->Y) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        default:
            break;
    }

    cycles += instr.cycles;
}

// ---------- Tests ----------

void test_instr_rti() {
    processor.S = 0xFC;
    memory[0x01FF] = 0x06;
    memory[0x01FE] = 0x02;
    memory[0x01FD] = 0x32;
    memory[0x0600] = 0x40;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(processor.A, 0x0);
    CU_ASSERT_EQUAL(processor.X, 0x0);
    CU_ASSERT_EQUAL(processor.Y, 0x0);
}

// ---------- Run Tests ----------

CU_pSuite add_rti_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction RTI Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "RTI", test_instr_rti) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

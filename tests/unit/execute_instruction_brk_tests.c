#include "6502.h"
#include "types.h"

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// ---------- Test Setup/Cleanup ----------

Processor processor;
uint8_t* memory;

void init_brk_test() {
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

void clean_brk_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_brk() {
    processor.P = 0x22;
    memory[0x0600] = 0x00;
    memory[0xFFFE] = 0x20;
    memory[0xFFFF] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(processor.PC, 0x1020);
    CU_ASSERT_EQUAL(processor.S, 0xFC);
    CU_ASSERT_EQUAL(memory[0x01FF], 0x06);
    CU_ASSERT_EQUAL(memory[0x01FE], 0x02);
    CU_ASSERT_EQUAL(memory[0x01FD], 0x32);
}

// ---------- Run Tests ----------

CU_pSuite add_brk_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction BRK Tests", NULL,
                                                           NULL, init_brk_test, clean_brk_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BRK", test_instr_brk) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

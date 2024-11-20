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

void init_nop_test() {
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

void clean_nop_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_nop() {
    memory[0x0600] = 0xEA;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.PC, 0x0601);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(processor.A, 0x0);
    CU_ASSERT_EQUAL(processor.X, 0x0);
    CU_ASSERT_EQUAL(processor.Y, 0x0);
}

// ---------- Run Tests ----------

CU_pSuite add_nop_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction NOP Tests", NULL,
                                                           NULL, init_nop_test, clean_nop_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "NOP", test_instr_nop) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

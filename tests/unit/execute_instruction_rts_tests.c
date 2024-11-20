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

void init_rts_test() {
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

void clean_rts_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_rts() {
    processor.S = 0xFD;
    memory[0x01FF] = 0x06;
    memory[0x01FE] = 0x02;
    memory[0x0600] = 0x60;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.PC, 0x0603);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(processor.A, 0x0);
    CU_ASSERT_EQUAL(processor.X, 0x0);
    CU_ASSERT_EQUAL(processor.Y, 0x0);
}

// ---------- Run Tests ----------

CU_pSuite add_rts_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction RTS Tests", NULL,
                                                           NULL, init_rts_test, clean_rts_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "RTS", test_instr_rts) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

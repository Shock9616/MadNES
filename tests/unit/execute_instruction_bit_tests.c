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

void init_bit_test() {
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

void clean_bit_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_bit_zp() {
    processor.A = 0x22;
    memory[0x0010] = 0x22;
    memory[0x0600] = 0x24;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x22);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bit_abs() {
    processor.A = 0x22;
    memory[0x1020] = 0x22;
    memory[0x0600] = 0x2C;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x22);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bit_zero() {
    processor.A = 0x42;
    memory[0x0010] = 0x21;
    memory[0x0600] = 0x24;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_bit_v() {
    processor.A = 0x42;
    memory[0x0010] = 0x42;
    memory[0x0600] = 0x24;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x70);
}

void test_instr_bit_neg() {
    processor.A = 0x96;
    memory[0x0010] = 0x96;
    memory[0x0600] = 0x24;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_bit_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction BIT Tests", NULL,
                                                           NULL, init_bit_test, clean_bit_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BIT Zero Page", test_instr_bit_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BIT Absolute", test_instr_bit_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BIT Zero", test_instr_bit_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BIT Overflow", test_instr_bit_v) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BIT Negative", test_instr_bit_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

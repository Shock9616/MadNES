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

void init_asl_test() {
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

void clean_asl_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_asl_accum() {
    processor.A = 0x3F;
    memory[0x0600] = 0x0A;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x7E);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_asl_zp() {
    memory[0x0010] = 0x3F;
    memory[0x0600] = 0x06;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0010], 0x7E);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_asl_zpx() {
    processor.X = 0x08;
    memory[0x0010] = 0x3F;
    memory[0x0600] = 0x16;
    memory[0x0601] = 0x08;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0010], 0x7E);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_asl_abs() {
    memory[0x1020] = 0x3F;
    memory[0x0600] = 0x0E;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1020], 0x7E);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_asl_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x3F;
    memory[0x0600] = 0x1E;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1025], 0x7E);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_asl_carry() {
    processor.A = 0xBE;
    memory[0x0600] = 0x0A;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x7C);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_asl_zero() {
    processor.A = 0x80;
    memory[0x0600] = 0x0A;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

void test_instr_asl_neg() {
    processor.A = 0x7F;
    memory[0x0600] = 0x0A;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0xFE);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_asl_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction ASL Tests", NULL,
                                                           NULL, init_asl_test, clean_asl_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Accumulator", test_instr_asl_accum) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Zero Page", test_instr_asl_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Zero Page,X", test_instr_asl_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Absolute", test_instr_asl_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Absolute,X", test_instr_asl_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Carry", test_instr_asl_carry) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Zero", test_instr_asl_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ASL Negative", test_instr_asl_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

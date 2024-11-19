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

void init_cpxy_test() {
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

void clean_cpxy_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_cpx_imm() {
    processor.X = 0x69;
    memory[0x0600] = 0xE0;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpx_zp() {
    processor.X = 0x69;
    memory[0x0010] = 0x42;
    memory[0x0600] = 0xE4;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpx_abs() {
    processor.X = 0x69;
    memory[0x1020] = 0x42;
    memory[0x0600] = 0xEC;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpx_eq() {
    processor.X = 0x69;
    memory[0x0600] = 0xE0;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

void test_instr_cpx_gt() {
    processor.X = 0x42;
    memory[0x0600] = 0xE0;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_cpx_lt() {
    processor.X = 0x69;
    memory[0x0600] = 0xE0;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpy_imm() {
    processor.Y = 0x69;
    memory[0x0600] = 0xC0;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpy_zp() {
    processor.Y = 0x69;
    memory[0x0010] = 0x42;
    memory[0x0600] = 0xC4;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpy_abs() {
    processor.Y = 0x69;
    memory[0x1020] = 0x42;
    memory[0x0600] = 0xCC;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cpy_eq() {
    processor.Y = 0x69;
    memory[0x0600] = 0xC0;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

void test_instr_cpy_gt() {
    processor.Y = 0x42;
    memory[0x0600] = 0xC0;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_cpy_lt() {
    processor.Y = 0x69;
    memory[0x0600] = 0xC0;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

// ---------- Run Tests ----------

CU_pSuite add_cpxy_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction CPX/CPY Tests", NULL,
                                                           NULL, init_cpxy_test, clean_cpxy_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Immediate", test_instr_cpx_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Zero Page", test_instr_cpx_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Absolute", test_instr_cpx_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Equal", test_instr_cpx_eq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Greater Than", test_instr_cpx_gt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPX Less Than", test_instr_cpx_lt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Immediate", test_instr_cpy_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Zero Page", test_instr_cpy_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Absolute", test_instr_cpy_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Equal", test_instr_cpy_eq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Greater Than", test_instr_cpy_gt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CPY Less Than", test_instr_cpy_lt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

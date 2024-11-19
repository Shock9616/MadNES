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

void init_eor_test() {
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

void clean_eor_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_eor_imm() {
    processor.A = 0x3C;
    memory[0x0600] = 0x49;
    memory[0x0601] = 0x55;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_zp() {
    processor.A = 0x3C;
    memory[0x0010] = 0x55;
    memory[0x0600] = 0x45;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_zpx() {
    processor.A = 0x3C;
    processor.X = 0x05;
    memory[0x0015] = 0x55;
    memory[0x0600] = 0x55;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_abs() {
    processor.A = 0x3C;
    memory[0x1020] = 0x55;
    memory[0x0600] = 0x4D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_absx() {
    processor.A = 0x3C;
    processor.X = 0x05;
    memory[0x1025] = 0x55;
    memory[0x0600] = 0x5D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_absy() {
    processor.A = 0x3C;
    processor.Y = 0x05;
    memory[0x1025] = 0x55;
    memory[0x0600] = 0x59;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_indx() {
    processor.A = 0x3C;
    processor.X = 0x05;
    memory[0x0015] = 0x20;
    memory[0x0016] = 0x10;
    memory[0x1020] = 0x55;
    memory[0x0600] = 0x41;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_indy() {
    processor.A = 0x3C;
    processor.Y = 0x05;
    memory[0x0010] = 0x20;
    memory[0x0011] = 0x10;
    memory[0x1025] = 0x55;
    memory[0x0600] = 0x51;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_eor_zero() {
    processor.A = 0x69;
    memory[0x0600] = 0x49;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_eor_neg() {
    processor.A = 0xAA;
    memory[0x0600] = 0x49;
    memory[0x0601] = 0x55;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_eor_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction EOR Tests", NULL,
                                                           NULL, init_eor_test, clean_eor_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Immediate", test_instr_eor_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Zero Page", test_instr_eor_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Zero Page,X", test_instr_eor_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Absolute", test_instr_eor_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Absolute,X", test_instr_eor_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Absolute,Y", test_instr_eor_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR (Indirect,X)", test_instr_eor_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR (Indirect),Y", test_instr_eor_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Zero", test_instr_eor_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "EOR Negative", test_instr_eor_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

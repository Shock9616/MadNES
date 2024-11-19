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

void init_lda_test() {
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

void clean_lda_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_lda_imm() {
    memory[0x0600] = 0xA9;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_zp() {
    memory[0x0010] = 0x69;
    memory[0x0600] = 0xA5;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0x69;
    memory[0x0600] = 0xB5;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_abs() {
    memory[0x1020] = 0x69;
    memory[0x0600] = 0xAD;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x69;
    memory[0x0600] = 0xBD;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_absy() {
    processor.Y = 0x05;
    memory[0x1025] = 0x69;
    memory[0x0600] = 0xB9;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_indx() {
    processor.X = 0x05;
    memory[0x0015] = 0x20;
    memory[0x0016] = 0x10;
    memory[0x1020] = 0x69;
    memory[0x0600] = 0xA1;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_indy() {
    processor.Y = 0x05;
    memory[0x0010] = 0x20;
    memory[0x0011] = 0x10;
    memory[0x1025] = 0x69;
    memory[0x0600] = 0xB1;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lda_zero() {
    memory[0x0600] = 0xA9;
    memory[0x0601] = 0x00;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_lda_neg() {
    memory[0x0600] = 0xA9;
    memory[0x0601] = 0x96;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_lda_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction LDA Tests", NULL,
                                                           NULL, init_lda_test, clean_lda_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Immediate", test_instr_lda_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Zero Page", test_instr_lda_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Zero Page,X", test_instr_lda_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Absolute", test_instr_lda_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Absolute,X", test_instr_lda_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Absolute,Y", test_instr_lda_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA (Indirect,X)", test_instr_lda_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA (Indirect),Y", test_instr_lda_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Zero", test_instr_lda_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDA Negative", test_instr_lda_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

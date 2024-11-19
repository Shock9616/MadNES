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

void init_and_test() {
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

void clean_and_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_and_imm() {
    processor.A = 0xFF;
    memory[0x0600] = 0x29;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_zp() {
    processor.A = 0xFF;
    memory[0x0010] = 0x42;
    memory[0x0600] = 0x25;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_zpx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x0015] = 0x42;
    memory[0x0600] = 0x35;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_abs() {
    processor.A = 0xFF;
    memory[0x1020] = 0x42;
    memory[0x0600] = 0x2D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_absx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x3D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_absy() {
    processor.A = 0xFF;
    processor.Y = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x39;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_indx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x0025] = 0x25;
    memory[0x0026] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x21;
    memory[0x0601] = 0x20;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_indy() {
    processor.A = 0xFF;
    processor.Y = 0x05;
    memory[0x0020] = 0x20;
    memory[0x0021] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x31;
    memory[0x0601] = 0x20;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_and_zero() {
    processor.A = 0x00;
    memory[0x0600] = 0x29;
    memory[0x0601] = 0xFF;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_and_neg() {
    processor.A = 0xFF;
    memory[0x0600] = 0x29;
    memory[0x0601] = 0x80;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x80);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_and_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction AND Tests", NULL,
                                                           NULL, init_and_test, clean_and_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Immediate", test_instr_and_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Zero Page", test_instr_and_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Zero Page,X", test_instr_and_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Absolute", test_instr_and_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Absolute,X", test_instr_and_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Absolute,Y", test_instr_and_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND (Indirect,X)", test_instr_and_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND (Indirect),Y", test_instr_and_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Zero", test_instr_and_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "AND Negative", test_instr_and_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

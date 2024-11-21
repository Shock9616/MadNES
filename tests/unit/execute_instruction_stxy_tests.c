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

void init_stxy_test() {
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

void clean_stxy_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_stx_zp() {
    processor.X = 0x69;
    memory[0x0600] = 0x86;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x0010], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_stx_zpy() {
    processor.X = 0x69;
    processor.Y = 0x05;
    memory[0x0600] = 0x96;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x0015], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_stx_abs() {
    processor.X = 0x69;
    memory[0x0600] = 0x8E;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_sty_zp() {
    processor.Y = 0x69;
    memory[0x0600] = 0x84;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x0010], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_sty_zpx() {
    processor.Y = 0x69;
    processor.X = 0x05;
    memory[0x0600] = 0x94;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x0015], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_sty_abs() {
    processor.Y = 0x69;
    memory[0x0600] = 0x8C;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

// ---------- Run Tests ----------

CU_pSuite add_stxy_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction STX/STY Tests", NULL,
                                                           NULL, init_stxy_test, clean_stxy_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STX Zero Page", test_instr_stx_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STX Zero Page,Y", test_instr_stx_zpy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STX Absolute", test_instr_stx_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STY Zero Page", test_instr_sty_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STY Zero Page,X", test_instr_sty_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STY Absolute", test_instr_sty_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}
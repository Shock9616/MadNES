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

void init_sbc_test() {
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

void clean_sbc_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_sbc_imm() {
    processor.A = 0xFF;
    memory[0x0600] = 0xE9;
    memory[0x0601] = 0x95;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_zp() {
    processor.A = 0xFF;
    memory[0x0010] = 0x95;
    memory[0x0600] = 0xE5;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_zpx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x0015] = 0x95;
    memory[0x0600] = 0xF5;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_abs() {
    processor.A = 0xFF;
    memory[0x1020] = 0x95;
    memory[0x0600] = 0xED;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_absx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x1025] = 0x95;
    memory[0x0600] = 0xFD;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_absy() {
    processor.A = 0xFF;
    processor.Y = 0x05;
    memory[0x1025] = 0x95;
    memory[0x0600] = 0xF9;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_indx() {
    processor.A = 0xFF;
    processor.X = 0x05;
    memory[0x0015] = 0x20;
    memory[0x0016] = 0x10;
    memory[0x1020] = 0x95;
    memory[0x0600] = 0xE1;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_indy() {
    processor.A = 0xFF;
    processor.Y = 0x05;
    memory[0x0010] = 0x20;
    memory[0x0011] = 0x10;
    memory[0x1025] = 0x95;
    memory[0x0600] = 0xF1;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sbc_carry() {
    processor.A = 0x00;
    memory[0x0600] = 0xE9;
    memory[0x0601] = 0x96;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_sbc_zero() {
    processor.A = 0x69;
    processor.P = 0x31;
    memory[0x0600] = 0xE9;
    memory[0x0601] = 0x69;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

void test_instr_sbc_overflow() {
    processor.A = 0x88;
    memory[0x0600] = 0xE9;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x77);
    CU_ASSERT_EQUAL(processor.P, 0x71);
}

void test_instr_sbc_neg() {
    processor.A = 0xFF;
    memory[0x0600] = 0xE9;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0xEE);
    CU_ASSERT_EQUAL(processor.P, 0xB1);
}

// ---------- Run Tests ----------

CU_pSuite add_sbc_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction SBC Tests", NULL,
                                                           NULL, init_sbc_test, clean_sbc_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Immediate", test_instr_sbc_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Zero Page", test_instr_sbc_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Zero Page,X", test_instr_sbc_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Absolute", test_instr_sbc_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Absolute,X", test_instr_sbc_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Absolute,Y", test_instr_sbc_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC (Indirect,X)", test_instr_sbc_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC (Indirect),Y", test_instr_sbc_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Carry", test_instr_sbc_carry) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Zero", test_instr_sbc_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Overflow", test_instr_sbc_overflow) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SBC Negative", test_instr_sbc_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

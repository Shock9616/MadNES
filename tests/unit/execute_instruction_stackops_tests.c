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

void init_stackops_test() {
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

void clean_stackops_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_pha() {
    processor.A = 0x69;
    memory[0x0600] = 0x48;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(memory[0x01FF], 0x69);
}

void test_instr_php() {
    memory[0x0600] = 0x08;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(memory[0x01FF], 0x30);
}

void test_instr_pla() {
    processor.S = 0xFE;
    memory[0x01FF] = 0x69;
    memory[0x0600] = 0x68;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
}

void test_instr_pla_zero() {
    processor.A = 0x69;
    processor.S = 0xFE;
    memory[0x01FF] = 0x00;
    memory[0x0600] = 0x68;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_pla_neg() {
    processor.A = 0x69;
    processor.S = 0xFE;
    memory[0x01FF] = 0x96;
    memory[0x0600] = 0x68;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x96);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_plp() {
    processor.S = 0xFE;
    memory[0x01FF] = 0xB1;
    memory[0x0600] = 0x28;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.P, 0xB1);
    CU_ASSERT_EQUAL(processor.S, 0xFF);
}

// ---------- Run Tests ----------

CU_pSuite add_stackops_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(
        "executeInstruction stackops Tests", NULL, NULL, init_stackops_test, clean_stackops_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PHA", test_instr_pha) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PHP", test_instr_php) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PLA", test_instr_pla) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PLA Zero", test_instr_pla_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PLA Negative", test_instr_pla_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "PLP", test_instr_plp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

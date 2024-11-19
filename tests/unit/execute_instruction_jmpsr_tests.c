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

void init_jmpsr_test() {
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

void clean_jmpsr_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_jmp_abs() {
    memory[0x0600] = 0x4C;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.PC, 0x1020);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_jmp_ind() {
    memory[0x1020] = 0x40;
    memory[0x1021] = 0x30;
    memory[0x0600] = 0x6C;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.PC, 0x3040);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_jsr() {
    memory[0x0600] = 0x20;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.PC, 0x1020);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x01FF], 0x06)
    CU_ASSERT_EQUAL(memory[0x01FE], 0x02)
}

// ---------- Run Tests ----------

CU_pSuite add_jmpsr_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction JMP/JSR Tests", NULL,
                                                           NULL, init_jmpsr_test, clean_jmpsr_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "JMP Absolute", test_instr_jmp_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "JMP Indirect", test_instr_jmp_ind) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "JSR", test_instr_jsr) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

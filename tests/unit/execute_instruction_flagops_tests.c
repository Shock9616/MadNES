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

void init_flagops_test() {
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

void clean_flagops_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_clc_set() {
    processor.P = 0x31;
    memory[0x0600] = 0x18;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_clc_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x18;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_cld_set() {
    processor.P = 0x38;
    memory[0x0600] = 0xD8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_cld_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xD8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_cli_set() {
    processor.P = 0x34;
    memory[0x0600] = 0x58;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_cli_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x58;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_clv_set() {
    processor.P = 0x70;
    memory[0x0600] = 0xB8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_clv_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xB8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_sec_set() {
    processor.P = 0x31;
    memory[0x0600] = 0x38;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sec_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x38;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_sed_set() {
    processor.P = 0x38;
    memory[0x0600] = 0xF8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x38);
}

void test_instr_sed_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xF8;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x38);
}

void test_instr_sei_set() {
    processor.P = 0x34;
    memory[0x0600] = 0x78;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x34);
}

void test_instr_sei_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x78;

    Instruction instr = parseInstruction(memory, 0x0600);
    executeInstruction(instr, &memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x34);
}

// ---------- Run Tests ----------

CU_pSuite add_flagops_suite_to_registry() {
    CU_pSuite suite =
        CU_add_suite_with_setup_and_teardown("executeInstruction Flag Operation Tests", NULL, NULL,
                                             init_flagops_test, clean_flagops_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLC (Carry set)", test_instr_clc_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLC (Carry clear)", test_instr_clc_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLD (Decimal Mode set)", test_instr_cld_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLD (Decimal Mode clear)", test_instr_cld_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLI (Interrupt Disable set)", test_instr_cli_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLI (Interrupt Disable clear)", test_instr_cli_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLV (Overflow set)", test_instr_clv_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CLV (Overflow clear)", test_instr_clv_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SEC (Carry set)", test_instr_sec_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SEC (Carry clear)", test_instr_sec_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SED (Decmial Mode set)", test_instr_sed_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SED (Decimal Mode clear)", test_instr_sed_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SEI (Interrupt Disable set)", test_instr_sei_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "SEI (Interrupt Disable clear)", test_instr_sei_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

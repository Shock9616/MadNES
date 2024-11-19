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

void init_adc_test() {
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

void clean_adc_test() {
    free(memory);
}

// ---------- Tests ----------

void test_instr_adc_imm() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x42;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_zp() {
    memory[0x0010] = 0x42;
    memory[0x0600] = 0x65;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0x42;
    memory[0x0600] = 0x75;
    memory[0x0601] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}
void test_instr_adc_abs() {
    memory[0x1020] = 0x42;
    memory[0x0600] = 0x6D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x7D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_absy() {
    processor.Y = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x79;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_indx() {
    processor.X = 0x05;
    memory[0x0025] = 0x25;
    memory[0x0026] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x61;
    memory[0x0601] = 0x20;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_indy() {
    processor.Y = 0x05;
    memory[0x0020] = 0x20;
    memory[0x0021] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x71;
    memory[0x0601] = 0x20;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_adc_carry() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0xFF;
    memory[0x0602] = 0x69;
    memory[0x0603] = 0x02;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;
    instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x01);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_adc_zero() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x00;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_adc_overflow() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x50;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0xA0);
    CU_ASSERT_EQUAL(processor.P, 0xF0);
}

void test_instr_adc_neg() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x80;

    Instruction instr = parseInstruction(memory, processor.PC);
    executeInstruction(instr, &memory, &processor);
    processor.PC += instr.length;

    CU_ASSERT_EQUAL(processor.A, 0x80);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_adc_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction ADC Tests", NULL,
                                                           NULL, init_adc_test, clean_adc_test);
    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Immediate", test_instr_adc_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Zero Page", test_instr_adc_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Zero Page,X", test_instr_adc_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Absolute", test_instr_adc_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Absolute,X", test_instr_adc_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Absolute,Y", test_instr_adc_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC (Indirect,X)", test_instr_adc_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC (Indirect),Y", test_instr_adc_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Carry", test_instr_adc_carry) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Zero", test_instr_adc_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Overflow", test_instr_adc_overflow) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Negative", test_instr_adc_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

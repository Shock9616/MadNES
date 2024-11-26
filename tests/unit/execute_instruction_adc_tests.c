#include "6502.h"
#include "types.h"
#include "utils.h"

#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestDB.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// ---------- Test Setup/Cleanup ----------

Processor processor;
uint8_t* memory;
uint64_t cycles;

static void init_test() {
    // Set registers to default values
    processor.PC = 0x0600;
    processor.S = 0xFF;
    processor.P = 0x30;
    processor.A = 0x0;
    processor.X = 0x0;
    processor.Y = 0x0;

    // Allocate system memory
    memory = calloc(MEMORY_SPACE, sizeof(uint8_t));
    cycles = 0;
}

static void clean_test() {
    free(memory);
}

static void simulateMainloop(uint8_t** memory, Processor* processor) {
    Instruction instr = parseInstruction(*memory, processor->PC);
    executeInstruction(instr, memory, processor);
    processor->PC += instr.length;

    // If the address mode is Absolute X, Absolute Y, or Indirect Indexed, check
    // if a page was crossed and add an extra cycle
    switch (instr.addr_mode) {
        uint16_t addr;
        case ABSX:
            addr = instr.addr;
            if (((addr + processor->X) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        case ABSY:
            addr = instr.addr;
            if (((addr + processor->Y) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        case INDY:
            addr = concatenateBytes((*memory)[instr.addr + 1], (*memory)[instr.addr]);
            if (((addr + processor->Y) & 0xFF00) != (addr & 0xFF00)) {
                instr.cycles++;
            }
            break;
        default:
            break;
    }

    cycles += instr.cycles;
}

// ---------- Tests ----------

void test_instr_adc_imm() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_adc_zp() {
    memory[0x0010] = 0x42;
    memory[0x0600] = 0x65;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 3);
}

void test_instr_adc_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0x42;
    memory[0x0600] = 0x75;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}
void test_instr_adc_abs() {
    memory[0x1020] = 0x42;
    memory[0x0600] = 0x6D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}

void test_instr_adc_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x7D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}

void test_instr_adc_absy() {
    processor.Y = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x79;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}

void test_instr_adc_indx() {
    processor.X = 0x05;
    memory[0x0025] = 0x25;
    memory[0x0026] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x61;
    memory[0x0601] = 0x20;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 6);
}

void test_instr_adc_indy() {
    processor.Y = 0x05;
    memory[0x0020] = 0x20;
    memory[0x0021] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0x71;
    memory[0x0601] = 0x20;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_adc_carry() {
    processor.A = 0xFF;
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x02;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x01);
    CU_ASSERT_EQUAL(processor.P, 0x31);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_adc_zero() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x00;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_adc_overflow() {
    processor.A = 0x50;
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x50;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0xA0);
    CU_ASSERT_EQUAL(processor.P, 0xF0);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_adc_neg() {
    memory[0x0600] = 0x69;
    memory[0x0601] = 0x80;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x80);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_adc_absx_page() {
    processor.X = 0xFF;
    memory[0x111F] = 0x69;
    memory[0x0600] = 0x7D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_adc_absy_page() {
    processor.Y = 0xFF;
    memory[0x111F] = 0x69;
    memory[0x0600] = 0x79;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_adc_indy_page() {
    processor.Y = 0xFF;
    memory[0x0020] = 0x20;
    memory[0x0021] = 0x10;
    memory[0x111F] = 0x42;
    memory[0x0600] = 0x71;
    memory[0x0601] = 0x20;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 6);
}

// ---------- Run Tests ----------

CU_pSuite add_adc_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction ADC Tests", NULL,
                                                           NULL, init_test, clean_test);
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

    if (CU_add_test(suite, "ADC Absolute,X Page Crossed", test_instr_adc_absx_page) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Absolute,Y Page Crossed", test_instr_adc_absy_page) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "ADC Indirect,Y Page Crossed", test_instr_adc_indy_page) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

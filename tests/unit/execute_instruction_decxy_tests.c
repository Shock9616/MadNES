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
    uint16_t old_PC = processor->PC;

    Instruction instr = parseInstruction(*memory, processor->PC);
    executeInstruction(instr, memory, processor);
    processor->PC += instr.length;

    // If the address mode is Absolute X, Absolute Y, Indirect Indexed, or
    // Relative, check if a page was crossed and add an extra cycle
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
        case REL:
            if (processor->PC != old_PC + instr.length) {
                // Branch succeeded
                instr.cycles++;

                if ((processor->PC & 0xFF00) != (old_PC & 0xFF00)) {
                    // Page crossed
                    instr.cycles++;
                }
            }
        default:
            break;
    }

    cycles += instr.cycles;
}

// ---------- Tests ----------

void test_instr_dec_zp() {
    memory[0x0010] = 0x6A;
    memory[0x0600] = 0xC6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x0010], 0x69);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_dec_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0x6A;
    memory[0x0600] = 0xD6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x0015], 0x69);
    CU_ASSERT_EQUAL(cycles, 6);
}

void test_instr_dec_abs() {
    memory[0x1020] = 0x6A;
    memory[0x0600] = 0xCE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
    CU_ASSERT_EQUAL(cycles, 6);
}

void test_instr_dec_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x6A;
    memory[0x0600] = 0xDE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x1025], 0x69);
    CU_ASSERT_EQUAL(cycles, 7);
}

void test_instr_dec_zero() {
    memory[0x0010] = 0x01;
    memory[0x0600] = 0xC6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(memory[0x0010], 0x00);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_dec_neg() {
    memory[0x0010] = 0x00;
    memory[0x0600] = 0xC6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0xB0);
    CU_ASSERT_EQUAL(memory[0x0010], 0xFF);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_dex() {
    processor.X = 0x6A;
    memory[0x0600] = 0xCA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_dex_zero() {
    processor.X = 0x01;
    memory[0x0600] = 0xCA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_dex_neg() {
    processor.X = 0x00;
    memory[0x0600] = 0xCA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_dey() {
    processor.Y = 0x6A;
    memory[0x0600] = 0x88;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_dey_zero() {
    processor.Y = 0x01;
    memory[0x0600] = 0x88;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_dey_neg() {
    processor.X = 0x00;
    memory[0x0600] = 0x88;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0xFF);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
    CU_ASSERT_EQUAL(cycles, 2);
}

// ---------- Run Tests ----------

CU_pSuite add_decxy_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction DEC/DEX/DEY Tests",
                                                           NULL, NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Zero Page", test_instr_dec_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Zero Page,X", test_instr_dec_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Absolute", test_instr_dec_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Absolute,X", test_instr_dec_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Zero", test_instr_dec_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEC Negative", test_instr_dec_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEX", test_instr_dex) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEX Zero", test_instr_dex_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEX Negative", test_instr_dex_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEY", test_instr_dey) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEY Zero", test_instr_dey_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "DEY Negative", test_instr_dey_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

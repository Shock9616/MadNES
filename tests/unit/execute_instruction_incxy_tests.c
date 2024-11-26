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

void test_instr_inc_zp() {
    memory[0x0010] = 0x68;
    memory[0x0600] = 0xE6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x0010], 0x69);
}

void test_instr_inc_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0x68;
    memory[0x0600] = 0xF6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x0015], 0x69);
}

void test_instr_inc_abs() {
    memory[0x1020] = 0x68;
    memory[0x0600] = 0xEE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
}

void test_instr_inc_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0x68;
    memory[0x0600] = 0xFE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(memory[0x1025], 0x69);
}

void test_instr_inc_zero() {
    memory[0x0010] = 0xFF;
    memory[0x0600] = 0xE6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x32);
    CU_ASSERT_EQUAL(memory[0x0010], 0x00);
}

void test_instr_inc_neg() {
    memory[0x0010] = 0x7F;
    memory[0x0600] = 0xE6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0xB0);
    CU_ASSERT_EQUAL(memory[0x0010], 0x80);
}

void test_instr_inx() {
    processor.X = 0x68;
    memory[0x0600] = 0xE8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_inx_zero() {
    processor.X = 0xFF;
    memory[0x0600] = 0xE8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_inx_neg() {
    processor.X = 0x7F;
    memory[0x0600] = 0xE8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x80);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}
void test_instr_iny() {
    processor.Y = 0x68;
    memory[0x0600] = 0xC8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_iny_zero() {
    processor.Y = 0xFF;
    memory[0x0600] = 0xC8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_iny_neg() {
    processor.Y = 0x7F;
    memory[0x0600] = 0xC8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x80);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_incxy_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction INC/INX/INY Tests",
                                                           NULL, NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Zero Page", test_instr_inc_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Zero Page,X", test_instr_inc_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Absolute", test_instr_inc_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Absolute,X", test_instr_inc_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Zero", test_instr_inc_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INC Negative", test_instr_inc_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INX", test_instr_inx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INX Zero", test_instr_inx_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INX Negative", test_instr_inx_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INY", test_instr_iny) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INY Zero", test_instr_iny_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "INY Negative", test_instr_iny_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

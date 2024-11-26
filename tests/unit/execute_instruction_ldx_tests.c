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

void test_instr_ldx_imm() {
    memory[0x0600] = 0xA2;
    memory[0x0601] = 0x69;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_ldx_zp() {
    memory[0x0010] = 0x69;
    memory[0x0600] = 0xA6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_ldx_zpy() {
    processor.Y = 0x05;
    memory[0x0015] = 0x69;
    memory[0x0600] = 0xB6;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_ldx_abs() {
    memory[0x1020] = 0x69;
    memory[0x0600] = 0xAE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_ldx_absy() {
    processor.Y = 0x05;
    memory[0x1025] = 0x69;
    memory[0x0600] = 0xBE;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_ldx_zero() {
    memory[0x0600] = 0xA2;
    memory[0x0601] = 0x00;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_ldx_neg() {
    memory[0x0600] = 0xA2;
    memory[0x0601] = 0x96;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_ldx_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction LDX Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Immediate", test_instr_ldx_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Zero Page", test_instr_ldx_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Zero Page,Y", test_instr_ldx_zpy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Absolute", test_instr_ldx_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Absolute,Y", test_instr_ldx_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Zero", test_instr_ldx_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LDX Negative", test_instr_ldx_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

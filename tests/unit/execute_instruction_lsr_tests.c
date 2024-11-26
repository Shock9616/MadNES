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

void test_instr_lsr_accum() {
    processor.A = 0xCC;
    memory[0x0600] = 0x4A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lsr_zp() {
    memory[0x0010] = 0xCC;
    memory[0x0600] = 0x46;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0010], 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lsr_zpx() {
    processor.X = 0x05;
    memory[0x0015] = 0xCC;
    memory[0x0600] = 0x56;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0015], 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lsr_abs() {
    memory[0x1020] = 0xCC;
    memory[0x0600] = 0x4E;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1020], 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lsr_absx() {
    processor.X = 0x05;
    memory[0x1025] = 0xCC;
    memory[0x0600] = 0x5E;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1025], 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_lsr_carry() {
    processor.A = 0xCD;
    memory[0x0600] = 0x4A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x66);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_lsr_zero() {
    processor.A = 0x01;
    memory[0x0600] = 0x4A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

// ---------- Run Tests ----------

CU_pSuite add_lsr_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction LSR Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Accumulator", test_instr_lsr_accum) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Zero Page", test_instr_lsr_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Zero Page,X", test_instr_lsr_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Absolute", test_instr_lsr_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Absolute,X", test_instr_lsr_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Carry", test_instr_lsr_carry) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "LSR Zero", test_instr_lsr_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

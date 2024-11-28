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

void test_instr_sta_zp() {
    processor.A = 0x69;
    memory[0x0600] = 0x85;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0010], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 3);
}

void test_instr_sta_zpx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x0600] = 0x95;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x0015], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}

void test_instr_sta_abs() {
    processor.A = 0x69;
    memory[0x0600] = 0x8D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 4);
}

void test_instr_sta_absx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x0600] = 0x9D;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1025], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_sta_absy() {
    processor.A = 0x69;
    processor.Y = 0x05;
    memory[0x0600] = 0x99;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1025], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 5);
}

void test_instr_sta_indx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x0015] = 0x20;
    memory[0x0016] = 0x10;
    memory[0x0600] = 0x81;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1020], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 6);
}

void test_instr_sta_indy() {
    processor.A = 0x69;
    processor.Y = 0x05;
    memory[0x0010] = 0x20;
    memory[0x0011] = 0x10;
    memory[0x0600] = 0x81;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(memory[0x1025], 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 6);
}

// ---------- Run Tests ----------

CU_pSuite add_sta_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction STA Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA Zero Page", test_instr_sta_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA Zero Page,X", test_instr_sta_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA Absolute", test_instr_sta_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA Absolute,X", test_instr_sta_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA Absolute,Y", test_instr_sta_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA (Indirect,X)", test_instr_sta_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "STA (Indirect),Y", test_instr_sta_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

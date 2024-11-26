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

void test_instr_cmp_imm() {
    processor.A = 0x69;
    memory[0x0600] = 0xC9;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_zp() {
    processor.A = 0x69;
    memory[0x0010] = 0x42;
    memory[0x0600] = 0xC5;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_zpx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x0015] = 0x42;
    memory[0x0600] = 0xD5;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_abs() {
    processor.A = 0x69;
    memory[0x1020] = 0x42;
    memory[0x0600] = 0xCD;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_absx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0xDD;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_absy() {
    processor.A = 0x69;
    processor.Y = 0x05;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0xD9;
    memory[0x0601] = 0x20;
    memory[0x0602] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_indx() {
    processor.A = 0x69;
    processor.X = 0x05;
    memory[0x0015] = 0x20;
    memory[0x0016] = 0x10;
    memory[0x1020] = 0x42;
    memory[0x0600] = 0xC1;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_indy() {
    processor.A = 0x69;
    processor.Y = 0x05;
    memory[0x0010] = 0x20;
    memory[0x0011] = 0x10;
    memory[0x1025] = 0x42;
    memory[0x0600] = 0xD1;
    memory[0x0601] = 0x10;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_cmp_eq() {
    processor.A = 0x69;
    memory[0x0600] = 0xC9;
    memory[0x0601] = 0x69;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x33);
}

void test_instr_cmp_gt() {
    processor.A = 0x42;
    memory[0x0600] = 0xC9;
    memory[0x0601] = 0x69;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x42);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_cmp_lt() {
    processor.A = 0x69;
    memory[0x0600] = 0xC9;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

// ---------- Run Tests ----------

CU_pSuite add_cmp_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction CMP Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Immediate", test_instr_cmp_imm) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Zero Page", test_instr_cmp_zp) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Zero Page,X", test_instr_cmp_zpx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Absolute", test_instr_cmp_abs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Absolute,X", test_instr_cmp_absx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Absolute,Y", test_instr_cmp_absy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP (Indirect,X)", test_instr_cmp_indx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP (Indirect),Y", test_instr_cmp_indy) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Equal", test_instr_cmp_eq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Greater Than", test_instr_cmp_gt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "CMP Less Than", test_instr_cmp_lt) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

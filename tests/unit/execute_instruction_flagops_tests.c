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

void test_instr_clc_set() {
    processor.P = 0x31;
    memory[0x0600] = 0x18;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_clc_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x18;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_cld_set() {
    processor.P = 0x38;
    memory[0x0600] = 0xD8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_cld_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xD8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_cli_set() {
    processor.P = 0x34;
    memory[0x0600] = 0x58;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_cli_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x58;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_clv_set() {
    processor.P = 0x70;
    memory[0x0600] = 0xB8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_clv_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xB8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x30);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sec_set() {
    processor.P = 0x31;
    memory[0x0600] = 0x38;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x31);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sec_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x38;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x31);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sed_set() {
    processor.P = 0x38;
    memory[0x0600] = 0xF8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x38);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sed_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xF8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x38);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sei_set() {
    processor.P = 0x34;
    memory[0x0600] = 0x78;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x34);
    CU_ASSERT_EQUAL(cycles, 2);
}

void test_instr_sei_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x78;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.P, 0x34);
    CU_ASSERT_EQUAL(cycles, 2);
}

// ---------- Run Tests ----------

CU_pSuite add_flagops_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(
        "executeInstruction Flag Operation Tests", NULL, NULL, init_test, clean_test);

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

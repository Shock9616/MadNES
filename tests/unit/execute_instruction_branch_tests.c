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

void test_instr_bcc_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0x90;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bcc_set() {
    processor.P = 0x31;
    memory[0x0600] = 0x90;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_bcs_clear() {
    processor.P = 0x30;
    memory[0x0600] = 0xB0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bcs_set() {
    processor.P = 0x31;
    memory[0x0600] = 0xB0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x31);
}

void test_instr_beq_eq() {
    processor.P = 0x32;
    memory[0x0600] = 0xF0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_beq_neq() {
    processor.P = 0x30;
    memory[0x0600] = 0xF0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bmi_neg() {
    processor.P = 0xB0;
    memory[0x0600] = 0x30;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_bmi_pos() {
    processor.P = 0x30;
    memory[0x0600] = 0x30;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bne_eq() {
    processor.P = 0x32;
    memory[0x0600] = 0xD0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_bne_neq() {
    processor.P = 0x30;
    memory[0x0600] = 0xD0;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bpl_pos() {
    processor.P = 0x30;
    memory[0x0600] = 0x10;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bpl_neg() {
    processor.P = 0xB0;
    memory[0x0600] = 0x10;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_bvc_vclr() {
    processor.P = 0x30;
    memory[0x0600] = 0x50;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bvc_vset() {
    processor.P = 0x70;
    memory[0x0600] = 0x50;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x70);
}

void test_instr_bvs_vclr() {
    processor.P = 0x30;
    memory[0x0600] = 0x70;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0602);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_bvs_vset() {
    processor.P = 0x70;
    memory[0x0600] = 0x70;
    memory[0x0601] = 0x42;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.PC, 0x0600 + 0x42);
    CU_ASSERT_EQUAL(processor.P, 0x70);
}

// ---------- Run Tests ----------

CU_pSuite add_branch_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction Branch Tests", NULL,
                                                           NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BCC (Carry clear)", test_instr_bcc_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BCC (Carry set)", test_instr_bcc_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BCS (Carry clear)", test_instr_bcc_clear) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BCS (Carry set)", test_instr_bcc_set) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BEQ (Zero set)", test_instr_beq_eq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BEQ (Zero clear)", test_instr_beq_neq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BMI (Negative set)", test_instr_bmi_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BMI (Negative clear)", test_instr_bmi_pos) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BNE (Zero set)", test_instr_bne_eq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BNE (Zero clear)", test_instr_bne_neq) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BPL (Negative clear)", test_instr_bpl_pos) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BPL (Negative set)", test_instr_bpl_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BVC (Overflow clear)", test_instr_bvc_vclr) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BVC (Overflow set)", test_instr_bvc_vset) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BVS (Overflow clear)", test_instr_bvs_vclr) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "BVS (Overflow set)", test_instr_bvs_vset) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

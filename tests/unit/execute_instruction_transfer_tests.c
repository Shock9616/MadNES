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

void test_instr_tax() {
    processor.A = 0x69;
    memory[0x0600] = 0xAA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_tax_zero() {
    processor.A = 0x00;
    memory[0x0600] = 0xAA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_tax_neg() {
    processor.A = 0x96;
    memory[0x0600] = 0xAA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_tay() {
    processor.A = 0x69;
    memory[0x0600] = 0xA8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_tay_zero() {
    processor.A = 0x00;
    memory[0x0600] = 0xA8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_tay_neg() {
    processor.A = 0x96;
    memory[0x0600] = 0xA8;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.Y, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_tsx() {
    processor.S = 0x69;
    memory[0x0600] = 0xBA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_tsx_zero() {
    processor.S = 0x00;
    memory[0x0600] = 0xBA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_tsx_neg() {
    processor.S = 0x96;
    memory[0x0600] = 0xBA;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.X, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_txa() {
    processor.X = 0x69;
    memory[0x0600] = 0x8A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_txa_zero() {
    processor.X = 0x00;
    memory[0x0600] = 0x8A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_txa_neg() {
    processor.X = 0x96;
    memory[0x0600] = 0x8A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

void test_instr_txs() {
    processor.X = 0x69;
    memory[0x0600] = 0x9A;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.S, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_tya() {
    processor.Y = 0x69;
    memory[0x0600] = 0x98;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x69);
    CU_ASSERT_EQUAL(processor.P, 0x30);
}

void test_instr_tya_zero() {
    processor.Y = 0x00;
    memory[0x0600] = 0x98;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x00);
    CU_ASSERT_EQUAL(processor.P, 0x32);
}

void test_instr_tya_neg() {
    processor.Y = 0x96;
    memory[0x0600] = 0x98;

    simulateMainloop(&memory, &processor);

    CU_ASSERT_EQUAL(processor.A, 0x96);
    CU_ASSERT_EQUAL(processor.P, 0xB0);
}

// ---------- Run Tests ----------

CU_pSuite add_transfer_suite_to_registry() {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown("executeInstruction Transfer Tests",
                                                           NULL, NULL, init_test, clean_test);

    if (suite == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAX", test_instr_tax) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAX Zero", test_instr_tax_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAX Negative", test_instr_tax_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAY", test_instr_tay) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAY Zero", test_instr_tay_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TAY Negative", test_instr_tay_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TSX", test_instr_tsx) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TSX Zero", test_instr_tsx_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TSX Negative", test_instr_tsx_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TXA", test_instr_txa) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TXA Zero", test_instr_txa_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TXA Negative", test_instr_txa_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TXS", test_instr_txs) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TYA", test_instr_tya) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TYA Zero", test_instr_tya_zero) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    if (CU_add_test(suite, "TYA Negative", test_instr_tya_neg) == NULL) {
        CU_cleanup_registry();
        return NULL;
    }

    return suite;
}

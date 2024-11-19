#include <CUnit/Basic.h>
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>

extern CU_pSuite add_adc_suite_to_registry();
extern CU_pSuite add_and_suite_to_registry();
extern CU_pSuite add_asl_suite_to_registry();
extern CU_pSuite add_branch_suite_to_registry();
extern CU_pSuite add_bit_suite_to_registry();
extern CU_pSuite add_brk_suite_to_registry();
extern CU_pSuite add_flagops_suite_to_registry();
extern CU_pSuite add_cmp_suite_to_registry();
extern CU_pSuite add_cpxy_suite_to_registry();
extern CU_pSuite add_decxy_suite_to_registry();
extern CU_pSuite add_eor_suite_to_registry();
extern CU_pSuite add_incxy_suite_to_registry();

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    if (add_adc_suite_to_registry() == NULL || add_and_suite_to_registry() == NULL ||
        add_asl_suite_to_registry() == NULL || add_branch_suite_to_registry() == NULL ||
        add_bit_suite_to_registry() == NULL || add_brk_suite_to_registry() == NULL ||
        add_flagops_suite_to_registry() == NULL || add_cmp_suite_to_registry() == NULL ||
        add_cpxy_suite_to_registry() == NULL || add_decxy_suite_to_registry() == NULL ||
        add_eor_suite_to_registry() == NULL || add_incxy_suite_to_registry() == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}

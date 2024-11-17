#include <CUnit/Basic.h>
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>

extern CU_pSuite add_adc_suite_to_registry();
extern CU_pSuite add_and_suite_to_registry();
extern CU_pSuite add_asl_suite_to_registry();

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    if (add_adc_suite_to_registry() == NULL || add_and_suite_to_registry() == NULL ||
        add_asl_suite_to_registry() == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}

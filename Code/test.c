#include <stdio.h>
#include <CUnit/Basic.h>

void test_addition_operator(void) {
    // Test adding two numbers
    CU_ASSERT_EQUAL(2 + 3, 5);
}

int main() {
    CU_pSuite pSuite = NULL;

    // Initialize CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    // Add a suite to the registry
    pSuite = CU_add_suite("Operator_Test_Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test to the suite
    if (NULL == CU_add_test(pSuite, "test_addition_operator", test_addition_operator)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

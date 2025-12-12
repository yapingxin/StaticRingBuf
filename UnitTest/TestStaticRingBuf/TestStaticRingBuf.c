// TestStaticRingBuf.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "CUnit/Automated.h"
#include "CUnit/Console.h"
#include "Common/unistd.h"

#include "Verify/TS_0001.h"

typedef enum
{
    RUN_MODE_DEFAULT = 0,
    RUN_MODE_AUTO = 1,
    RUN_MODE_CONSOLE = 2,
    RUN_MODE_CURSES = 3,

} Run_Mode;

int main(int argc, char* argv[])
{
    Run_Mode run_mode = RUN_MODE_AUTO;
    CU_pSuite suite_0001 = NULL;

    if (argc >= 2)
    {
        if (strncasecmp(argv[1], "-a", 2) == 0)
        {
            run_mode = RUN_MODE_AUTO;
        }
        else if (strncasecmp(argv[1], "-c", 2) == 0)
        {
            run_mode = RUN_MODE_CONSOLE;
        }
    }

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        goto EXIT;
    }

    /* Add a test suite to the registry: StaticRingBuf */
    suite_0001 = CU_add_suite(TS_0001_Identifier, TS_0001_Setup, TS_0001_Cleanup);
    if (suite_0001 == NULL)
    {
        goto EXIT;
    }

    CU_ADD_TEST(suite_0001, TC0001_STARB_Create_Release);
    CU_ADD_TEST(suite_0001, TC0002_STARB_Init_Release);
    CU_ADD_TEST(suite_0001, TC0003_STARB_Write1Byte);
    CU_ADD_TEST(suite_0001, TC0004_STARB_TestCapBound_Write);
    CU_ADD_TEST(suite_0001, TC0005_STARB_WriteItems);
    CU_ADD_TEST(suite_0001, TC0006_STARB_TestCapBound_WriteItems);

    switch (run_mode)
    {
    case RUN_MODE_AUTO:
        /* Run all tests using the automated interface */
        CU_automated_run_tests();
        CU_list_tests_to_file();
        break;
    case RUN_MODE_CONSOLE:
        /* Run all tests using the console interface */
        CU_console_run_tests();
        break;
    default:
        /* Run all tests using the console interface */
        CU_console_run_tests();
        break;
    }

    printf("Please check the result in CUnitAutomated-Results.xml\n");

EXIT:
    CU_cleanup_registry();
    return CU_get_error();
}

#include <stdlib.h>
#include <string.h>
#include "CUnit/CUnit.h"
#include "TS_0001.h"
#include "StaticRingBuf.h"
#include "TS_0001_TestData.h"

/** @par Private (Static) data declaration
 */

static byte STOBUF[2 * RB_LENGTH] = { 0 };

/** @par Private (Static) functions declaration
 */

/** @par Public functions implementation: Test Suite
 */

int TS_0001_Setup(void)
{
    return CUE_SUCCESS;
}

int TS_0001_Cleanup(void)
{
    return CUE_SUCCESS;
}

/** @par Public functions implementation: Test Cases
 */

void TC0001_STARB_Create_Release(void)
{
    StaticRingBuf rbuf = { 0 };

    uint8_t rc = StaticRingBuf_Create(&rbuf, RB_LENGTH);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc == STARB_OK)
    {
        CU_ASSERT_PTR_NOT_NULL(rbuf.buffer);
        CU_ASSERT_EQUAL(rbuf.capacity, RB_LENGTH);
        CU_ASSERT_EQUAL(rbuf.wpos, 0);
        CU_ASSERT_EQUAL(rbuf.rpos, 0);
        CU_ASSERT_EQUAL(rbuf.flag.zeros, 0);
        CU_ASSERT_EQUAL(rbuf.flag.ownbuf, 1);
        CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

        STARB_CAPTYPE wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
        CU_ASSERT_EQUAL(wcap, RB_LENGTH);
        STARB_CAPTYPE rcap = StaticRingBuf_GetReadCapacity(&rbuf);
        CU_ASSERT_EQUAL(rcap, 0);
    }

    StaticRingBuf_Release(&rbuf);
    CU_ASSERT_PTR_NULL(rbuf.buffer);
    CU_ASSERT_EQUAL(rbuf.capacity, 0);
    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.zeros, 0);
    CU_ASSERT_EQUAL(rbuf.flag.ownbuf, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);
}

void TC0002_STARB_Init_Release(void)
{
    StaticRingBuf rbuf = { 0 };

    uint8_t rc = StaticRingBuf_Init(&rbuf, RB_LENGTH, STOBUF);
    CU_ASSERT_EQUAL(rc, STARB_OK);

    if (rc == STARB_OK)
    {
        CU_ASSERT_PTR_NOT_NULL(rbuf.buffer);
        CU_ASSERT_EQUAL(rbuf.capacity, RB_LENGTH);
        CU_ASSERT_EQUAL(rbuf.wpos, 0);
        CU_ASSERT_EQUAL(rbuf.rpos, 0);
        CU_ASSERT_EQUAL(rbuf.flag.zeros, 0);
        CU_ASSERT_EQUAL(rbuf.flag.ownbuf, 0);
        CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

        STARB_CAPTYPE wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
        CU_ASSERT_EQUAL(wcap, RB_LENGTH);
        STARB_CAPTYPE rcap = StaticRingBuf_GetReadCapacity(&rbuf);
        CU_ASSERT_EQUAL(rcap, 0);
    }

    StaticRingBuf_Release(&rbuf);
    CU_ASSERT_PTR_NULL(rbuf.buffer);
    CU_ASSERT_EQUAL(rbuf.capacity, 0);
    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.zeros, 0);
    CU_ASSERT_EQUAL(rbuf.flag.ownbuf, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);
}

/** @par Private (Static) functions implementation
 */

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

static void Verify_STARB_Create(StaticRingBuf* const rbuf, const STARB_CAPTYPE _capacity);
static void Verify_STARB_Init(StaticRingBuf* const rbuf, const STARB_CAPTYPE _capacity);
static void Verify_STARB_Release(StaticRingBuf* const rbuf);

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
        Verify_STARB_Create(&rbuf, RB_LENGTH);
    }

    StaticRingBuf_Release(&rbuf);
    Verify_STARB_Release(&rbuf);
}

void TC0002_STARB_Init_Release(void)
{
    StaticRingBuf rbuf = { 0 };

    uint8_t rc = StaticRingBuf_Init(&rbuf, RB_LENGTH, STOBUF);
    CU_ASSERT_EQUAL(rc, STARB_OK);

    if (rc == STARB_OK)
    {
        Verify_STARB_Init(&rbuf, RB_LENGTH);
    }

    StaticRingBuf_Release(&rbuf);
    Verify_STARB_Release(&rbuf);
}

void TC0003_STARB_Write1Byte(void)
{
    StaticRingBuf rbuf = { 0 };
    byte* src = Get_ByteArray0();

    uint8_t rc = StaticRingBuf_Init(&rbuf, RB_LENGTH, STOBUF);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }
    Verify_STARB_Init(&rbuf, RB_LENGTH);

    rc = StaticRingBuf_Write(&rbuf, src[0]);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 1);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);

EXIT:
    StaticRingBuf_Release(&rbuf);
}

/** @par Private (Static) functions implementation
 */

static void Verify_STARB_Create(StaticRingBuf* const rbuf, const STARB_CAPTYPE _capacity)
{
    CU_ASSERT_PTR_NOT_NULL(rbuf->buffer);
    CU_ASSERT_EQUAL(rbuf->capacity, _capacity);
    CU_ASSERT_EQUAL(rbuf->wpos, 0);
    CU_ASSERT_EQUAL(rbuf->rpos, 0);
    CU_ASSERT_EQUAL(rbuf->flag.zeros, 0);
    CU_ASSERT_EQUAL(rbuf->flag.ownbuf, 1);
    CU_ASSERT_EQUAL(rbuf->flag.cycle, 0);

    STARB_CAPTYPE wcap = StaticRingBuf_GetWriteCapacity(rbuf);
    CU_ASSERT_EQUAL(wcap, _capacity);
    STARB_CAPTYPE rcap = StaticRingBuf_GetReadCapacity(rbuf);
    CU_ASSERT_EQUAL(rcap, 0);
}

static void Verify_STARB_Init(StaticRingBuf* const rbuf, const STARB_CAPTYPE _capacity)
{
    CU_ASSERT_PTR_NOT_NULL(rbuf->buffer);
    CU_ASSERT_EQUAL(rbuf->capacity, _capacity);
    CU_ASSERT_EQUAL(rbuf->wpos, 0);
    CU_ASSERT_EQUAL(rbuf->rpos, 0);
    CU_ASSERT_EQUAL(rbuf->flag.zeros, 0);
    CU_ASSERT_EQUAL(rbuf->flag.ownbuf, 0);
    CU_ASSERT_EQUAL(rbuf->flag.cycle, 0);

    STARB_CAPTYPE wcap = StaticRingBuf_GetWriteCapacity(rbuf);
    CU_ASSERT_EQUAL(wcap, _capacity);
    STARB_CAPTYPE rcap = StaticRingBuf_GetReadCapacity(rbuf);
    CU_ASSERT_EQUAL(rcap, 0);
}

static void Verify_STARB_Release(StaticRingBuf* const rbuf)
{
    CU_ASSERT_PTR_NULL(rbuf->buffer);
    CU_ASSERT_EQUAL(rbuf->capacity, 0);
    CU_ASSERT_EQUAL(rbuf->wpos, 0);
    CU_ASSERT_EQUAL(rbuf->rpos, 0);
    CU_ASSERT_EQUAL(rbuf->flag.zeros, 0);
    CU_ASSERT_EQUAL(rbuf->flag.ownbuf, 0);
    CU_ASSERT_EQUAL(rbuf->flag.cycle, 0);
}

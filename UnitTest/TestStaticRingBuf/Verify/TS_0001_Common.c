#include <stdlib.h>
#include <string.h>
#include "CUnit/CUnit.h"
#include "TS_0001.h"
#include "StaticRingBuf.h"
#include "TS_0001_TestData.h"

/** @par Private (Static) data declaration
 */

static byte STOBUF[2 * RB_LENGTH] = { 0 };  // StaticRingBuf storage buffer
static byte ReadBuf[RB_LENGTH] = { 0 };     // Read output buffer

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
    if (rc == STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
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
    STARB_CAPTYPE wcap, rcap;

    const STARB_CAPTYPE Capacity = 4;

    uint8_t rc = StaticRingBuf_Init(&rbuf, Capacity, STOBUF);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }
    Verify_STARB_Init(&rbuf, Capacity);

    // Insert src[0] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[0]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 1);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 1);
    CU_ASSERT_EQUAL(rcap, 1);

    // Insert src[1] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[1]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 2);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 1], src[1]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 2);
    CU_ASSERT_EQUAL(rcap, 2);

    // Insert src[2] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[2]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 3);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[2], src[2]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 2], src[2]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 3);
    CU_ASSERT_EQUAL(rcap, 3);

    // Insert src[3] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[3]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[2], src[2]);
    CU_ASSERT_EQUAL(rbuf.buffer[3], src[3]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 2], src[2]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 3], src[3]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 0);
    CU_ASSERT_EQUAL(rcap, 4);

    // Insert src[4] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[4]);
    if (rc != STARB_BUFOVERFLOW)
    {
        CU_ASSERT_EQUAL(rc, STARB_BUFOVERFLOW);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[2], src[2]);
    CU_ASSERT_EQUAL(rbuf.buffer[3], src[3]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 2], src[2]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 3], src[3]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 0);
    CU_ASSERT_EQUAL(rcap, 4);

EXIT:
    StaticRingBuf_Release(&rbuf);
}

/** @note STARB_CAPTYPE is uint16_t */
void TC0004_STARB_TestCapBound_Write(void)
{
    StaticRingBuf rbuf = { 0 };
    byte* src = Get_ByteArray0();
    STARB_CAPTYPE wcap, rcap;

    const STARB_CAPTYPE Capacity = U16_MAX;

    uint8_t rc = StaticRingBuf_Create(&rbuf, Capacity);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }
    Verify_STARB_Create(&rbuf, Capacity);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity);
    CU_ASSERT_EQUAL(rcap, 0);

    // Insert src[0] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[0]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 1);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 1);
    CU_ASSERT_EQUAL(rcap, 1);

    // Insert src[1] into rbuf

    rc = StaticRingBuf_Write(&rbuf, src[1]);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 2);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[(size_t)Capacity + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[(size_t)Capacity + 1], src[1]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 2);
    CU_ASSERT_EQUAL(rcap, 2);

    // Insert data into rbuf until `U16_MAX - 1` items in the storage buffer

    STARB_CAPTYPE count = 2;
    int srcidx = 1;

    while (count < U16_MAX - 1)
    {
        srcidx = (srcidx >= BYTEARRAY0_ITEMS_COUNT - 1) ? 0 : (srcidx + 1);
        rc = StaticRingBuf_Write(&rbuf, src[srcidx]);
        count++;
    }

    CU_ASSERT_EQUAL(rc, STARB_OK);

    CU_ASSERT_EQUAL(rbuf.wpos, count);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 1);
    CU_ASSERT_EQUAL(rcap, count);

    // Insert 1 byte into rbuf

    srcidx = (srcidx >= BYTEARRAY0_ITEMS_COUNT - 1) ? 0 : (srcidx + 1);
    rc = StaticRingBuf_Write(&rbuf, src[srcidx]);
    count++;
    
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }
    
    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);

    CU_ASSERT_EQUAL(rbuf.buffer[count - 1], src[srcidx]);
    CU_ASSERT_EQUAL(rbuf.buffer[2 * (size_t)Capacity - 1], src[srcidx]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 0);
    CU_ASSERT_EQUAL(rcap, count);

    // Insert 1 byte into rbuf

    int newidx = (srcidx >= BYTEARRAY0_ITEMS_COUNT - 1) ? 0 : (srcidx + 1);
    rc = StaticRingBuf_Write(&rbuf, src[newidx]);
    if (rc != STARB_BUFOVERFLOW)
    {
        CU_ASSERT_EQUAL(rc, STARB_BUFOVERFLOW);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 0);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);

    CU_ASSERT_EQUAL(rbuf.buffer[count - 1], src[srcidx]);
    CU_ASSERT_EQUAL(rbuf.buffer[2 * (size_t)Capacity - 1], src[srcidx]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 0);
    CU_ASSERT_EQUAL(rcap, count);

EXIT:
    StaticRingBuf_Release(&rbuf);
}

void TC0005_STARB_WriteItems(void)
{
    StaticRingBuf rbuf = { 0 };
    byte* src = Get_ByteArray0();
    STARB_CAPTYPE wcap, rcap;

    uint8_t rc = StaticRingBuf_Init(&rbuf, RB_LENGTH, STOBUF);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }
    Verify_STARB_Init(&rbuf, RB_LENGTH);

    // Insert 12 items: Failed with error code 0xE2 (STARB_PARAMOUTRANGE)

    rc = StaticRingBuf_WriteItems(&rbuf, &src[0], 12);
    CU_ASSERT_EQUAL(rc, STARB_PARAMOUTRANGE);
    if (rc != STARB_PARAMOUTRANGE)
    {
        goto EXIT;
    }

    // Insert 3 items src[0..2] into rbuf

    rc = StaticRingBuf_WriteItems(&rbuf, &src[0], 3);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 3);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[2], src[2]);

    CU_ASSERT_EQUAL(rbuf.buffer[RB_LENGTH + 0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[RB_LENGTH + 1], src[1]);
    CU_ASSERT_EQUAL(rbuf.buffer[RB_LENGTH + 2], src[2]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, RB_LENGTH - 3);
    CU_ASSERT_EQUAL(rcap, 3);

    // Insert 6 items src[3..8] into rbuf

    rc = StaticRingBuf_WriteItems(&rbuf, &src[3], 6);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 9);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    for (uint8_t idx = 0; idx <= 8; idx++)
    {
        CU_ASSERT_EQUAL(rbuf.buffer[idx], src[idx]);
        CU_ASSERT_EQUAL(rbuf.buffer[RB_LENGTH + idx], src[idx]);
    }

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 1);
    CU_ASSERT_EQUAL(rcap, 9);

    // Insert 2 items: Failed with error code 0xE4 (STARB_BUFOVERFLOW)

    rc = StaticRingBuf_WriteItems(&rbuf, &src[9], 2);
    CU_ASSERT_EQUAL(rc, STARB_BUFOVERFLOW);
    if (rc != STARB_BUFOVERFLOW)
    {
        goto EXIT;
    }

    // Verify that wpos and rpos have not been changed.
    CU_ASSERT_EQUAL(rbuf.wpos, 9);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);
    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 1);
    CU_ASSERT_EQUAL(rcap, 9);

    // Read 5 items

    rc = StaticRingBuf_ReadItems(&rbuf, ReadBuf, 5);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    for (uint8_t idx = 0; idx < 5; idx++)
    {
        CU_ASSERT_EQUAL(ReadBuf[idx], src[idx]);
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 9);
    CU_ASSERT_EQUAL(rbuf.rpos, 5);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);
    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 6);
    CU_ASSERT_EQUAL(rcap, 4);

    // Insert 3 items src[9..11] into rbuf

    rc = StaticRingBuf_WriteItems(&rbuf, &src[9], 3);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[10]);
    CU_ASSERT_EQUAL(rbuf.buffer[1], src[11]);

    for (uint8_t idx = 2; idx <= 9; idx++)
    {
        CU_ASSERT_EQUAL(rbuf.buffer[idx], src[idx]);
        CU_ASSERT_EQUAL(rbuf.buffer[RB_LENGTH + idx], src[idx]);
    }

    CU_ASSERT_EQUAL(rbuf.buffer[10], src[10]);
    CU_ASSERT_EQUAL(rbuf.buffer[11], src[11]);

    CU_ASSERT_EQUAL(rbuf.wpos, 2);
    CU_ASSERT_EQUAL(rbuf.rpos, 5);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);
    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 3);
    CU_ASSERT_EQUAL(rcap, 7);

    // Read 7 items

    rc = StaticRingBuf_ReadItems(&rbuf, ReadBuf, 7);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    for (uint8_t idx = 0; idx < 7; idx++)
    {
        CU_ASSERT_EQUAL(ReadBuf[idx], src[5 + idx]);
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 2);
    CU_ASSERT_EQUAL(rbuf.rpos, 2);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);
    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, 10);
    CU_ASSERT_EQUAL(rcap, 0);

EXIT:
    StaticRingBuf_Release(&rbuf);
}

/** @note STARB_CAPTYPE is uint16_t */
void TC0006_STARB_TestCapBound_WriteItems(void)
{
    StaticRingBuf rbuf = { 0 };
    byte* src = Get_ByteArray0();
    STARB_CAPTYPE wcap, rcap;

    const STARB_CAPTYPE Capacity = U16_MAX;

    uint8_t rc = StaticRingBuf_Create(&rbuf, Capacity);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }
    Verify_STARB_Create(&rbuf, Capacity);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity);
    CU_ASSERT_EQUAL(rcap, 0);

    // Insert 1 byte src[0] into rbuf

    rc = StaticRingBuf_WriteItems(&rbuf, &src[0], 1);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, 1);
    CU_ASSERT_EQUAL(rbuf.rpos, 0);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    CU_ASSERT_EQUAL(rbuf.buffer[0], src[0]);
    CU_ASSERT_EQUAL(rbuf.buffer[Capacity + 0], src[0]);

    wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    rcap = StaticRingBuf_GetReadCapacity(&rbuf);
    CU_ASSERT_EQUAL(wcap, Capacity - 1);
    CU_ASSERT_EQUAL(rcap, 1);

    // Insert random bytes into rbuf

    STARB_CAPTYPE count;
    size_t temp_wpos;

    while (wcap > 10)
    {
        temp_wpos = rbuf.wpos;

        count = 7;
        if (count <= 0)
        {
            continue;
        }

        rc = StaticRingBuf_WriteItems(&rbuf, &src[0], count);
        if (rc != STARB_OK)
        {
            CU_ASSERT_EQUAL(rc, STARB_OK);
            goto EXIT;
        }

        if (rbuf.wpos != temp_wpos + count)
        {
            CU_ASSERT_EQUAL(rbuf.wpos, temp_wpos + count);
            goto EXIT;
        }

        wcap = StaticRingBuf_GetWriteCapacity(&rbuf);
    }

    // Read 10 items

    rc = StaticRingBuf_ReadItems(&rbuf, ReadBuf, 10);
    if (rc != STARB_OK)
    {
        CU_ASSERT_EQUAL(rc, STARB_OK);
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.rpos, 10);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 0);

    // Insert 10 bytes into rbuf

    temp_wpos = rbuf.wpos;

    rc = StaticRingBuf_WriteItems(&rbuf, &src[0], 10);
    CU_ASSERT_EQUAL(rc, STARB_OK);
    if (rc != STARB_OK)
    {
        goto EXIT;
    }

    CU_ASSERT_EQUAL(rbuf.wpos, temp_wpos + 10 - Capacity);
    CU_ASSERT_EQUAL(rbuf.rpos, 10);
    CU_ASSERT_EQUAL(rbuf.flag.cycle, 1);

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

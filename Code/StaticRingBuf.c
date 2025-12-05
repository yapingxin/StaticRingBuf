#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "StaticRingBuf.h"


/** @brief   Initialize the StaticRingBuf instance.
 *  @details Initialize the StaticRingBuf instance, and link `buffer` to the given existent `_buffer` pointer.
 * 
 *  @param[out] rbuf     The StaticRingBuf instance to be initialized
 *  @param[in] _capacity Logical storage capacity in bytes
 *  @param[in] _buffer   Pointer to the physical storage buffer
 *  
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 */
uint8_t StaticRingBuf_Init(StaticRingBuf* rbuf, const STARB_CAPTYPE _capacity, byte* _buffer)
{
    uint8_t rc = STARB_OK;

    if (rbuf == NULL || _capacity <= 0 || _buffer == NULL)
    {
        rc = STARB_PARAM_NULL;
        goto EXIT;
    }

    rbuf->buffer = _buffer;
    rbuf->capacity = _capacity;
    rbuf->wpos = 0;
    rbuf->rpos = 0;
    rbuf->flag.zeros = 0;
    rbuf->flag.ownbuf = 0;
    rbuf->flag.cycle = 0;

EXIT:
    return rc;
}

/** @brief   Initialize the StaticRingBuf instance.
 *  @details Initialize the StaticRingBuf instance, and dynamically create buffer memory space.
 *
 *  @param[out] rbuf     The StaticRingBuf instance to be initialized
 *  @param[in] _capacity Logical storage capacity in bytes
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE2 Failed: Dynamically create buffer memory space failed.
 */
uint8_t StaticRingBuf_Create(StaticRingBuf* rbuf, const STARB_CAPTYPE _capacity)
{
    uint8_t rc = STARB_OK;
    size_t bufsz;

    if (rbuf == NULL || _capacity <= 0)
    {
        rc = STARB_PARAM_NULL;
        goto EXIT;
    }

    bufsz = 2 * (size_t)_capacity;
    rbuf->buffer = malloc(bufsz * sizeof(byte));
    if (!rbuf->buffer)
    {
        rc = STARB_ALLOC_FAIL;
        goto EXIT;
    }
    memset(rbuf->buffer, 0, bufsz);

    rbuf->capacity = _capacity;
    rbuf->wpos = 0;
    rbuf->rpos = 0;
    rbuf->flag.zeros = 0;
    rbuf->flag.ownbuf = 1;
    rbuf->flag.cycle = 0;

EXIT:
    return rc;
}

/** @brief   Release the StaticRingBuf instance.
 *  @details Reset the attributes of the StaticRingBuf instance, release dynamically created buffer memory space.
 *
 *  @param[inout] rbuf The StaticRingBuf instance to be released.
 */
void StaticRingBuf_Release(StaticRingBuf* rbuf)
{
    if (rbuf)
    {
        if (rbuf->flag.ownbuf == 1 && rbuf->buffer != NULL)
        {
            free(rbuf->buffer);
        }
        memset(rbuf, 0, sizeof(StaticRingBuf));
    }
}

/** @brief Get writing capacity of the StaticRingBuf instance.
 *
 *  @param[in] rbuf The StaticRingBuf instance
 *  @return Writing capacity (bytes)
 */
STARB_CAPTYPE StaticRingBuf_GetWriteCapacity(StaticRingBuf* rbuf)
{
    assert(rbuf != NULL);

    STARB_CAPTYPE write_capacity;
    if (rbuf->flag.cycle == 0)
    {
        write_capacity =
            (rbuf->rpos > rbuf->wpos) ?
            0 :
            rbuf->capacity - rbuf->wpos + rbuf->rpos;
    }
    else
    {
        write_capacity =
            (rbuf->rpos < rbuf->wpos) ?
            0 :
            rbuf->rpos - rbuf->wpos;
    }
    return write_capacity;
}

/** @brief Get reading capacity of the StaticRingBuf instance.
 *
 *  @param[in] rbuf The StaticRingBuf instance
 *  @return Reading capacity (bytes)
 */
STARB_CAPTYPE StaticRingBuf_GetReadCapacity(StaticRingBuf* rbuf)
{
    assert(rbuf != NULL);
    STARB_CAPTYPE read_capacity;
    if (rbuf->flag.cycle == 0)
    {
        read_capacity =
            (rbuf->rpos > rbuf->wpos) ?
            0 :
            rbuf->wpos - rbuf->rpos;
    }
    else
    {
        read_capacity =
            (rbuf->rpos < rbuf->wpos) ?
            0 :
            rbuf->capacity - rbuf->rpos + rbuf->wpos;
    }
    return read_capacity;
}

/** @brief Write 1 byte into the StaticRingBuf instance's storage buffer.
 *
 *  @param[in] rbuf  The StaticRingBuf instance to be initialized
 *  @param[in] _elem Logical storage capacity in bytes
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE3 Failed: Buffer overflow.
 */
uint8_t StaticRingBuf_Write(StaticRingBuf* rbuf, const byte _elem)
{
    uint8_t rc = STARB_OK;

    if (rbuf == NULL)
    {
        rc = STARB_PARAM_NULL;
        goto EXIT;
    }

    STARB_CAPTYPE write_capacity = StaticRingBuf_GetWriteCapacity(rbuf);
    if (write_capacity <= 0)
    {
        rc = STARB_BUFOVERFLOW;
        goto EXIT;
    }

    rbuf->buffer[rbuf->wpos] = _elem;
#if STARB_TD_COPYMIRRORATLAST == 0
    if ((rbuf->wpos + 1) < rbuf->capacity)
    {
        rbuf->buffer[(size_t)rbuf->wpos + (size_t)rbuf->capacity] = _elem;
    }
#else
    rbuf->buffer[(size_t)rbuf->wpos + (size_t)rbuf->capacity] = _elem;
#endif

    if (rbuf->flag.cycle == 0 && (rbuf->wpos + 1) >= rbuf->capacity)
    {
        rbuf->flag.cycle = 1;
        rbuf->wpos = 0;
    }
    else
    {
        rbuf->wpos++;
    }

EXIT:
    return rc;
}

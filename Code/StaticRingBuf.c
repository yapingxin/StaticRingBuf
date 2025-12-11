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
 *  @retval 0xE3 Failed: Dynamically create buffer memory space failed.
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
 *  @param[in] rbuf     The StaticRingBuf instance
 *  @param[in] _elem    Content to be written
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE4 Failed: Buffer overflow.
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
    rbuf->buffer[(size_t)rbuf->wpos + (size_t)rbuf->capacity] = _elem;

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

/** @brief Write bytes into the StaticRingBuf instance's storage buffer.
 *
 *  @param[in] rbuf         The StaticRingBuf instance
 *  @param[in] srcbuf       Pointer to source data buffer to be written
 *  @param[in] writecount   Data length to be written
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE2 Failed: Input parameter is out of range.
 *  @retval 0xE4 Failed: Buffer overflow.
 */
uint8_t StaticRingBuf_WriteItems(StaticRingBuf* rbuf, byte* srcbuf, const STARB_CAPTYPE writecount)
{
    uint8_t rc = STARB_OK;

    if (writecount <= 0)
    {
        goto EXIT;
    }

    if (rbuf == NULL || srcbuf == NULL)
    {
        rc = STARB_PARAM_NULL;
        goto EXIT;
    }

    if (writecount > rbuf->capacity)
    {
        rc = STARB_PARAMOUTRANGE;
        goto EXIT;
    }

    STARB_CAPTYPE write_capacity = StaticRingBuf_GetWriteCapacity(rbuf);
    if (write_capacity <= 0 || writecount > write_capacity)
    {
        rc = STARB_BUFOVERFLOW;
        goto EXIT;
    }

    byte* dst = rbuf->buffer + rbuf->wpos;
    memcpy((void*)dst, srcbuf, writecount);
    if (rbuf->wpos <= rbuf->capacity - writecount)
    {
        dst += rbuf->capacity;
        memcpy((void*)dst, srcbuf, writecount);
    }
    else
    {
        STARB_CAPTYPE countL = rbuf->capacity - rbuf->wpos;
        dst += rbuf->capacity;
        memcpy((void*)dst, srcbuf, countL);
        size_t countR = (size_t)rbuf->wpos + (size_t)writecount - (size_t)rbuf->capacity;
        byte* src = rbuf->buffer + rbuf->capacity;
        memcpy((void*)rbuf->buffer, src, countR);
    }

    if (rbuf->wpos >= rbuf->capacity - writecount)
    {
        rbuf->wpos = (STARB_CAPTYPE)((size_t)rbuf->wpos + (size_t)writecount - (size_t)rbuf->capacity);
        rbuf->flag.cycle = 1;
    }
    else
    {
        rbuf->wpos = rbuf->wpos + writecount;
    }

EXIT:
    return rc;
}

/** @brief Read 1 byte from the StaticRingBuf instance's storage buffer.
 *
 *  @param[in] rbuf     The StaticRingBuf instance
 *  @param[out] _elem   Pointer to output the read content
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE5 Failed: No (enough) data.
 *  @retval 0xE6 Failed: Invalid data.
 */
uint8_t StaticRingBuf_Read(StaticRingBuf* rbuf, byte* _elem)
{
    return StaticRingBuf_ReadItems(rbuf, _elem, 1);
}

/** @brief Read bytes from the StaticRingBuf instance's storage buffer.
 *
 *  @param[in] rbuf      The StaticRingBuf instance
 *  @param[out] outbuf   Pointer to the output buffer to hold the read result
 *  @param[in] readcount The length to read (unit: bytes)
 *
 *  @retval 1    Executed successfully.
 *  @retval 0xE1 Failed: Has empty input parameter.
 *  @retval 0xE2 Failed: Input parameter is out of range.
 *  @retval 0xE5 Failed: No (enough) data.
 *  @retval 0xE6 Failed: Invalid data.
 */
uint8_t StaticRingBuf_ReadItems(StaticRingBuf* rbuf, byte* outbuf, const STARB_CAPTYPE readcount)
{
    uint8_t rc = STARB_OK;
    STARB_CAPTYPE rpos_next;

    if (readcount <= 0)
    {
        goto EXIT;
    }

    if (rbuf == NULL || outbuf == NULL)
    {
        rc = STARB_PARAM_NULL;
        goto EXIT;
    }

    if (readcount > rbuf->capacity)
    {
        rc = STARB_PARAMOUTRANGE;
        goto EXIT;
    }

    STARB_CAPTYPE read_capacity = StaticRingBuf_GetReadCapacity(rbuf);
    if (read_capacity <= 0 || readcount > read_capacity)
    {
        rc = STARB_NOENOUGHDAT;
        goto EXIT;
    }

    if (rbuf->rpos >= rbuf->capacity - readcount)
    {
        if (rbuf->flag.cycle == 1)
        {
            rpos_next = (STARB_CAPTYPE)((ssize_t)rbuf->rpos + (ssize_t)readcount - (ssize_t)rbuf->capacity);
            rbuf->flag.cycle = 0;
        }
        else
        {
            rc = STARB_DATAINVALID;
            goto EXIT;
        }
    }
    else
    {
        rpos_next = rbuf->rpos + readcount;
    }

    memcpy(outbuf, (rbuf->buffer + rbuf->rpos), readcount);
    rbuf->rpos = rpos_next;

EXIT:
    return rc;
}

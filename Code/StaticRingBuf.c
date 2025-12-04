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

    if (rbuf == NULL || _capacity == 0 || _buffer == NULL)
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

    if (rbuf == NULL || _capacity == 0)
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

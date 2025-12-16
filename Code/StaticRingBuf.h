/*****************************************************************************
 *  @file     StaticRingBuf.h                                                *
 *  @brief    Interface definition of the Static Ring Buffer.                *
 *                                                                           *
 *  @author   Yaping Xin                                                     *
 *  @link     https://github.com/yapingxin                                   *
 *  @version  0.1.0.0                                                        *
 *  @date     2025-12-04                                                     *
 *---------------------------------------------------------------------------*
 *  Copyright 2025 Yaping Xin                                                *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *---------------------------------------------------------------------------*
 *  Change History :                                                         *
 *---------------------------------------------------------------------------*
 *  2025/12/04 | 0.1.0.0   | Yaping Xin     | Create file                    *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *************************************************************************** */

#ifndef _INC_GH2025_StaticRingBuf_H
#define _INC_GH2025_StaticRingBuf_H

#include "Common/datatypes.h"

#define STARB_OK            1
#define STARB_PARAM_NULL    0xE1
#define STARB_PARAMOUTRANGE 0xE2
#define STARB_ALLOC_FAIL    0xE3
#define STARB_BUFOVERFLOW   0xE4
#define STARB_NOENOUGHDAT   0xE5
#define STARB_DATAINVALID   0xE6

/** @brief Datatype redefinition */
typedef uint16_t STARB_CAPTYPE;

/** @brief Tech Decision Macros */

/** @brief Ring buffer flag structure */
typedef struct _STARB_Flag_t {
    uint8_t zeros  : 6;
    uint8_t ownbuf : 1;
    uint8_t cycle  : 1;
} STARB_Flag;

/** \brief  Struct definition for the Static Ring Buffer.
 * \details The Static Ring Buffer is a kind of circular buffer.
 */
typedef struct _StaticRingBuf_t
{
    byte* buffer;           // Pointer to the physical storage buffer.
    STARB_CAPTYPE capacity; // Logical storage capacity in bytes.
    STARB_CAPTYPE wpos;     // Write position index (base 0)
    STARB_CAPTYPE rpos;     // Read  position index (base 0)
    STARB_Flag    flag;     // The flag (status)

} StaticRingBuf;

/** @brief Ring buffer Overwrite data structure */
typedef struct _STARB_Overwrite_t {
    byte* data;
    STARB_CAPTYPE pos;
    STARB_CAPTYPE count;
} STARB_Overwrite;

/** @brief Callback function definition to log the  Ring buffer Overwrite data */
typedef void (*STARB_OverwriteLogFunc)(STARB_Overwrite* log);

#ifdef __cplusplus
extern "C" {
#endif

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
    uint8_t StaticRingBuf_Init(StaticRingBuf* rbuf, const STARB_CAPTYPE _capacity, byte* _buffer);

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
    uint8_t StaticRingBuf_Create(StaticRingBuf* rbuf, const STARB_CAPTYPE _capacity);

    /** @brief   Release the StaticRingBuf instance.
     *  @details Reset the attributes of the StaticRingBuf instance, release dynamically created buffer memory space.
     *
     *  @param[inout] rbuf The StaticRingBuf instance to be released.
     */
    void StaticRingBuf_Release(StaticRingBuf* rbuf);

    /** @brief Get writing capacity of the StaticRingBuf instance.
     *
     *  @param[in] rbuf The StaticRingBuf instance
     *  @return Writing capacity (bytes)
     */
    STARB_CAPTYPE StaticRingBuf_GetWriteCapacity(StaticRingBuf* rbuf);

    /** @brief Get reading capacity of the StaticRingBuf instance.
     *
     *  @param[in] rbuf The StaticRingBuf instance
     *  @return Reading capacity (bytes)
     */
    STARB_CAPTYPE StaticRingBuf_GetReadCapacity(StaticRingBuf* rbuf);

    /** @brief Write 1 byte into the StaticRingBuf instance's storage buffer.
     *
     *  @param[in] rbuf     The StaticRingBuf instance
     *  @param[in] _elem    Content to be written
     *
     *  @retval 1    Executed successfully.
     *  @retval 0xE1 Failed: Has empty input parameter.
     *  @retval 0xE4 Failed: Buffer overflow.
     */
    uint8_t StaticRingBuf_Write(StaticRingBuf* rbuf, const byte _elem);

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
    uint8_t StaticRingBuf_WriteItems(StaticRingBuf* rbuf, byte* srcbuf, const STARB_CAPTYPE writecount);

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
    uint8_t StaticRingBuf_Read(StaticRingBuf* rbuf, byte* _elem);

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
    uint8_t StaticRingBuf_ReadItems(StaticRingBuf* rbuf, byte* outbuf, const STARB_CAPTYPE readcount);

    /** @brief Read bytes from the StaticRingBuf instance's storage buffer without moving the read position.
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
    uint8_t StaticRingBuf_PeekItems(StaticRingBuf* rbuf, byte* outbuf, const STARB_CAPTYPE readcount);

    /** @brief Get reading pointer of the StaticRingBuf instance. */
    byte* StaticRingBuf_GetReadPtr(StaticRingBuf* rbuf);

    /** @brief Get writing pointer of the StaticRingBuf instance. */
    byte* StaticRingBuf_GetWritePtr(StaticRingBuf* rbuf);

    /** @brief Move reading pointer forward (skip bytes) for the StaticRingBuf instance.
     *
     *  @param[in] rbuf      The StaticRingBuf instance
     *  @param[in] readcount The length to move forward (skip)
     *
     *  @retval 1    Executed successfully.
     *  @retval 0xE1 Failed: Has empty input parameter.
     *  @retval 0xE2 Failed: Input parameter is out of range.
     *  @retval 0xE5 Failed: No (enough) data.
     *  @retval 0xE6 Failed: Invalid data.
     */
    uint8_t StaticRingBuf_Forward(StaticRingBuf* rbuf, const STARB_CAPTYPE skipcount);

    uint8_t StaticRingBuf_ForceWriteItemsWithLog(
        StaticRingBuf* rbuf, byte* srcbuf, const STARB_CAPTYPE writecount,
        STARB_Overwrite* pOverwrite, STARB_OverwriteLogFunc logcallback);

#ifdef __cplusplus
} // ! extern "C"
#endif

#endif // !_INC_GH2025_StaticRingBuf_H

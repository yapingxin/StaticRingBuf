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
 *  2025/12/04 | 0.1.0.0   | Yaping Xin | Create file                        *
 *  2026/01/04 | 0.2.0.0   | Yaping Xin | Remove the dynamic memory creation *
 *                         | for non-critical usage. (Thanks Fei)            *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *************************************************************************** */

#ifndef _INC_GH2025_StaticRingBuf_H
#define _INC_GH2025_StaticRingBuf_H

#include "Common/datatypes.h"

#define STARB_OK            1
#define STARB_PARAM_NULL    0xE1
#define STARB_PARAMOUTRANGE 0xE2
#define STARB_BUFOVERFLOW   0xE4
#define STARB_NOENOUGHDAT   0xE5
#define STARB_DATAINVALID   0xE6

/** @brief Datatype redefinition */
typedef uint16_t STARB_CAPTYPE;
typedef uint32_t STARB_STOSZTP;

/** @brief Define ring buffer's maxium capacity */
#define STARB_MAXCAP    (2 * UINT16_MAX)

/** @brief Tech Decision Macros */

/** @brief Ring buffer flag structure */
typedef struct _STARB_Flag_t {
    uint8_t zeros  : 7;
    uint8_t cycle  : 1;
} STARB_Flag;

/** \brief  Struct definition for the Static Ring Buffer.
 * \details The Static Ring Buffer is a kind of circular buffer.
 */
typedef struct _StaticRingBuf_t
{
    byte* buffer;                   // Pointer to the physical storage buffer.
    STARB_CAPTYPE capacity;         // Logical storage capacity in bytes.
    volatile STARB_CAPTYPE wpos;    // Write position index (base 0)
    volatile STARB_CAPTYPE rpos;    // Read  position index (base 0)
    volatile STARB_Flag    flag;    // The flag (status)

} StaticRingBuf;

/** @brief Ring buffer lost data structure */
typedef struct _STARB_Lostdata_t {
    byte* dataptr;
    STARB_CAPTYPE pos;
    STARB_CAPTYPE count;
} STARB_Lostdata;

/** @brief Callback function definition to log the ring buffer lost data */
typedef void (*STARB_LostdataLogFunc)(STARB_Lostdata* log);

#ifdef __cplusplus
extern "C" {
#endif

    /** @brief   Initialize the StaticRingBuf instance.
     *  @details Initialize the StaticRingBuf instance, and link `buffer` to the given existent `_buffer` pointer.
     * 
     * CRITICAL NOTE: This library uses a Mirrored Buffer technique.
     * The 'total_buffer_size' provided must be at least 2 bytes.
     * The logical capacity will be exactly HALF of the provided buffer size.
     *
     *  @param[out] rbuf             The StaticRingBuf instance to be initialized
     *  @param[in] total_buffer_size Physical size for the storage buffer
     *  @param[in] _buffer           Pointer to the physical storage buffer
     *
     *  @retval 1    Executed successfully.
     *  @retval 0xE1 Failed: Has empty input parameter.
     *  @retval 0xE2 Failed: Input parameter is out of range.
     */
    uint8_t StaticRingBuf_Init(StaticRingBuf* rbuf, const STARB_STOSZTP total_buffer_size, byte* _buffer);

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
        STARB_Lostdata* pOverwrite, STARB_LostdataLogFunc logcallback);

#ifdef __cplusplus
} // ! extern "C"
#endif

#endif // !_INC_GH2025_StaticRingBuf_H

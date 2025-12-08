/*****************************************************************************
 *  @file     Common/datatypes.h                                             *
 *  @brief    Common data types definitions.                                 *
 *                                                                           *
 *  @author   Yaping Xin                                                     *
 *  @link     https://github.com/yapingxin                                   *
 *  @version  0.1.0.0                                                        *
 *  @date     2025-12-03                                                     *
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
 *  2025/12/03 | 0.1.0.0   | Yaping Xin     | Create file                    *
 *---------------------------------------------------------------------------*
 *                                                                           *
 *************************************************************************** */

#ifndef _INC_COMMON_datatypes_H
#define _INC_COMMON_datatypes_H

#include <stdbool.h>
#include <stddef.h>

#if _MSC_VER && _MSC_VER < 1700
typedef __int8              int8_t;
typedef __int16             int16_t;
typedef __int32             int32_t;
typedef __int64             int64_t;
typedef unsigned __int8     uint8_t;
typedef unsigned __int16    uint16_t;
typedef unsigned __int32    uint32_t;
typedef unsigned __int64    uint64_t;
#else
#include <stdint.h>
#endif

typedef float           float32_t;
typedef double          float64_t;
typedef unsigned char	byte;
typedef char            sbyte;

#ifdef _WIN64
#define ssize_t __int64
#else
#define ssize_t long
#endif

#define U16_MAX  0xFFFF
#define U32_MAX  0xFFFFFFFF

#endif // !_INC_COMMON_datatypes_H

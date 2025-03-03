/*
 * Copyright (c) 2009-2025 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EXECTB_TESTS_H__
#define __EXECTB_TESTS_H__

/*============================ INCLUDES ======================================*/
#if defined(_RTE_)
#   include "RTE_Components.h"
#endif

#include "perf_counter.h"
#include <stdio.h>

#include "exectb_mcu.h"
#include "EXECTB_Config.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*============================ MACROS ========================================*/

#define MAX_TEST_OCCURENCES         16
#define ITER_NUM                    32
#define EXTRA_TAIL_CHK              8

#define TEST_PASS 0
#define TEST_FAIL 1

/*============================ MACROFIED FUNCTIONS ===========================*/

#if   __IS_COMPILER_ARM_COMPILER_5__
    #define dbgInst(imm) __asm volatile{ DBG (imm) }
#elif defined ( __GNUC__ ) || defined ( __llvm__ )
    #define dbgInst(imm) __asm volatile("DBG %0\n\t" : :"Ir" ((imm)) )
#else
    #error "Unsupported compiler"
#endif

#define startSection(x)                                                         \
    do {                                                                        \
        start_cycle_counter();                                                  \
        dbgInst(((x) & 0x7) | 0x8);                                             \
    } while(0)
    
#define stopSection(x)                                                          \
    do {                                                                        \
        dbgInst(((x) & 0x7) | 0x0);                                             \
        cyclesArray[cyclesArrayCnt++] = stop_cycle_counter();                   \
    } while(0)

#undef ABS
#define ABS(a) \
  ({ __typeof__ (a) _a = (a); \
    _a <0 ? -_a : _a; })

#define PRINT(fmt, args...) printf( fmt, ##args)

#define dump_buf(a, buf_sz, wrap, format )                            \
{                                                                     \
    printf("%s:\n", #a);                                              \
    for (int i = 0; i < buf_sz; i++)                                  \
        printf(i % wrap == wrap - 1 ? format",\n":format", ", a[i]);  \
    printf("\n");                                                     \
}

#define check_for_error(a, b, buf_sz, thresh )                              \
{                                                                           \
    for (int i = 0; i < buf_sz; i++)                                        \
        if (ABS(a[i] - b[i]) > thresh)                                      \
        {                                                                   \
            /* force integer format for RTL */                              \
            PRINT("[%s] error idx %d %x %x\n", #a, i,(int)a[i],(int)b[i]);  \
            errorCnt++;                                                     \
            break;                                                          \
        }                                                                   \
}


////////////////////////////////////////////////////////////////////////////////
//
// Print macro to allow conditional printing of messages
//
// Invoke like printf, but use double braces: MSG(("format %x", var));
//
#ifdef EXECTB_PRINTF
#define MSG(arg) { printf arg ; }
#else
#define MSG(arg) { (void) 0; }
#endif

#ifdef DEBUGTRACE_PRINTF
#define TRC_MSG(arg) { printf arg ; }
#else
#define TRC_MSG(arg) { (void) 0; }
#endif

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern int32_t cyclesArray[];
extern int32_t cyclesArrayCnt;

/*============================ PROTOTYPES ====================================*/

extern void EXECTB_Init(void);

__STATIC_INLINE
void enableCde(void)
{
    SCB->CPACR = SCB->CPACR | 0xFFFF;
    __DSB();
    __ISB();
}

#ifdef __cplusplus
}
#endif

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#endif

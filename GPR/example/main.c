/*
 * Copyright (c) 2009-2024 Arm Limited. All rights reserved.
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

/*============================ INCLUDES ======================================*/
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <assert.h>

#include "aci_gpr_lib.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif

/*============================ MACROS ========================================*/
#ifndef ITERATION_COUNT
#   define ITERATION_COUNT  10000
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/

__NO_INIT
static
uint32_t s_wInput[ITERATION_COUNT];

__NO_INIT
static
volatile
uint32_t s_wOutputC[ITERATION_COUNT];

__NO_INIT
static
volatile
uint32_t s_wOutputACI[ITERATION_COUNT];

/*============================ IMPLEMENTATION ================================*/

static
uint32_t popcount(uint32_t x) 
{
    uint32_t n = 0;
    for (int i = 0; i < 32; ++i) {
        n += (x >> i) & 1;
    }

    return n;
}


void population_count_test_in_c(    uint32_t *pwInput, 
                                    volatile uint32_t *pwOutput, 
                                    size_t tSize)
{
    assert(NULL != pwInput);
    assert(NULL != pwOutput);
    assert(tSize > 0);

    do {
        *pwOutput++ = popcount(*pwInput++);
    } while(--tSize);
}


void population_count_test_in_aci(  uint32_t *pwInput, 
                                    volatile uint32_t *pwOutput, 
                                    size_t tSize)
{
    assert(NULL != pwInput);
    assert(NULL != pwOutput);
    assert(tSize > 0);

    uint32_t wTotal = 0;

    do {
        *pwOutput++ = popc_u32(*pwInput++);
    } while(--tSize);
}

static
void prepare_test(uint32_t *pwData, size_t tSize)
{
    assert(NULL != pwData);
    assert(tSize > 0);

    srand(get_system_ticks());

    do {
        *pwData++ = (uint32_t)rand();
    } while(--tSize);
}

static
bool validation_result(void)
{
    printf("\r\n""Validate the output...");
    uint32_t wErrors = 0;
    for (size_t n = 0; n < dimof(s_wInput); n++) {
        if (s_wOutputC[n] != s_wOutputACI[n]) {
            printf("\r\nERROR: Expect 0x%08x, but get 0x%08x at index: %d",
                   s_wOutputC[n],
                   s_wOutputACI[n],
                   n);
            wErrors++;
        }
    }

    if (0 == wErrors) {
        printf("PASS!\r\n");
        
        return true;
    }
    
    printf("\r\nFailed! %d errors were detected.\r\n", wErrors);
    return false;
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) 
{
    stdio_init();
    aci_init();

    prepare_test(s_wInput, dimof(s_wInput));

    __cycleof__("Calculate Population Count in C") {
        population_count_test_in_c(s_wInput, s_wOutputC, dimof(s_wInput));
    }

    __cycleof__("Calculate Population Count in ACI") {
        population_count_test_in_aci(s_wInput, s_wOutputACI, dimof(s_wInput));
    }

    return validation_result() ? 0 : -1;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif



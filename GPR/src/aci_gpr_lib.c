/*
 * Copyright (C) 2010-2024 Arm Limited or its affiliates. All rights reserved.
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

/* ----------------------------------------------------------------------
 * Project:      ACI Library
 * Title:        aci_gpr_lib.c
 * Description:  ACI example accessing General Purpose Registers
 *
 * $Date:        30 Dec 2024
 * $Revision:    V.1.0.0
 *
 * Target Processor:  Cortex-M processors that support ACI
 *
 * -------------------------------------------------------------------- */

/*============================ INCLUDES ======================================*/
#ifdef _RTE_
#   include <RTE_Components.h>
#   include CMSIS_device_header
#else
//#   include <your device header file>
#endif

#include "aci_gpr_lib.h"

/*============================ MACROS ========================================*/

#define COPRO_FULL_ACCESS 0x3U

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/



void aci_init(void)
{
#ifdef __ARM_FEATURE_CDE
    if (__ARM_FEATURE_CDE_COPROC & (1 << ACI_GPR_LIB_COPRO_ID)) {
        SCB->CPACR |= COPRO_FULL_ACCESS << (ACI_GPR_LIB_COPRO_ID * 2);
        __ISB();
    }
#endif
}

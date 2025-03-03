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
 * Title:        aci_gpr_lib.h
 * Description:  ACI example accessing General Purpose Registers
 *
 * $Date:        30 Dec 2024
 * $Revision:    V.1.0.0
 *
 * Target Processor:  Cortex-M processors that support ACI
 *
 * -------------------------------------------------------------------- */

#ifndef __ACI_GPR_LIB_H__
#define __ACI_GPR_LIB_H__

/*============================ INCLUDES ======================================*/
#include <arm_cde.h>
#include <cmsis_compiler.h>

/*============================ MACROS ========================================*/
/* The following library uses CDE coprocessor 0 */
#define ACI_GPR_LIB_COPRO_ID  0

#ifdef __ARM_FEATURE_CDE

#if (__ARM_FEATURE_CDE_COPROC & (0x1 << ACI_LIB_COPRO_ID) ) == 0
#error "__ARM_FEATURE_CDE_COPROC does not have correct feature bits set"
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/

/*!
 * \brief calculate the population of a 32-bit word
 * \param[in] value   32-bit input word
 * \return uint32_t number of 1s bits in 32-bit word
 */
__STATIC_INLINE
uint32_t popc_u32(uint32_t value)
{
    return __arm_cx1a(ACI_GPR_LIB_COPRO_ID, /* coprocessor ID for current ACI */
                      value,                /* input word */
                      0);                   /* imm=0 to identify custom instruction */
}


/*!
 * \brief initialize ACI extension
 */
extern
void aci_init(void);

#endif                          /* __ARM_FEATURE_CDE  */
#endif                          /* __ARM_2D_RGB565_ACI_LIB_H__ */

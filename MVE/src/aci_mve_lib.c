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
 * Project:      Arm-2D RGB565 ACI Library
 * Title:        aci_mve_lib.c
 * Description:  Provides RGB565 ACI acceleration
 *
 * $Date:        30 Dec 2024
 * $Revision:    V.1.2.0
 *
 * Target Processor:  Cortex-M cores with Helium & RGB565 Vector CDE
 *
 * -------------------------------------------------------------------- */

#ifdef _RTE_
#   include <RTE_Components.h>
#   include CMSIS_device_header
#else
//#   include <your device header file>
#endif

#include "aci_mve_lib.h"

void __arm_2d_impl_rgb565_des_msk_copy_aci(uint16_t * __RESTRICT pSourceBase,
                                           int16_t iSourceStride,
                                           uint16_t * __RESTRICT pTargetBase,
                                           int16_t iTargetStride,
                                           uint8_t * __RESTRICT ptTargetMaskBase,
                                           int16_t iTargetMaskStride,
                                           arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;
    uint16x8_t      v127 = vdupq_n_u16(127);
    uint8_t        *ptTargetMask = ptTargetMaskBase;

    for (int_fast16_t y = 0; y < iHeight; y++) {


        uint16_t       *__RESTRICT ptSrc = pSourceBase;
        uint16_t       *__RESTRICT ptTargetCur = pTargetBase;
        uint8_t        *__RESTRICT ptTargetMaskCur = ptTargetMask;

        int32_t         blkCnt = iWidth;

        do {
            mve_pred16_t    p = vctp16q((uint32_t) blkCnt);
            uint16x8_t      vecTarget = vld1q_z(ptTargetCur, p);
            uint16x8_t      vecSource = vld1q_z(ptSrc, p);
            uint16x8_t      vecTargetMask = vldrbq_z_u16(ptTargetMaskCur, p);
            
            vecTargetMask = v127 - (vecTargetMask >> 1);

            vecTarget = vblda7q_m_rgb565(   vecTarget, 
                                            vecSource, 
                                            vecTargetMask, 
                                            vcmpneq(vecTargetMask, v127));

            vst1q_p(ptTargetCur, vecTarget, p);

            ptTargetMaskCur += (128 / 16);
            ptTargetCur += (128 / 16);
            ptSrc += (128 / 16);
            blkCnt -= (128 / 16);
        } while (blkCnt > 0);

        pSourceBase += (iSourceStride);
        pTargetBase += (iTargetStride);
        ptTargetMask += (iTargetMaskStride);
    }
}

static void __arm_2d_cde_rgb565_init(void)
{
#define COPRO_FULL_ACCESS 0x3U

    SCB->CPACR |= COPRO_FULL_ACCESS << (ARM_2D_RGB565_ACI_LIB_COPRO_ID * 2);
    __ISB();
}

void __arm_2d_aci_init(void)
{
#ifdef __ARM_FEATURE_CDE
    if (__ARM_FEATURE_CDE_COPROC & (1 << ARM_2D_RGB565_ACI_LIB_COPRO_ID))
        __arm_2d_cde_rgb565_init();
#endif
}

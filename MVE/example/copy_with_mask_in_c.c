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
#include "platform.h"

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
/*============================ MACROFIED FUNCTIONS ===========================*/
#ifndef __ARM_2D_PIXEL_BLENDING_RGB565
#   define __ARM_2D_PIXEL_BLENDING_RGB565(__SRC_ADDR, __DES_ADDR, __TRANS)      \
            do {                                                                \
                uint16_t hwOPA = 256 - (__TRANS);                               \
                __arm_2d_color_fast_rgb_t tSrcPix, tTargetPix;                  \
                uint16_t *phwTargetPixel = (__DES_ADDR);                        \
                __arm_2d_rgb565_unpack(*(__SRC_ADDR), &tSrcPix);                \
                __arm_2d_rgb565_unpack(*phwTargetPixel, &tTargetPix);           \
                                                                                \
                for (int i = 0; i < 3; i++) {                                   \
                    uint16_t        hwTemp =                                    \
                        (uint16_t) (tSrcPix.BGRA[i] * hwOPA) +                  \
                        (tTargetPix.BGRA[i] * (__TRANS));                       \
                    tTargetPix.BGRA[i] = (uint16_t) (hwTemp >> 8);              \
                }                                                               \
                                                                                \
                /* pack merged stream */                                        \
                *phwTargetPixel = __arm_2d_rgb565_pack(&tTargetPix);            \
            } while(0)
#endif


/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ IMPLEMENTATION ================================*/

/*!
 * \brief unpack a rgb565 colour into a given __arm_2d_color_fast_rgb_t object
 * \param[in] hwColour the target rgb565 colour
 * \param[in] ptRGB a __arm_2d_color_fast_rgb_t object
 */
ARM_NONNULL(2)
__STATIC_INLINE void __arm_2d_rgb565_unpack(uint16_t hwColor,
                                            __arm_2d_color_fast_rgb_t * ptRGB)
{
    assert(NULL != ptRGB);
    
    /* uses explicit extraction, leading to a more efficient autovectorized code */
    uint16_t maskRunpk = 0x001f, maskGunpk = 0x003f;

    ptRGB->B = (uint16_t) ((hwColor & maskRunpk) << 3);
    ptRGB->R = (uint16_t) ((hwColor >> 11) << 3);
    ptRGB->G = (uint16_t) (((hwColor >> 5) & maskGunpk) << 2);
    
    ptRGB->A = 0xFF;
}

/*!
 * \brief generate a rgb565 colour from a __arm_2d_color_fast_rgb_t object 
 * \param[in] ptRGB the target __arm_2d_color_fast_rgb_t object
 * \return uint16_t a rgb565 colour
 */
ARM_NONNULL(1)
__STATIC_INLINE uint16_t __arm_2d_rgb565_pack(__arm_2d_color_fast_rgb_t * ptRGB)
{
    assert(NULL != ptRGB);
    
    arm_2d_color_rgb565_t tOutput = {
        .u5B = (uint16_t) (ptRGB->B >> 3),
        .u6G = (uint16_t) (ptRGB->G >> 2),
        .u5R = (uint16_t) (ptRGB->R >> 3),
    };
    return tOutput.tValue;
}


void __arm_2d_impl_rgb565_des_msk_copy(
                                    uint16_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    uint16_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    uint8_t * __RESTRICT ptTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    //arm_2d_size_t * __RESTRICT ptTargetMaskSize,
                                    arm_2d_size_t * __RESTRICT ptCopySize)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    uint8_t *ptTargetMask = ptTargetMaskBase;

    for (   int_fast16_t y = 0; y < iHeight; y++) {

        uint16_t       *__RESTRICT ptSrc = pSourceBase;
        uint16_t       *__RESTRICT ptTargetCur = pTargetBase;

        uint8_t        *__RESTRICT ptTargetMaskCur = ptTargetMask;
        int32_t         blkCnt = iWidth;


        do {
            uint16_t hwAlpha = *ptTargetMaskCur++;
            
            hwAlpha = 256 - hwAlpha;
            hwAlpha -= (hwAlpha == 1);
            
            __ARM_2D_PIXEL_BLENDING_RGB565(ptSrc++, ptTargetCur++, hwAlpha);

        } while (--blkCnt > 0);

        pSourceBase += (iSourceStride);
        pTargetBase += (iTargetStride);
        ptTargetMask += (iTargetMaskStride);
    }
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

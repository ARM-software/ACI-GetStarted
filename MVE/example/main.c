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

#include "aci_mve_lib.h"

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
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/

extern const arm_2d_tile_t c_tileCMSISLogoMask;
extern const arm_2d_tile_t c_tileCMSISLogoRGB565;

/*============================ PROTOTYPES ====================================*/
extern
void __arm_2d_impl_rgb565_des_msk_copy(
                                    uint16_t * __RESTRICT pSourceBase,
                                    int16_t iSourceStride,
                                    uint16_t * __RESTRICT pTargetBase,
                                    int16_t iTargetStride,
                                    uint8_t * __RESTRICT ptTargetMaskBase,
                                    int16_t iTargetMaskStride,
                                    //arm_2d_size_t * __RESTRICT ptTargetMaskSize,
                                    arm_2d_size_t * __RESTRICT ptCopySize);
/*============================ LOCAL VARIABLES ===============================*/


uint16_t s_hwFrameBuffer[GLCD_HEIGHT][GLCD_WIDTH];

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

 
void Disp0_DrawBitmap(  int16_t x, 
                        int16_t y, 
                        int16_t width, 
                        int16_t height, 
                        const uint8_t *bitmap)
{
    GLCD_DrawBitmap(x, y, width, height, bitmap);
}


void __arm_2d_impl_rgb565_des_msk_copy_mve(
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
    uint16x8_t      v256 = vdupq_n_u16(256);
    uint16x8_t      v0 = vdupq_n_u16(0);

    uint8_t *ptTargetMask = ptTargetMaskBase;

    for (   int_fast16_t y = 0;
            y < iHeight;
            y++) {

        uint16_t       *__RESTRICT ptSrc = pSourceBase;
        uint16_t       *__RESTRICT ptTargetCur = pTargetBase;

        uint8_t        *__RESTRICT ptTargetMaskCur = ptTargetMask;
        int32_t         blkCnt = iWidth;

        do {
            mve_pred16_t    p = vctp16q((uint32_t) blkCnt);
            uint16x8_t      vecTarget = vld1q_z(ptTargetCur, p);
            uint16x8_t      vecSource = vld1q_z(ptSrc, p);
            uint16x8_t      vecTargetMask = vldrbq_z_u16(ptTargetMaskCur, p);


            uint16x8_t      vecHwOpacity = v256 - vecTargetMask;

            vecHwOpacity = vpselq(  v0,
                                    vecHwOpacity,
                                    vcmpeqq_n_u16(vecHwOpacity, 1));

            vecTarget = __arm_2d_rgb565_blending_opacity_single_vec(
                                                                vecTarget, 
                                                                vecSource, 
                                                                vecHwOpacity);
            /* tail predication */
            vst1q_p(ptTargetCur,  vecTarget, p);

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



/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main (void) 
{
    stdio_init();
    __arm_2d_aci_init();

    printf("\r\nACI Example\r\n");


    /* clear the screen with WHITE */
    memset(s_hwFrameBuffer, 0xFF, sizeof(s_hwFrameBuffer));

    int16_t iXOffset = (GLCD_WIDTH - c_tileCMSISLogoRGB565.tRegion.tSize.iWidth)
                     / 2;
    int16_t iYOffset = c_tileCMSISLogoRGB565.tRegion.tSize.iHeight + 8;
    
    
    /* draw cmsis logo using C */
    __cycleof__("Draw CMSIS Logo using C") {
        
        __arm_2d_impl_rgb565_des_msk_copy(
            c_tileCMSISLogoRGB565.phwBuffer,
            c_tileCMSISLogoRGB565.tRegion.tSize.iWidth,
            &s_hwFrameBuffer[iYOffset * 0][iXOffset],
            GLCD_WIDTH,
            c_tileCMSISLogoMask.pchBuffer,
            c_tileCMSISLogoMask.tRegion.tSize.iWidth,
            (arm_2d_size_t *)&c_tileCMSISLogoRGB565.tRegion.tSize);

    }

    /* draw cmsis logo using MVE */
    __cycleof__("Draw CMSIS Logo using Helium") {
        
        __arm_2d_impl_rgb565_des_msk_copy_mve(
            c_tileCMSISLogoRGB565.phwBuffer,
            c_tileCMSISLogoRGB565.tRegion.tSize.iWidth,
            &s_hwFrameBuffer[iYOffset * 1][iXOffset],
            GLCD_WIDTH,
            c_tileCMSISLogoMask.pchBuffer,
            c_tileCMSISLogoMask.tRegion.tSize.iWidth,
            (arm_2d_size_t *)&c_tileCMSISLogoRGB565.tRegion.tSize);

    }

    /* draw cmsis logo using MVE */
    __cycleof__("Draw CMSIS Logo using Helium-ACI") {
        
        __arm_2d_impl_rgb565_des_msk_copy_aci(
            c_tileCMSISLogoRGB565.phwBuffer,
            c_tileCMSISLogoRGB565.tRegion.tSize.iWidth,
            &s_hwFrameBuffer[iYOffset * 2][iXOffset],
            GLCD_WIDTH,
            c_tileCMSISLogoMask.pchBuffer,
            c_tileCMSISLogoMask.tRegion.tSize.iWidth,
            (arm_2d_size_t *)&c_tileCMSISLogoRGB565.tRegion.tSize);

    }

    /* send framebuffer to the screen */
    Disp0_DrawBitmap(0,0, GLCD_WIDTH, GLCD_HEIGHT, (const uint8_t *)s_hwFrameBuffer);

    while(1) __NOP();
    return 0;
}

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif



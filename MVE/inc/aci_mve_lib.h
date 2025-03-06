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
 * Title:        aci_mve_lib.h
 * Description:  Provides RGB565 ACI acceleration
 *
 * $Date:        30 Dec 2024
 * $Revision:    V.1.2.0
 *
 * Target Processor:  Cortex-M cores with Helium & RGB565 Vector CDE
 *
 * -------------------------------------------------------------------- */

#ifndef __ARM_2D_RGB565_ACI_LIB_H__
#define __ARM_2D_RGB565_ACI_LIB_H__

/* The following library uses CDE coprocessor 0 */
#define ARM_2D_RGB565_ACI_LIB_COPRO_ID  0

#ifdef __ARM_FEATURE_CDE
/*
    // Mnemonic                  | Class          | Copro |  Opcode        | CDERTLID  | Comment
    //---------------------------|----------------|-------|----------------|-----------|----------
    // VRGB16UNPB<T>  Qd, Qm     | VCX2  (vector) |   0   |  7'b<T>000001  |   2/3/5   | Unpacking of Blue unpack
    // VRGB16UNPB<T>A Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>000001  |   2/3/5   | Unpacking of Blue unpack with accumulate
    // VRGB16UNPG<T>  Qd, Qm     | VCX2  (vector) |   0   |  7'b<T>000010  |   2/3/5   | Unpacking of green unpack
    // VRGB16UNPG<T>A Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>000010  |   2/3/5   | Unpacking of green unpack with accumulate
    // VRGB16UNPR<T>  Qd, Qm     | VCX2  (vector) |   0   |  7'b<T>000100  |   2/3/5   | Unpacking of Red unpack
    // VRGB16UNPR<T>A Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>000100  |   2/3/5   | Unpacking of Red unpack with accumulate
    // VRGB16PR<T>A   Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>001000  |    2/5    | Packing of red channel
    // VRGB16PG<T>A   Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>010000  |    2/5    | Packing of green channel
    // VRGB16PB<T>A   Qd, Qm     | VCX2A (vector) |   0   |  7'b<T>100000  |    2/5    | Packing of blue channel
    // VRGB16P<T>A    Qd, Qn, Qm | VCX3A (vector) |   0   |  4'b<T>000     |    3/5    | Packing of all channels
    // VRGB16MIX      Qd, Qn, Qm | VCX3A (vector) |   0   |  4'b0001       |    4/5    | Unpacking, multiplying and packing of all channels
*/

#if (__ARM_FEATURE_CDE_COPROC & (0x1 << ARM_2D_RGB565_ACI_LIB_COPRO_ID) ) == 0
#error "__ARM_FEATURE_CDE_COPROC does not have correct feature bits set"
#endif

#include <arm_mve.h>
#include <arm_cde.h>


#ifndef __HAS_ARM_2D_SIZE_TYPE__
typedef struct arm_2d_size_t {
    int16_t iWidth;
    int16_t iHeight;
} arm_2d_size_t;

#define __HAS_ARM_2D_SIZE_TYPE__        1
#endif

/*!
 * \brief blend rgb565 pixels with an user specified 7bit alpha.
 * 
 * \param In1Out the 8 pixels of the target rgb565 frame buffer, the vector is
 *        also used to store the blend result
 * \param In2 the 8 pixels of the source
 * \param vu7TransRatio the 7bit transparency ratio
 * \return uint16x8_t the blend result
 *
 * \note v:         using FPU register page
 *       bld:       blend
 *       a7:        7bit alpha (transparency ratio)
 *       q:         MVE extension
 *       rgb565:    the data type
 */
__STATIC_FORCEINLINE
uint16x8_t vblda7q_rgb565(  uint16x8_t In1Out, 
                            uint16x8_t In2, 
                            uint16x8_t vu7TransRatio)
{
    return __arm_vcx3qa(ARM_2D_RGB565_ACI_LIB_COPRO_ID, In1Out, In2, vu7TransRatio, 1);
}

/*!
 * \brief blend rgb565 pixels with an user specified 7bit alpha and a prediction mask
 * \param In1Out the 8 pixels of the target rgb565 frame buffer, the vector is
 *        also used to store the blend result
 * \param In2 the 8 pixels of the source
 * \param vu7TransRatio the 7bit transparency ratio
 * \param p the 16bit prediction mask
 * \return uint16x8_t the blend result
 *
 * \note v:         using FPU register page
 *       bld:       blend
 *       a7:        7bit alpha (transparency ratio)
 *       q:         MVE extension
 *       m:         using predition
 *       rgb565:    the data type
 */
__STATIC_FORCEINLINE
uint16x8_t vblda7q_m_rgb565(uint16x8_t In1Out, 
                        uint16x8_t In2, 
                        uint16x8_t vu7TransRatio,
                        mve_pred16_t p)
{
    return __arm_vcx3qa_m(  ARM_2D_RGB565_ACI_LIB_COPRO_ID, 
                            In1Out, 
                            In2, 
                            vu7TransRatio, 
                            1, 
                            p);
}


/*!
 * \brief initialize ACI extension
 */
extern
void aci_init(void);

/*!
 * \brief copy a rgb565 image to a target framebuffer with a specified alpha mask
 * 
 * \param phwSourceBase a pointer points to the source pixels
 * \param iSourceStride the source image stride
 * \param phwTargetBase a pointer points to the target framebuffer
 * \param iTargetStride the stride of the target framebuffer
 * \param pchTargetMaskBase a pointer points to the alpha mask 
 * \param iTargetMaskStride the alpha mask stride
 * \param ptCopySize the size of the copy operation
 */
extern
void __arm_2d_impl_rgb565_des_msk_copy_aci(uint16_t * __RESTRICT phwSourceBase,
                                           int16_t iSourceStride,
                                           uint16_t * __RESTRICT phwTargetBase,
                                           int16_t iTargetStride,
                                           uint8_t * __RESTRICT pchTargetMaskBase,
                                           int16_t iTargetMaskStride,
                                           arm_2d_size_t * __RESTRICT ptCopySize);

#endif                          /* __ARM_FEATURE_CDE  */
#endif                          /* __ARM_2D_RGB565_ACI_LIB_H__ */

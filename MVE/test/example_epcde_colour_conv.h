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
 *
 */

////////////////////////////////////////////////////////////////////////////////
//
// Cortex-M55 Execution Testbench Colour Conversion Demo
//
////////////////////////////////////////////////////////////////////////////////
//
// Defined Macros for ARM intrinsic:
//
// - Enumerate type of available tests
// - RGB16 pack
// - RGB16 unpack
// - RGB16 mix
//
#pragma once

#include <arm_cde.h>
#include "arm_mve.h"
#include <stdint.h>
#include "exectb_mcu.h"
#include "EXECTB_tests.h"

// RGB16 Pack

#define VRGB16_PACK_RED_ACC_BOT(acc, in)        __arm_vcx2qa(0, acc, in, 0b0001000)
#define VRGB16_PACK_RED_ACC_TOP(acc, in)        __arm_vcx2qa(0, acc, in, 0b1001000)

#define VRGB16_PACK_GREEN_ACC_BOT(acc, in)      __arm_vcx2qa(0, acc, in, 0b0010000)
#define VRGB16_PACK_GREEN_ACC_TOP(acc, in)      __arm_vcx2qa(0, acc, in, 0b1010000)

#define VRGB16_PACK_BLUE_ACC_BOT(acc, in)       __arm_vcx2qa(0, acc, in, 0b0100000)
#define VRGB16_PACK_BLUE_ACC_TOP(acc, in)       __arm_vcx2qa(0, acc, in, 0b1100000)

// RGB16 Unpack
#define VRGB16_UNPACK_RED_ACC_BOT(acc, in)      __arm_vcx2qa(0, acc, in, 0b0000001)
#define VRGB16_UNPACK_RED_ACC_TOP(acc, in)      __arm_vcx2qa(0, acc, in, 0b1000001)

#define VRGB16_UNPACK_GREEN_ACC_BOT(acc, in)    __arm_vcx2qa(0, acc, in, 0b0000010)
#define VRGB16_UNPACK_GREEN_ACC_TOP(acc, in)    __arm_vcx2qa(0, acc, in, 0b1000010)

#define VRGB16_UNPACK_BLUE_ACC_BOT(acc, in)     __arm_vcx2qa(0, acc, in, 0b0000100)
#define VRGB16_UNPACK_BLUE_ACC_TOP(acc, in)     __arm_vcx2qa(0, acc, in, 0b1000100)

// RGB16 Mix
#define VRGB16MIX(in1, in2, scal)                in1= __arm_vcx3qa(0, in1, in2, scal, 0b01)

enum TEST_NAME {
    rgb16_unpack,
    rgb16_pack,
    rgb16_mix
};
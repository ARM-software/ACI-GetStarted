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
// Cortex-M55 Execution Testbench Population Count Demo
//
////////////////////////////////////////////////////////////////////////////////
//
// Defined Macros for ARM intrinsic
//
#pragma once

#include <arm_cde.h>
#include <stdint.h>
#include <stdlib.h>
#include "exectb_mcu.h"
#include "EXECTB_tests.h"

#define POPCNT(in1) __arm_cx1a (0, in1, 0b0);
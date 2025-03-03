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
// This test checks CDE module of population count algorithm:
//
// Count the number of bit set in a sequence of a given length.
// Reference C and CDE accelerated versions
//
#include "example_cde_popcnt.h"

uint32_t value;
uint32_t exception_expected = 0;
uint32_t exception_occurred = 0;
uint32_t mapped;
uint32_t disabled;
uint32_t present;
uint32_t cdertlid_ok;
uint32_t cpif_ok;
uint32_t cde_mapping_ok;

void arm_pop_cnt_ref(uint8_t *out, const uint32_t *in1, uint32_t blockSize)
{
    const unsigned char __popcount_tab[] = {
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
};
    int32_t        blkCnt;
    blkCnt = blockSize;
    do {
        uint32_t packedIn;
        packedIn = *in1;
        in1 += 1;

        *out = __popcount_tab[(packedIn >>  0) & 0xff] +
               __popcount_tab[(packedIn >>  8) & 0xff] +
               __popcount_tab[(packedIn >> 16) & 0xff] +
               __popcount_tab[(packedIn >> 24) & 0xff];

        out++;

        blkCnt -= 1;
    }
    while (blkCnt > 0);
}

void arm_pop_cnt_cde(uint8_t *out, const uint32_t *in, uint32_t blockSize)
{
    int32_t blkCnt;

    blkCnt = blockSize;

    do {
        uint32_t packedIn = *in;
        uint32_t popcnt   =  POPCNT(packedIn);

        *out = popcnt;
        out += 1;
        in += 1;
        blkCnt -= 1;
    }
    while (blkCnt > 0);
}

int arm_pop_cnt(int length)
{
    int       errorCnt = 0;
    int       vectorSize = length;
    uint32_t A[vectorSize], *pA;
    uint8_t destRef [vectorSize + EXTRA_TAIL_CHK];
    uint8_t destCde [vectorSize + EXTRA_TAIL_CHK];
    pA = A;

    int dump = 0;

    srand(0xdeafbeef);
    for (int i = 0; i < vectorSize; i++)
        pA[i] = rand();

    startSection(1);
    arm_pop_cnt_ref(destRef, pA, vectorSize);
    stopSection(1);

    startSection(2);
    arm_pop_cnt_cde(destCde, pA, vectorSize);
    stopSection(2);

    check_for_error(destRef, destCde, vectorSize, 0);
    if (errorCnt || dump) {
        dump_buf(A, vectorSize, 8, "%04x");
        dump_buf(destRef, vectorSize, 8, "%02x");
        dump_buf(destCde, vectorSize, 8, "%02x");
    }
    return errorCnt;
}

int main(){
    // Call Execution Testbench Specific Initialisation
    EXECTB_Init();

    // Banner
    MSG(("Cortex-M55 - CDE Poplution Count Demo\n"));
    // Config Requirements:
    //  - CDERTLID           : 0, 255
    //  - CDEMAPPEDONCP[0]   : 0x1
    //  - CPSPRESENT         : 0x1F
    //  - CPNSPRESENT        : 0x1F
    //  - CPIF               : 1

    //  Check CDERTLID
    cdertlid_ok = (EXPECTED_CDERTLID == 0 || EXPECTED_CDERTLID == 255);

    // Check if CDE mapping is correct and nothing is disabled that shouldn't be
    mapped   = ((EXPECTED_CDEMAPPEDONCP7 & 1) << 7) | ((EXPECTED_CDEMAPPEDONCP6 & 1) << 6) |
               ((EXPECTED_CDEMAPPEDONCP5 & 1) << 5) | ((EXPECTED_CDEMAPPEDONCP4 & 1) << 4) |
               ((EXPECTED_CDEMAPPEDONCP3 & 1) << 3) | ((EXPECTED_CDEMAPPEDONCP2 & 1) << 2) |
               ((EXPECTED_CDEMAPPEDONCP1 & 1) << 1) | ((EXPECTED_CDEMAPPEDONCP0 & 1));
    disabled = mapped & ~EXPECTED_CFGNOCDECP;
    present  = disabled & EXPECTED_CPSPRESENT & EXPECTED_CPNSPRESENT;
    cde_mapping_ok = (present | 0xFFFFFFFE) == 0xFFFFFFFF;

    // Check CPIF
    cpif_ok = (EXPECTED_CPIF == 1);
    int errorCnt = 0;
    if (!(cdertlid_ok & cde_mapping_ok & cpif_ok)) {
        MSG(("Config:        \tValue\tOK?\n"));
        MSG(("\tCDERTLID     \t%x\t%s\n", EXPECTED_CDERTLID, cdertlid_ok? "Y" : "N"));
        MSG(("\tCDEMAPPEDONCP\t%x\t%s\n", present, cde_mapping_ok? "Y" : "N"));
        MSG(("\tCPIF         \t%x\t%s\n", EXPECTED_CPIF, cpif_ok? "Y" : "N"));
        MSG(("** TEST SKIPPED ** Config is not set up correctly for this test\n"));
        return TEST_PASS;
    }

    else {
        enableCde();
        errorCnt = arm_pop_cnt(ITER_NUM);
        if (errorCnt != 0){
            MSG(("Error occurred\n"));
            return errorCnt;
        }
        else {
            MSG(("Test passed\n"));
        }
        return TEST_PASS;
    }
}

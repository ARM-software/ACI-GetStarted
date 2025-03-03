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
// This test checks CDE module of Colour Conversion:
//
// RGB16 pack test
// RGB16 unpack test
// RGB16 mixed test
// CDE output and C reference function
//
#include "example_epcde_colour_conv.h"

uint32_t value;
uint32_t exception_expected = 0;
uint32_t exception_occurred = 0;
uint32_t mapped;
uint32_t disabled;
uint32_t present;
uint32_t cdertlid_ok;
uint32_t cpif_ok;
uint32_t cde_mapping_ok;
uint32_t mve_fpu_ok;
uint32_t bigend_ok;

void arm_pack_rgb16_ref(uint16_t * out,
                      const uint8_t * r, const uint8_t * g, const uint8_t * b, uint32_t blockSize)
//
// @brief         Pack red, green, blue to RGB16 streams
// @param[out]    out        points to the RGB16 output stream
// @param[in]     r          points to the 8-bit red stream
// @param[in]     g          points to the 8-bit green stream
// @param[in]     b          points to the 8-bit blue stream
// @param[in]     blockSize  number of pixels to process
// @return        none
//
{
    int32_t         blkCnt;
    uint16_t        maskB = (0x00f8);
    uint16_t        maskG = (0x00fc);

    //Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3))

    blkCnt = blockSize;
    do {
        *out = (*r >> 3) | ((*g & maskG) << 3) | ((*b & maskB) << 8);
        out++;
        r++;
        g++;
        b++;
        blkCnt -= 1;
    }
    while (blkCnt > 0);
}

void arm_unpack_rgb16_ref(const uint16_t * in,
                      uint8_t * r, uint8_t * g, uint8_t * b, uint32_t blockSize)
//
// @brief         Unpack a RGB16 stream to red, green, blue output stream
// @param[in]     in         points to the RGB16 input stream
// @param[out]    r          points to the 8-bit red stream
// @param[out]    g          points to the 8-bit green stream
// @param[out]    b          points to the 8-bit blue stream
// @param[in]     blockSize  number of pixels to process
// @return        none
//
{
    int32_t         blkCnt;
    uint16_t        maskR = 0x001f;
    uint16_t        maskG = 0x003f;
    uint16_t        packedIn = *in++;

    blkCnt = blockSize;
    do {
        uint16_t        R = packedIn & maskR;
        *r++ = R << 3;

        uint16_t        B = packedIn >> 11;
        *b++ = B << 3;

        uint16_t        G = ((packedIn >> 5) & maskG);
        *g++ = G << 2;

        packedIn = *in++;
        blkCnt -= 1;
    }
    while (blkCnt > 0);
}

void arm_unpack_merge_pack_rgb16_ref(uint16_t * out, const uint16_t * in1, const uint16_t * in2, uint8_t ratio, uint32_t blockSize)
//
//@brief         Unpack 2 RGB16 streams, mixed them together and Pack the output (RGB blending)
//@param[out]    out        points to the RGB16 output stream
//@param[in]     in1        points to the 1st RGB16 stream
//@param[in]     in2        points to the 2nd RGB16 stream
//@param[in]     ratio      Fixed point ratio in UQ0.8
//@param[in]     blockSize  number of pixels to process
//@return        none
//
{
    ratio >>= 1;
    int32_t         blkCnt;
    uint16_t        maskRunpk = 0x001f;
    uint16_t        maskGunpk = 0x003f;
    uint16_t        maskRpk = 0x00f8;
    uint16_t        maskGpk = 0x00fc;
    uint8_t         r1, g1, b1;
    uint8_t         r2, g2, b2;
    blkCnt = blockSize;
    do {
        uint16_t        packedIn;
        uint8_t         R, B, G;
        int32_t         tmp;

        // unpack 1st stream
        packedIn = *in1++;

        R = packedIn & maskRunpk;
        r1 = R << 3;

        B = packedIn >> 11;
        b1 = B << 3;

        G = ((packedIn >> 5) & maskGunpk);
        g1 = G << 2;


        // unpack 2nd stream
        packedIn = *in2++;
        R = packedIn & maskRunpk;
        r2 = R << 3;

        B = packedIn >> 11;
        b2 = B << 3;

        G = ((packedIn >> 5) & maskGunpk);
        g2 = G << 2;


        // merge
        tmp = (r1 * ratio) + (r2 * (127 - ratio));
        r1 = (uint8_t) (tmp >> 7);

        tmp = (g1 * ratio) + (g2 * (127 - ratio));
        g1 = (uint8_t) (tmp >> 7);

        tmp = (b1 * ratio) + (b2 * (127 - ratio));
        b1 = (uint8_t) (tmp >> 7);

        // pack merged stream
        *out = (r1 >> 3) | ((g1 & maskGpk) << 3) | ((b1 & maskRpk) << 8);
        out++;

        blkCnt -= 1;
    }
    while (blkCnt > 0);
}

void arm_pack_rgb16_cde_mve(uint16_t * out,
                            const uint8_t * r, const uint8_t * g, const uint8_t * b,
                            uint32_t blockSize)
{
    int32_t         blkCnt;
    uint16x8x2_t    dst;

    blkCnt = blockSize;
    do {
        uint8x16_t      vecR = vldrbq_u8(r);
        uint8x16_t      vecG = vldrbq_u8(g);
        uint8x16_t      vecB = vldrbq_u8(b);

        dst.val[0] = VRGB16_PACK_RED_ACC_BOT(dst.val[0], vecR);
        dst.val[0] = VRGB16_PACK_GREEN_ACC_BOT(dst.val[0], vecG);
        dst.val[0] = VRGB16_PACK_BLUE_ACC_BOT(dst.val[0], vecB);

        dst.val[1] = VRGB16_PACK_RED_ACC_TOP(dst.val[1], vecR);
        dst.val[1] = VRGB16_PACK_GREEN_ACC_TOP(dst.val[1], vecG);
        dst.val[1] = VRGB16_PACK_BLUE_ACC_TOP(dst.val[1], vecB);

        vst2q(out, dst);

        r += 16;
        g += 16;
        b += 16;
        out += 16;
        blkCnt -= 16;
    }
    while (blkCnt > 0);
}

void arm_unpack_rgb16_cde_mve(const uint16_t * in,
                              uint8_t * r, uint8_t * g, uint8_t * b, uint32_t blockSize)
{
    int32_t         blkCnt;
    const uint16_t *pIn;
    uint8x16_t      dst = vdupq_n_u8(0);

    blkCnt = blockSize;
    pIn = in;
    do {
        uint16x8x2_t    vecIn = vld2q(pIn);
        pIn += 16;

        dst = VRGB16_UNPACK_RED_ACC_BOT(dst, vecIn.val[0]);
        dst = VRGB16_UNPACK_RED_ACC_TOP(dst, vecIn.val[1]);
        vstrbq_u8(r, dst);

        dst = VRGB16_UNPACK_GREEN_ACC_BOT(dst, vecIn.val[0]);
        dst = VRGB16_UNPACK_GREEN_ACC_TOP(dst, vecIn.val[1]);
        vstrbq_u8(g, dst);

        dst = VRGB16_UNPACK_BLUE_ACC_BOT(dst, vecIn.val[0]);
        dst = VRGB16_UNPACK_BLUE_ACC_TOP(dst, vecIn.val[1]);
        vstrbq_u8(b, dst);

        r += 16;
        g += 16;
        b += 16;
        blkCnt -= 16;
    }
    while (blkCnt > 0);
}

void arm_mix_rgb16_cde_mve(uint16_t * out, const uint16_t * in1, const uint16_t * in2, uint8_t ratio, uint32_t blockSize)
{
    int32_t         blkCnt;
    uint16_t        ratio1 = ratio >> 1;
    int16x8_t       vecRatio = vdupq_n_s16(ratio1);

    blkCnt = blockSize;

    do {
        uint16x8_t      vecIn1 = vld1q(in1);
        uint16x8_t      vecIn2 = vld1q(in2);

        VRGB16MIX(vecIn1, vecIn2, vecRatio);

        vst1q(out, vecIn1);
        out += 8;
        in1 += 8;
        in2 += 8;
        blkCnt -= 8;
    }
    while (blkCnt > 0);
}

int arm_vec_rgb16(int length, enum TEST_NAME select)
//
// @attention vectorSize needs larger than 16 due to CDE vector processing
//
{
    int             errorCnt = 0;
    int             vectorSize = length;
    int             dump = 0;

    switch (select)
    {
        case rgb16_pack:
        {
            uint16_t destRef[vectorSize + EXTRA_TAIL_CHK];
            uint16_t destCde[vectorSize + EXTRA_TAIL_CHK];
            uint8_t  r[vectorSize], g[vectorSize], b[vectorSize];

            for (int i = 0; i < vectorSize; i++) {
                r[i] = i;
                g[i] = 2 * i;
                b[i] = 3 * i;
            }

            startSection(1);
            arm_pack_rgb16_ref(destRef, r, g, b, vectorSize);
            stopSection(1);

            startSection(2);
            arm_pack_rgb16_cde_mve(destCde, r, g, b, vectorSize);
            stopSection(2);

            check_for_error(destRef, destCde, vectorSize, 0);

            if (errorCnt || dump) {
                dump_buf(destRef, vectorSize, 8, "%04x");
                dump_buf(destCde, vectorSize, 8, "%04x");
                dump_buf(r, vectorSize, 8, "%02x");
                dump_buf(g, vectorSize, 8, "%02x");
                dump_buf(b, vectorSize, 8, "%02x");
            }
        }
            break;
        case rgb16_unpack:
        {
            uint16_t A[vectorSize], *pA;
            pA = A;
            uint8_t  rRef[vectorSize + EXTRA_TAIL_CHK],
                gRef[vectorSize + EXTRA_TAIL_CHK], bRef[vectorSize + EXTRA_TAIL_CHK];
            uint8_t  rCde[vectorSize + EXTRA_TAIL_CHK],
                gCde[vectorSize + EXTRA_TAIL_CHK], bCde[vectorSize + EXTRA_TAIL_CHK];

            for (int i = 0; i < vectorSize; i++) {
                uint16_t        Rgb565 =
                    ((i & (0x1f)) << 11) | ((i & (0x3f)) << 5) | (i & (0x1f));
                pA[i] = Rgb565;
            }

            startSection(1);
            arm_unpack_rgb16_ref(pA, rRef, gRef, bRef, vectorSize);
            stopSection(1);

            startSection(2);
            arm_unpack_rgb16_cde_mve(pA, rCde, gCde, bCde, vectorSize);
            stopSection(2);

            check_for_error(rRef, rCde, vectorSize, 0);
            check_for_error(gRef, gCde, vectorSize, 0);
            check_for_error(bRef, bCde, vectorSize, 0);

            if (errorCnt || dump) {
                dump_buf(A, vectorSize, 8, "%04x");
                dump_buf(rRef, vectorSize, 8, "%02x");
                dump_buf(rCde, vectorSize, 8, "%02x");
                dump_buf(gRef, vectorSize, 8, "%02x");
                dump_buf(gCde, vectorSize, 8, "%02x");
                dump_buf(bRef, vectorSize, 8, "%02x");
                dump_buf(bCde, vectorSize, 8, "%02x");
            }
        }
            break;
        case rgb16_mix:
        {
            uint16_t A[vectorSize], *pA;
            uint16_t B[vectorSize], *pB;
            pA = A;
            pB = B;
            uint16_t        destRef[vectorSize + EXTRA_TAIL_CHK];
            uint16_t        destCde[vectorSize + EXTRA_TAIL_CHK];

            for (int i = 0; i < vectorSize; i++) {
                uint16_t        Rgb565;
                Rgb565 =
                    ((i & (0x1f)) << 11) | ((2 * i & (0x3f)) << 5) | (3 * i & (0x1f));
                pA[i] = Rgb565;

                Rgb565 =
                    (((i / 2) & (0x1f)) << 11) | (((i / 2) & (0x3f)) << 5) | ((i /
                                                                               2) &
                                                                              (0x1f));
                pB[i] = Rgb565;
            }

            startSection(1);
            arm_unpack_merge_pack_rgb16_ref(destRef, pA, pB, 128, vectorSize);
            stopSection(1);

            startSection(2);
            arm_mix_rgb16_cde_mve(destCde, pA, pB, 128, vectorSize);
            stopSection(2);

            check_for_error(destRef, destCde, vectorSize, 0);

            if (errorCnt || dump) {
                dump_buf(A, vectorSize, 8, "%04x");
                dump_buf(B, vectorSize, 8, "%04x");
                dump_buf(destRef, vectorSize, 8, "%02x");
                dump_buf(destCde, vectorSize, 8, "%02x");
            }
        }
            break;
        default:
            printf("Did not have corresponding test\n");
            break;
    }
    return errorCnt;
}

int main(){
    // Call Execution Testbench Specific Initialisation
    EXECTB_Init();

    // Banner
    MSG(("Cortex-M55 -  CDE RGB Mix Demo\n"));
    // Config Requirements:
    //  - CDERTLID           : 2, 3, 4, 5, 255
    //  - CDEMAPPEDONCP[0]   : 0x1
    //  - CPSPRESENT         : 0x1F
    //  - CPNSPRESENT        : 0x1F
    //  - CPIF               : 1

    //  Check CDERTLID
    cdertlid_ok = (EXPECTED_CDERTLID == 5 || EXPECTED_CDERTLID == 255);

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

    if (EXPECTED_MVE <= EXPECTED_CFGMVE) {
        mve_fpu_ok = EXPECTED_MVE;
    } else {
        mve_fpu_ok = EXPECTED_CFGMVE;
    }
    mve_fpu_ok |= (EXPECTED_FPU == 1) && (EXPECTED_CFGFPU == 1);

    // Check BIG ENDIAN
    bigend_ok = (EXPECTED_BIGEND == 0);

    int errorCnt = 0;
    if (!(cdertlid_ok & cde_mapping_ok & cpif_ok & mve_fpu_ok & bigend_ok)) {
        MSG(("Config:        \tValue\tOK?\n"));
        MSG(("\tCDERTLID     \t%x\t%s\n", EXPECTED_CDERTLID, cdertlid_ok? "Y" : "N"));
        MSG(("\tCDEMAPPEDONCP\t%x\t%s\n", present, cde_mapping_ok? "Y" : "N"));
        MSG(("\tCPIF         \t%x\t%s\n", EXPECTED_CPIF, cpif_ok? "Y" : "N"));
        MSG(("\tMVE | FPU    \t%x\t%s\n", mve_fpu_ok, mve_fpu_ok? "Y" : "N"));
        MSG(("\tBIGEND       \t%x\t%s\n", EXPECTED_BIGEND, bigend_ok? "Y" : "N"));
        MSG(("** TEST SKIPPED ** Config is not set up correctly for this test\n"));
        return TEST_PASS;
    }

    else {
        enableCde();
        enum TEST_NAME test[] = {rgb16_pack, rgb16_unpack, rgb16_mix};

        for (int i = 0; i < sizeof(test) / sizeof(test[0]); i++)
        {
            errorCnt += arm_vec_rgb16(ITER_NUM, test[i]);
        }

        if (errorCnt != 0){
            MSG(("Error occurred\n"));
            return TEST_FAIL;
        }
        else {
            MSG(("Test passed\n"));
        }
        return TEST_PASS;
    }
}

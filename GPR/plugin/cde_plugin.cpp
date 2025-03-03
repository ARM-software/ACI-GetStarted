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

/* ----------------------------------------------------------------------
 * Project:      ACI GPR Library
 * Title:        cde_plugin.h
 * Description:  ACI GPR example library
 *
 * $Date:        30. Dec 2024
 * $Revision:    V1.0.0
 *
 * -------------------------------------------------------------------- */


#include "cde_plugin.h"
#include <valarray>
#include <cstdio>

static
int popcount(uint32_t x) 
{
    int n = 0;
    for (int i = 0; i < 32; ++i) {
        n += (x >> i) & 1;
    }

    return n;
}

ACI_Status aci_fvp::exec_cx1(const ACICX1DecodeInfo * decode_info,
                                           uint32_t rd_val, uint32_t * result)
{
    /*! \note We valid the Pn, `imm`, and the `accumulate` in this implementation
     *        How to interpret `accumulate` and `imm` is IMPLEMENTATION DEFINED. For example,
     *        use can use imm as a parameter or a way to address different instructions
     *        or instruction variants.
     */
    if (!isCDECoprocessorValid(decode_info->coproc))
        return ACI_STATUS_NOT_IMPLEMENTED;

    /* check decode info for the population-counter logic */
    if (decode_info->accumulate && 0 == decode_info->imm) {

        *result = popcount(rd_val);
    
        return ACI_STATUS_OK;
    }

    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_cx1_d(const ACICX1DecodeInfo * decode_info,
                                             uint64_t rfd_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_cx2(const ACICX2DecodeInfo * decode_info,
                                           uint32_t rd_val, uint32_t rn_val, uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_cx2_d(const ACICX2DecodeInfo * decode_info,
                                             uint64_t rfd_val, uint64_t rn_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_cx3(const ACICX3DecodeInfo * decode_info,
                                           uint32_t rd_val,
                                           uint32_t rn_val, uint32_t rm_val, uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_cx3_d(const ACICX3DecodeInfo * decode_info,
                                             uint64_t rfd_val,
                                             uint64_t rn_val, uint64_t rm_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_vcx1_s(const ACIVCX1DecodeInfo * decode_info,
                                              uint32_t sd_val, uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_vcx1_d(const ACIVCX1DecodeInfo * decode_info,
                                              uint64_t dd_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}


ACI_Status aci_fvp::exec_vcx1_beatwise(const ACIVCX1DecodeInfo * decode_info,
                                                     uint32_t d_val,
                                                     uint8_t beat, uint8_t elmt_mask,
                                                     uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_vcx2_s(const ACIVCX2DecodeInfo * decode_info,
                                              uint32_t sd_val, uint32_t sm_val, uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_vcx2_d(const ACIVCX2DecodeInfo * decode_info,
                                              uint64_t dd_val, uint64_t dm_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}




ACI_Status aci_fvp::exec_vcx2_beatwise(const ACIVCX2DecodeInfo * decode_info,
                                                     uint32_t d_val,
                                                     uint32_t m_val,
                                                     uint8_t beat, uint8_t elmt_mask,
                                                     uint32_t * result)
{

    return ACI_STATUS_NOT_IMPLEMENTED;

}



ACI_Status aci_fvp::exec_vcx3_s(const ACIVCX3DecodeInfo * decode_info,
                                              uint32_t sd_val,
                                              uint32_t sn_val, uint32_t sm_val, uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

ACI_Status aci_fvp::exec_vcx3_d(const ACIVCX3DecodeInfo * decode_info,
                                              uint64_t dd_val,
                                              uint64_t dn_val, uint64_t dm_val, uint64_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}






ACI_Status aci_fvp::exec_vcx3_beatwise(const ACIVCX3DecodeInfo * decode_info,
                                                     uint32_t d_val,
                                                     uint32_t n_val,
                                                     uint32_t m_val,
                                                     uint8_t beat, uint8_t elmt_mask,
                                                     uint32_t * result)
{
    return ACI_STATUS_NOT_IMPLEMENTED;
}

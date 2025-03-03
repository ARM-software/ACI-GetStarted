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
// Cortex-M55 Execution Testbench Configuration File
//
////////////////////////////////////////////////////////////////////////////////
//
//  Processor configuration options.
//  These must match the expected hardware configuration of the processor.
//
//  - EXPECTED_FPU           : Expected Floating Point Unit (0,1)
//  - EXPECTED_MVE           : Expected M-profile Vector Extension (0,1,2)
//  - EXPECTED_SECEXT        : Expected Security Extension (0,1)
//  - EXPECTED_CPIF          : Expected External Coprocessor Interface (0,1)
//  - EXPECTED_MPU_NS        : Expected MPU non-secure regions (0,4,8,12,16)
//  - EXPECTED_MPU_S         : Expected MPU secure regions (0,4,8,12,16)
//  - EXPECTED_SAU           : Expected number of SAU regions (0,4,8)
//  - EXPECTED_ICACHESZ      : Expected instruction caches size KB(0,4,8,16,32,64)
//  - EXPECTED_DCACHESZ      : Expected data caches size KB(0,4,8,16,32,64)
//  - EXPECTED_IDCACHEID     : Expected caches id(0-255)
//  - EXPECTED_ECC           : Expected L1 and TCM Error Detection and Correction (0,1)
//  - EXPECTED_NUMIRQ        : Expected number of IRQ lines (0-240)
//  - EXPECTED_IRQLVL        : Expected number of IRQ levels (3-8)
//  - EXPECTED_IRQTIER       : Expected split between high and low latency interrupts (NUMIRQ length vector)
//  - EXPECTED_IRQDIS        : Expected disable individual interrupts (NUMIRQ length vector)
//  - EXPECTED_DBGLVL        : Expected debug level (0,1,2)
//  - EXPECTED_ITM           : Expected CoreSight ITM trace config (0,1)
//  - EXPECTED_ETM           : Expected CoreSight ETM trace config (0,1)
//  - EXPECTED_IWIC          : Expected Internal Wake-up interrupt controller support (0,1)
//  - EXPECTED_WICLINES      : Expected number of supported WIC lines <3-NUMIRQ+3>
//  - EXPECTED_CTI           : Expected Cross Trigger Interface Support (0,1)
//  - EXPECTED_BUSPROT       : Expected Bus Protection Support (0,1)
//  - EXPECTED_LOCKSTEP      : Expected Dual Core Lockstep Support (0,1)
//  - EXPECTED_ITGU          : Expected ITCM security gate (0,1)
//  - EXPECTED_ITGUBLKSZ     : Expected ITCM gate unit block size (0-15)
//  - EXPECTED_ITGUMAXBLKS   : Expected Max number of ITCM gate unit blocks (1-9)
//  - EXPECTED_DTGU          : Expected DTCM security gate (0,1)
//  - EXPECTED_DTGUBLKSZ     : Expected ITCM gate unit block size (0-15)
//  - EXPECTED_DTGUMAXBLKS   : Expected Max number of DTCM gate unit blocks (1-9)
//  - EXPECTED_PMC           : Expected Programmable MBIST Controller (0,1)
//  - EXPECTED_PMCPROGSIZE   : Expected Number of PMC registers (2-32)
//  - EXPECTED_CTI           : Expected Cross Trigger Interface Support (0,1)
//  - EXPECTED_RAR           : Expected Reset All Registers (0,1)
//  - EXPECTED_DCLS          : Expected Dual Core Lockstep (0,1)
//  - EXPECTED_FLOPPARITY    : Expected Flop Parity (0,1)
//
//  Processor Static Configuration Tie-offs Reset Config
//  - EXPECTED_CFGITCMSZ     : Expected ITCM gate unit block size (0-15)
//  - EXPECTED_CFGDTCMSZ     : Expected DTCM gate unit block size (0-15)
//  - EXPECTED_CFGPAHBSZ     : Expected Size of P-AHB memory region (0-4)
//  - EXPECTED_CFGMEMALIAS   : Expected Memory Address Alias bit (0,1,2,4,8,16)
//  - EXPECTED_BIGEND        : Expected Endianness 1=BE 0=LE(0-1)
//  - EXPECTED_CFGFPU        : Expected Floating Point Unit Enable (0-1)
//  - EXPECTED_CFGMVE        : Expected MVE Enable(0-2)
//  - EXPECTED_MPUNSDISABLE  : Expected MPU non-secure disable (0-1)
//  - EXPECTED_MPUSDISABLE   : Expected MPU secure disable (0-1)
//  - EXPECTED_SAUDISABLE    : Expected SAU disable (0-1)
//  - EXPECTED_CFGSSTCALIB   : Expected value of SSTCALIB[25:0] <0x0-0x3FFFFFF>
//  - EXPECTED_CFGNSSTCALIB  : Expected value of NSSTCALIB[25:0] <0x0-0x3FFFFFF>
//
//  Processor Reset Configuration
//  - EXPECTED_INITSVTOR     : Expected secure initialisation vector
//  - EXPECTED_INITNSVTOR    : Expected non-secure initialisation vector
//  - EXPECTED_INITTCMEN     : Expected TCM initialisation out of reset (3-0)
//  - EXPECTED_INITPAHBEN    : Expected P-AHB enable initialisation out of reset
//  - EXPECTED_INITECCEN     : Expected TCM and L1 cache ECC enable out of reset
//  - EXPECTED_INITL1RSTDIS  : Expected Disable L1 cache invalidation out of reset
//
//  MCU level configuration
//  - EXPECTED_MCUROMADDR    : Expected MCU ROM Address
//  - EXPECTED_EWIC          : Expected External Wakeup controller (0,1)
//  - EXPECTED_ETB           : Expected Embedded Trace Buffer (0,1)
//  - EXPECTED_TREVISION     : Expected Revision number (0-0xF)
//  - EXPECTED_PARTNO        : Expected Part number (0-0xFFF)
//  - EXPECTED_TDESIGNER     : Expected Designer code (0-0x7FF)
//  - EXPECTED_CUST_JEPID    : Expected JEDEC JEP-106 identity code (0-0x7F)
//  - EXPECTED_CUST_JEPCONT  : Expected JEDEC JEP-106 continuation code (0-0xF)
//  - EXPECTED_DPSEL         : Expected Debug Port Select (0,1,2)
//  - EXPECTED_SBISTC        : Expected SBIST Controller(0,1)
//  - EXPECTED_TPIUM         : Expected select TPIU/TPIUM (0,1)
//  - EXPECTED_FF_SYNC_DEPTH : Expected TPIUM Synchronizer Depth (2,3)
//  - EXPECTED_TRACEPORT_DATA_WIDTH : Expected TPIU tracedata width (1, 2, 4, 8, 12, 16)

//
//  - EXPECTED_CUST_REV      : Expected Revision number (0-0xF)
//  - EXPECTED_CUST_REVAND   : Expected Manufacturer Revision number (0-0xF)
//
//  Other configuration
//  - EXPECTED_ECOREVNUM     : Expected Engineering Change Order and Revision number (0-0xF_FFFFFFFF)
//  - EXPECTED_RETENTION     : Expected retention support (0-1)
//
////////////////////////////////////////////////////////////////////////////////

//
// <h> Processor configuration options
// <o.0> EXPECTED_FPU: Expected Floating Point Unit
#define EXPECTED_FPU            1

// <o.0> EXPECTED_MVE: Expected M-profile Vector Extension
#define EXPECTED_MVE            1

// <o.0> EXPECTED_SECEXT: Expected Security Extension
#define EXPECTED_SECEXT         1

// <o.0> EXPECTED_CPIF: Expected External Coprocessor Interface Unit
#define EXPECTED_CPIF           1

// <o> EXPECTED_MPU_NS: Expected MPU non-secure regions <0=> Absent <4=> 4 Regions <8=> 8 Regions <12=> 12 Regions <16=> 16 Regions
#define EXPECTED_MPU_NS         8

// <o> EXPECTED_MPU_S: Expected MPU secure regions <0=> Absent <4=> 4 Regions <8=> 8 Regions <12=> 12 Regions <16=> 16 Regions
#define EXPECTED_MPU_S          8

// <o> EXPECTED_SAU: Expected number SAU regions <0=> Absent <4=> 4 Regions <8=> 8 Regions
#define EXPECTED_SAU            4

// <o> EXPECTED_ICACHESZ: Expected instruction caches size <0=> Absent <4=> 4KB <8=> 8KB <16=> 16KB <32=> 32KB <64=> 64KB
#define EXPECTED_ICACHESZ       32

// <o> EXPECTED_DCACHESZ: Expected data caches size <0=> Absent <4=> 4KB <8=> 8KB <16=> 16KB <32=> 32KB <64=> 64KB
#define EXPECTED_DCACHESZ       32

// <o> EXPECTED_IDCACHEID: Expected number of IRQ lines <0-255>
#define EXPECTED_IDCACHEID      0

// <o> EXPECTED_ECC: Expected L1 and TCM Error Detection and Correction
#define EXPECTED_ECC            0

// <o> EXPECTED_NUMIRQ: Expected number of IRQ lines <0-480>
#define EXPECTED_NUMIRQ         32

// <o> EXPECTED_IRQLVL: Expected number of exception levels <3-8>
#define EXPECTED_IRQLVL         3

// <h> EXPECTED_IRQTIER: Expected split between high and low latency interrupts <NUMIRQ length vector>
// <o0 >IRQTIER for IRQ number 479 - 448
// <o1 >IRQTIER for IRQ number 447 - 416
// <o1 >IRQTIER for IRQ number 415 - 384
// <o1 >IRQTIER for IRQ number 383 - 352
// <o2 >IRQTIER for IRQ number 351 - 320
// <o3 >IRQTIER for IRQ number 319 - 288
// <o4 >IRQTIER for IRQ number 287 - 256
// <o5 >IRQTIER for IRQ number 255 - 224
// <o6 >IRQTIER for IRQ number 223 - 192
// <o7 >IRQTIER for IRQ number 191 - 160
// <o8 >IRQTIER for IRQ number 159 - 128
// <o9 >IRQTIER for IRQ number 127 - 96
// <o10>IRQTIER for IRQ number 95 - 64
// <o11>IRQTIER for IRQ number 63 - 32
// <o12>IRQTIER for IRQ number 31 - 0
#define EXPECTED_IRQTIER_479_448 0x00000000
#define EXPECTED_IRQTIER_447_416 0x00000000
#define EXPECTED_IRQTIER_415_384 0x00000000
#define EXPECTED_IRQTIER_383_352 0x00000000
#define EXPECTED_IRQTIER_351_320 0x00000000
#define EXPECTED_IRQTIER_319_288 0x00000000
#define EXPECTED_IRQTIER_287_256 0x00000000
#define EXPECTED_IRQTIER_255_224 0x00000000
#define EXPECTED_IRQTIER_223_192 0x00000000
#define EXPECTED_IRQTIER_191_160 0x00000000
#define EXPECTED_IRQTIER_159_128 0x00000000
#define EXPECTED_IRQTIER_127_96  0x00000000
#define EXPECTED_IRQTIER_95_64   0x00000000
#define EXPECTED_IRQTIER_63_32   0x00000000
#define EXPECTED_IRQTIER_31_0    0x00000000
// </h>

// <h> EXPECTED_IRQDIS Expected disable individual interrupts <NUMIRQ length vector>
// <o0 >IRQDIS for IRQ number 479 - 448
// <o1 >IRQDIS for IRQ number 447 - 416
// <o1 >IRQDIS for IRQ number 415 - 384
// <o1 >IRQDIS for IRQ number 383 - 352
// <o2 >IRQDIS for IRQ number 351 - 320
// <o3 >IRQDIS for IRQ number 319 - 288
// <o4 >IRQDIS for IRQ number 287 - 256
// <o5 >IRQDIS for IRQ number 255 - 224
// <o6 >IRQDIS for IRQ number 223 - 192
// <o7 >IRQDIS for IRQ number 191 - 160
// <o8 >IRQDIS for IRQ number 159 - 128
// <o9 >IRQDIS for IRQ number 127 - 96
// <o10>IRQDIS for IRQ number 95 - 64
// <o11>IRQDIS for IRQ number 63 - 32
// <o12>IRQDIS for IRQ number 31 - 0
#define EXPECTED_IRQDIS_479_448   0x00000000
#define EXPECTED_IRQDIS_447_416   0x00000000
#define EXPECTED_IRQDIS_415_384   0x00000000
#define EXPECTED_IRQDIS_383_352   0x00000000
#define EXPECTED_IRQDIS_351_320   0x00000000
#define EXPECTED_IRQDIS_319_288   0x00000000
#define EXPECTED_IRQDIS_287_256   0x00000000
#define EXPECTED_IRQDIS_255_224   0x00000000
#define EXPECTED_IRQDIS_223_192   0x00000000
#define EXPECTED_IRQDIS_191_160   0x00000000
#define EXPECTED_IRQDIS_159_128   0x00000000
#define EXPECTED_IRQDIS_127_96    0x00000000
#define EXPECTED_IRQDIS_95_64     0x00000000
#define EXPECTED_IRQDIS_63_32     0x00000000
#define EXPECTED_IRQDIS_31_0      0x00000000
// </h>

// <o> EXPECTED_DBGLVL: Expected debug level
//                      <0=> No halting debug or memory access
//                      <1=> 2 WPT and 4 BKPT
//                      <2=> 4 WPT and 8 BKPT
#define EXPECTED_DBGLVL         2

// <o.0> EXPECTED_ITM: Expected CoreSight ITM config
#define EXPECTED_ITM            1

// <o.0> EXPECTED_ETM: Expected CoreSight ETM config
#define EXPECTED_ETM            0

// <o.0> EXPECTED_IWIC: Expected Wake-up interrupt controller support <0,1>
#define EXPECTED_IWIC           1

// <o> EXPECTED_WICLINES : Expected number of supported WIC lines <4-483>
#define EXPECTED_WICLINES       35

// <o> EXPECTED_BUSPROT : Expected Bus Protection Support <0,1>
#define EXPECTED_BUSPROT        0

// <o> EXPECTED_LOCKSTEP : Expected Dual Core Lockstep Support <0,1>
#define EXPECTED_LOCKSTEP       0

// <o> EXPECTED_ITGU : Expected ITCM security gate <0,1>
#define EXPECTED_ITGU           0

// <o> EXPECTED_ITGUBLKSZ : Expected ITCM gate unit block size <0-15>
#define EXPECTED_ITGUBLKSZ      6

// <o> EXPECTED_ITGUMAXBLKS : Expected Max number of ITCM gate unit blocks <1-9>
#define EXPECTED_ITGUMAXBLKS    1

// <o> EXPECTED_DTGU : Expected DTCM security gate <0,1>
#define EXPECTED_DTGU           0

// <o> EXPECTED_DTGUBLKSZ : Expected DTCM gate unit block size <0-15>
#define EXPECTED_DTGUBLKSZ      6

// <o> EXPECTED_DTGUMAXBLKS : Expected Max number of DTCM gate unit blocks <1-9>
#define EXPECTED_DTGUMAXBLKS    1

// <o> EXPECTED_PMC : Expected Programmable MBIST Controller <0,1>
#define EXPECTED_PMC            0

// <o> EXPECTED_PMCPROGSIZE : Expected Number of PMC registers <2-32>
#define EXPECTED_PMCPROGSIZE    14

// <o> EXPECTED_CTI : Expected Cross Trigger Interface Support <0,1>
#define EXPECTED_CTI            1

// <o> EXPECTED_RAR : Expected Reset All Registers <0,1>
#define EXPECTED_RAR            0

// <o> EXPECTED_DCLS : Expected Dual Core Lockstep <0,1>
#define EXPECTED_DCLS           0

// <o> EXPECTED_FLOPPARITY : Expected Flop Parity <0,1>
#define EXPECTED_FLOPPARITY     0

// <o> EXPECTED_CDEMAPPEDONCP0 : Expected CDE Mapping on coprocessor 0 <0,1>
#define EXPECTED_CDEMAPPEDONCP0 1
// <o> EXPECTED_CDEMAPPEDONCP1 : Expected CDE Mapping on coprocessor 1 <0,1>
#define EXPECTED_CDEMAPPEDONCP1 1
// <o> EXPECTED_CDEMAPPEDONCP2 : Expected CDE Mapping on coprocessor 2 <0,1>
#define EXPECTED_CDEMAPPEDONCP2 0
// <o> EXPECTED_CDEMAPPEDONCP3 : Expected CDE Mapping on coprocessor 3 <0,1>
#define EXPECTED_CDEMAPPEDONCP3 0
// <o> EXPECTED_CDEMAPPEDONCP4 : Expected CDE Mapping on coprocessor 4 <0,1>
#define EXPECTED_CDEMAPPEDONCP4 0
// <o> EXPECTED_CDEMAPPEDONCP5 : Expected CDE Mapping on coprocessor 5 <0,1>
#define EXPECTED_CDEMAPPEDONCP5 0
// <o> EXPECTED_CDEMAPPEDONCP6 : Expected CDE Mapping on coprocessor 6 <0,1>
#define EXPECTED_CDEMAPPEDONCP6 0
// <o> EXPECTED_CDEMAPPEDONCP7 : Expected CDE Mapping on coprocessor 7 <0,1>
#define EXPECTED_CDEMAPPEDONCP7 0

// <o> EXPECTED_CDERTLID : Expected Custom Datapeth Extension (CDE) RTL ID <8 bit vector>
#define EXPECTED_CDERTLID       0

// </h>

// <h> Processor Static Configuration Tie-offs Reset Config
// <o> EXPECTED_CFGITCMSZ: Expected ITCM gate unit block size <0,3-15>
#define EXPECTED_CFGITCMSZ      11

// <o> EXPECTED_CFGDTCMSZ: Expected DTCM gate unit block size <0,3-15>
#define EXPECTED_CFGDTCMSZ      11

// <o> EXPECTED_CFGPAHBSZ: Expected Size of P-AHB memory region <0-4>
#define EXPECTED_CFGPAHBSZ      4

// <o> EXPECTED_CFGMEMALIAS: Expected Memory Address Alias bit <0,1,2,4,8,16>
#define EXPECTED_CFGMEMALIAS    0

// <o> EXPECTED_BIGEND: Expected Endianness <0=> Little Endian <1=> Big Endian
#define EXPECTED_BIGEND         0

// <o.0> EXPECTED_CFGFPU: Expected Floating Point Unit Enable <0,1>
#define EXPECTED_CFGFPU         1

// <o.0> EXPECTED_CFGMVE: Expected MVE Enable <0-2>
#define EXPECTED_CFGMVE         2

// <o> EXPECTED_MPUNSDISABLE: Expected value of MPUNSDISABLE <0,1>
#define EXPECTED_MPUNSDISABLE   0

// <o> EXPECTED_MPUSDISABLE: Expected value of MPUSDISABLE <0-1>
#define EXPECTED_MPUSDISABLE    0

// <o> EXPECTED_SAUDISABLE: Expected value of SAUDISABLE <0,1>
#define EXPECTED_SAUDISABLE     0

// <o> EXPECTED_CFGNOCDECP: Expected value of CFGNOCDECP <0x0-0xFF>
#define EXPECTED_CFGNOCDECP     0x00

// <o> EXPECTED_CFGSSTCALIB: Expected value of SSTCALIB[25:0] Secure Systick calibration <0x0-0x3FFFFFF>
#define EXPECTED_CFGSSTCALIB    0x107A11F

// <o> EXPECTED_CFGNSSTCALIB: Expected value of NSSTCALIB[25:0] Non-secure Systick calibration <0x0-0x3FFFFFF>
#define EXPECTED_CFGNSSTCALIB   0x107A11F

// <o> EXPECTED_CPSPRESENT: Expected value of CPSPRESENT <0x0-0xFF>
#define EXPECTED_CPSPRESENT     0x11

// <o> EXPECTED_CPNSPRESENT: Expected value of CPNSPRESENT <0x0-0xFF>
#define EXPECTED_CPNSPRESENT    0x11
// </h>

// <h> Processor Reset Configuration
// <o> EXPECTED_INITSVTOR:  Expected default secure initialisation vector
#define EXPECTED_INITSVTOR      0x0000000

// <o> EXPECTED_INITNSVTOR:  Expected default non-secure initialisation vector
#define EXPECTED_INITNSVTOR     0x0003000

// <o> EXPECTED_INITTCMEN:  Expected TCM initialisation out of reset <3-0> [DTCM, ITCM]
#define EXPECTED_INITTCMEN      0b00

// <o> EXPECTED_INITPAHBEN:  Expected P-AHB enable initialisation out of reset
#define EXPECTED_INITPAHBEN     1

// <o> EXPECTED_INITECCEN:  Expected TCM and L1 cache ECC enable out of reset
#define EXPECTED_INITECCEN      1

// <o> EXPECTED_INITL1RSTDIS:  Expected Disable L1 cache invalidation out of reset
#define EXPECTED_INITL1RSTDIS   0
// </h>

// <h> MCU level configuration
// <o> EXPECTED_MCUROMADDR : Expected MCU ROM Address
#define EXPECTED_MCUROMADDR     0xE00FE000

// <o> EXPECTED_EWIC:  Expected External Wakeup controller <0,1>
#define EXPECTED_EWIC           0

// <o> EXPECTED_ETB:  Expected Embedded Trace Buffer <0,1>
#define EXPECTED_ETB            0

// TARGETID components
#define EXPECTED_TREVISION      0x0
#define EXPECTED_TDESIGNER      ((EXPECTED_CUST_JEPCONT<<7) | EXPECTED_CUST_JEPID)

// <o> EXPECTED_TARGETID:  Expected DAP TARGET_ID
#define EXPECTED_TARGETID       ((EXPECTED_TREVISION<<28) | (EXPECTED_PARTNO<<16) | (EXPECTED_TDESIGNER<<1) | 0x1)
#define EXPECTED_INSTANCEID     EXPECTED_TREVISION

// <o> EXPECTED_CUST_JEPID: Expected JEDEC JEP-106 identity code (0-0x7F) [6:0]
#define EXPECTED_CUST_JEPID     0x00

// <o> EXPECTED_CUST_JEPCONT: Expected JEDEC JEP-106 continuation code (0-0xF) [10:7]
#define EXPECTED_CUST_JEPCONT   0x0

// <o> EXPECTED_PARTNO: Expected Part number (0-0xFFF)
#define EXPECTED_PARTNO         0x000

// <o> EXPECTED_DPSEL: Expected Debug Port Select <0,1,2> [JTAG, SW, Dynamic]
#define EXPECTED_DPSEL          0

// <o> EXPECTED_SBISTC: Expected SBIST Controller <0,1>
#define EXPECTED_SBISTC         0

// <o> EXPECTED_TPIUM : Expected select TPIU/TPIUM (0,1)
#define EXPECTED_TPIUM          0

// <o> EXPECTED_FF_SYNC_DEPTH : Expected TPIU Synchronizers Depth (2,3)
#define EXPECTED_FF_SYNC_DEPTH     2

// <o> EXPECTED_TRACEPORT_DATA_WIDTH : Expected TPIU tracedata width (1, 2, 4, 8, 12, 16)
#define EXPECTED_TRACEPORT_DATA_WIDTH 4
// </h>

// <h> Other configuration
// <o> EXPECTED_ECOREVNUM: Expected Engineering Change Order and Revision number (0-0xF_FFFFFFFF)
#define EXPECTED_ECOREVNUM      0x000000000LL

// <o> EXPECTED_PDEPU: Expected Power Domain EPU <0,1>
#define EXPECTED_PDEPU          1

// <o> EXPECTED_PDDEBUG: Expected Power Domain Debug <0,1>
#define EXPECTED_PDDEBUG        1

// <o> EXPECTED_RETENTION: Expected retention support <0,1>
#define EXPECTED_RETENTION      1
// </h>


////////////////////////////////////////////////////////////////////////////////
//
// Define whether the execution_tb tests and the DebugDriver code should display messages
// using the chracter output device in the testbench.
//
////////////////////////////////////////////////////////////////////////////////
#define EXECTB_PRINTF
//#define DEBUGDRIVER_PRINTF
//#define DEBUGTRACE_PRINTF

// These values are used for TRACE ID when testing trace.
// Allows the formatter output to be associated with the generating source

#define EXECTB_ETM_ID           0x22
#define EXECTB_ITM_ID           0x59


////////////////////////////////////////////////////////////////////////////////
//
// ID values - DO NOT MODIFY
//
////////////////////////////////////////////////////////////////////////////////
// DAP Periperal ID registers (PID)
// Expected Manufacturer Revision number (0-0xF)
#define EXPECTED_CUST_REVAND    ((YAMIN_DAP_DP_ECO & 0xF0LL) >> 4) //  [7:4]
// Expected Revision number (0-0xF)
#define EXPECTED_CUST_REV       ((YAMIN_DAP_DP_ECO & 0x0FLL) >> 0) //  [3:0]

// Used by the Dhrystone benchmark - Sets the SYSTICK value
#define EXPECTED_SYST           1

#define YAMIN_CPUID              (0x411fd221 ^ (EXPECTED_ECOREVNUM & 0x0000000FLL))

// CoreSight Component Identifier for Peripheral classes
#define CORESIGHT_CID0          0x0D
#define CORESIGHT_CID1_tbl      0x10
#define CORESIGHT_CID1_dbg      0x90
#define CORESIGHT_CID1_mem      0xE0
#define CORESIGHT_CID1_prm      0xF0
#define CORESIGHT_CID2          0x05
#define CORESIGHT_CID3          0xB1

#define ARM_JEP_ID              0x3B
#define ARM_JEP_CONT            0x4

#define ETM_JEP_ID              0x3B
#define ETM_JEP_CONT            0x4

#define EWIC_JEP_ID             0x3B
#define EWIC_JEP_CONT           0x4

#define ITM_JEP_ID              0x3B
#define ITM_JEP_CONT            0x4

#define PMC_JEP_ID              0x3B
#define PMC_JEP_CONT            0x4

// Component Part Numbers
#define YAMIN_CPU_PART          0x4D2
#define YAMIN_PART              0xD22
#define YAMIN_CSI_PART          0x4C8
#define YAMIN_DAP_DP_PART       0xBD
#define YAMIN_CS_ETB_PART       0x9E9
#define YAMIN_PMC_PART          0x9BA
#define TPIUM_PART              0x9F1

// ARCHID codes
#define YAMIN_ITM_ARCHID        0x1A01
#define YAMIN_DWT_ARCHID        0x1A02
#define YAMIN_FPB_ARCHID        0x1A03
#define YAMIN_DBG_ARCHID        0x2A04
#define YAMIN_ETM_ARCHID        0x4A13
#define YAMIN_CTI_ARCHID        0x1A14
#define YAMIN_TPIU_ARCHID       0x0000
#define YAMIN_PMU_ARCHID        0x0A06

// Revision Codes
#define YAMIN_ITM_REV           0x0
#define YAMIN_ETM_REV           0x4
#define YAMIN_EWIC_REV          0x0
#define YAMIN_PMC_REV           0x1

// Revand
#define YAMIN_PMC_ECO           0x0
#define YAMIN_EWIC_ECO          0x0
#define YAMIN_SYSROM_ECO        ((EXPECTED_ECOREVNUM & 0x0F000000000000LL) >> 52)
#define YAMIN_TPIU_ECO          ((EXPECTED_ECOREVNUM & 0x00F00000000000LL) >> 48)
#define YAMIN_DAP_DP_ECO        ((EXPECTED_ECOREVNUM & 0x000F0000000000LL) >> 44)
#define YAMIN_DAP_AP_ECO        ((EXPECTED_ECOREVNUM & 0x0000F000000000LL) >> 40)
#define YAMIN_PMU_ECO           ((EXPECTED_ECOREVNUM & 0x00000F00000000LL) >> 36)
#define YAMIN_ETM_ECO           ((EXPECTED_ECOREVNUM & 0x000000F0000000LL) >> 28)
#define YAMIN_CTI_ECO           ((EXPECTED_ECOREVNUM & 0x0000000F000000LL) >> 24)
#define YAMIN_ROM_ECO           ((EXPECTED_ECOREVNUM & 0x00000000F00000LL) >> 20)
#define YAMIN_ITM_ECO           ((EXPECTED_ECOREVNUM & 0x000000000F0000LL) >> 16)
#define YAMIN_SCS_ECO           ((EXPECTED_ECOREVNUM & 0x0000000000F000LL) >> 12)
#define YAMIN_DWT_ECO           ((EXPECTED_ECOREVNUM & 0x00000000000F00LL) >>  8)
#define YAMIN_BPU_ECO           ((EXPECTED_ECOREVNUM & 0x000000000000F0LL) >>  4)

// DAP Registers
#define YAMIN_DAP_JTAG_TAPID    (0x0BA04477 | (YAMIN_DAP_DP_ECO << 28))
#define YAMIN_DAP_JTAG_DPIDR    (0x0BE11477 | (YAMIN_DAP_DP_ECO << 28))
#define YAMIN_DAP_SW_DPIDR      (0x3C013477 | (YAMIN_DAP_DP_ECO << 28))
#define YAMIN_DAP_SWMD_DPIDR    (0x3C013477 | (YAMIN_DAP_DP_ECO << 28))
#define YAMIN_DAP_APIDR         (0x04770051 | (YAMIN_DAP_AP_ECO << 28))

// CTI Registers
#define DEV_TYPE_VALUE          0x00000014

//GPIO Interrupt
#define GPIO_IRQn               0


////////////////////////////////////////////////////////////////////////////////
//
// Defines used in execution_tb - Do not modify
//
////////////////////////////////////////////////////////////////////////////////

#define SYSROMTABLE_BASE         0xE00FD000
#define PILROMTABLE_BASE         0xE00FE000
#define YAMINROMTABLE_BASE       0xE00FF000

#define DEVARCH_Offset           0xFBC
#define MEMTYPE_Offset           0xFCC
#define PID4_Offset              0xFD0
#define PID5_Offset              0xFD4
#define PID6_Offset              0xFD8
#define PID7_Offset              0xFDC
#define PID0_Offset              0xFE0
#define PID1_Offset              0xFE4
#define PID2_Offset              0xFE8
#define PID3_Offset              0xFEC
#define CID0_Offset              0xFF0
#define CID1_Offset              0xFF4
#define CID2_Offset              0xFF8
#define CID3_Offset              0xFFC

#define YAMIN_SCB_MVFR0          0x10110221

#define YAMIN_SCB_ID_PFR1_SEC    0x00000010
#define YAMIN_SCB_ID_PFR1_NO_SEC 0x00000000

#define YAMIN_SCB_ISAR0_COPRO    0x01143110
#define YAMIN_SCB_ISAR0_NOCOPRO  0x01103110

//SCB
#define SCB_CPACR_FPU            (0xF << 20)   // set cp10 in CPACR



////////////////////////////////////////////////////////////////////////////////

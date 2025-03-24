# GPR Example

This example implements a 32-bit integer [population count](https://blog.heycoach.in/population-count-algorithm-in-c/) custom instruction. The population count instruction is useful for many algorithms, for example to calculate the [Hamming weight](https://en.wikipedia.org/wiki/Hamming_weight) or hamming distance as shown below.

``` c
// Function to calculate Hamming distance using population count
uint32_t hamming_distance(uint32_t x, uint32_t y) {
    // XOR the two numbers, then count the number of 1's in the result
    return popc_u32 (x ^ y);     // population count
}
```

A trivial implementation of the `popc_u32` function is shown below:

``` c
uint32_t popc_u32 (uint32_t x) {
    uint32_t count;
    for (count=0; x; count++) {
        x &= x - 1;
    }
    return count;
}
```

The [White-Paper Arm Custom Instructions: Enabling
Innovation and Greater Flexibility on Arm](https://armkeil.blob.core.windows.net/developer/Files/pdf/white-paper/arm-custom-instructions-wp.pdf) uses this example and shows a reduction of the execution time from 25 cycles to just 1 cycle using ACI.

In the chip design, the functional verification of hardware logic—especially newly added ACI instructions—relies on dedicated **test benches**. These test benches use C-based test cases tailored for specific functionalities.

To facilitate development and validation of ACI-related firmware alongside hardware development, we provide a software environment in the **test** folder that allows direct execution of test bench test cases.

> [!CAUTION]
>
> It is important to note that **FVP is not a cycle-accurate simulation model**. While `__cycleof__()` relies on system hardware counters such as SysTick or PMU for cycle measurement, these counters themselves are software-simulated in FVP and do not guarantee cycle accuracy. Therefore, the performance measurements obtained during simulation should be considered only as **rough estimates** rather than definitive benchmarks.

## Steps to Custom Instruction

The following steps describe how to implement the `popc_u32` function as custom instruction. Each step is explained in more detail and in a related directory.

| Step | Description                                                                                            | Directory |
|:----:|:-------------------------------------------------------------------------------------------------------|:----------|
|**1.**| [Map custom instructions](#map-custom-instructions) to Custom Datapath Extension (CDE)                 | inc       |
|**2.**| [Create plugin for AVH-FVP simulation models](#create-avh-fvp-plugin) that adds custom instructions    | plugin    |
|**3.**| [Create test code to validate](#create-test-code) the correctness of the AVH-FVP simulation            | test      |
|**4.**| [Use custom instructions](#use-custom-instructions) in your algorithm to estimate performance gains    | example   |

The steps for creating the processor hardware are not described here, but the test code created in step 3 can be reused also for hardware verification.

### Map Custom Instructions

The include file `./inc/aci_gpr_lib.h` contains the ACI mapping for the `popc_u32` instruction.  In this example, the `CX1A` intrinsic function is used with `ID=0` and `imm=0`. Further instructions may be defined with a different `imm` value.

The header file also defines the functions:

- `aci_init` to enable the related ACI accelerator.
    - enables access in non-secure mode.

### Create AVH-FVP Plugin

The simulation of the CX1A instruction is implemented in the module `./GPR/plugin/cde_plugin.cpp` with the function `aci_fvp::exec_cx1`.  For `imm=0` the a simulation code for popc_u32 is called.

The plugin is generated using [GitHub workflows](../actions) for Linux and Windows. To download the plugin for local usage click on the related action (for example `Build CDE plugins for Windows AMD64`), select the latest workflow run. The DLL is located under **Artifacts**.

### Create Test Code

The test code verifies the execution of the `pop_u32` instruction. This test may be reused later also for validation of the hardware implementation.

### Use Custom Instructions

To use the custom instruction, just call the function `popc_u32` that is defined in the include file `./GPR/inc/aci_gpr_lib.h`.  The MDK project in the folder `./GPR/example/mdk` shows the usage.

## Folder Structure

| Folder                | Description                                      |
|:----------------------|:------------------------------------------------ |
| [example](./example/) | the example project.                             |
| [inc](./inc/)         | the folder containing ACI library header file(s) |
| [src](./src/)         | the folder containing ACI library source file(s) |
| [plugin](./plugin/)   | the CDE plugin makefile project                  |
| [test](./test/)       | the validation project for the ACI instruction   |

## Using the Example

To use the example on a local development computer with MDK uVision on Windows follow these steps:

### Step 1: Install the MDK

Please install the MDK version 5.42a or higher from the [www.keil.arm.com](https://www.keil.arm.com/)

> [!TIP]
>
> - The [Community Edition](https://www.keil.arm.com/mdk-community/) is sufficient and can be used **ONLY** for evaluation and non-commercial use.

> [!IMPORTANT]
>
> The MDK project utilizes the FVP with the CDE plugin for debugging.
>
> Starting from version 5.42, MDK provides **specially optimized FVPs and a CDE Loader** to enhance the user experience for ACI simulation. **Please ensure you have MDK v5.42 or later** when using ACI-GetStarted.



### Step 2: Download the CDE plugin for simulation

Open [github.com/arm-software/ACI-GetStarted - Actions](https://github.com/arm-software/ACI-GetStarted/actions) and select `Build CDE plugins for Windows AMD64`. Then open the latest workflow run and download the related DLL available under **Artifacts**.  

After downloading the cde plugin, please copy it to the `plugin/build` folder, where the default MDK project debug setting points to. 

```sh
... -C CDE.CDELoader.aci_object_files="[..\..\plugin\build\cde_plugin.dll]" ...
```

If you want to place it to a different place, please update the MDK project debug setting accordingly.

> [!TIP]
>
> If you want to build the cde plugin locally in windows, please see [here](../document/How_to_build_CDE_Plugins_Locally_in_Windows.md).



### Step 3: Open the MDK project

Navigate to the folder `./GPR/example/mdk` and open in uVision the MDK project `ACI-Example.uvmpw`.

> [!NOTE]
>
> If this is the first time you open this project, MDK might ask you to install the missing software packs via the Pack-Installer. If you have trouble to install those packs in Pack-Installer, here are the links for manual download and installation:
>
> - Arm-2D: https://www.keil.arm.com/packs/arm-2d-arm/versions/
> - SSE-300-BSP: https://www.keil.arm.com/packs/v2m_mps3_sse_300_bsp-arm/boards/
> - perf_counter: https://www.keil.arm.com/packs/perf_counter-gorgonmeducer/versions/

### Step 4: Compile and Debug

You can compile the project by pressing `F7`. You should see no errors (and probably some warnings) in the **Build Output**:

```txt
".\Objects\ACI-Example.axf" - 0 Error(s), 0 Warning(s).
```

After that you can start a debug session by pressing `CTRL`+`F5`, in the **Debug (printf) View**, we can find the test report:

```txt
-[Cycle Report]------------------------------------
Calculate Hamming distance in C total cycle count: 585307 [0008ee5b]

-[Cycle Report]------------------------------------
Calculate Hamming distance in ACI total cycle count: 40601 [00009e99]

Validate the output...PASS!
```

> [!CAUTION]
>
> The Fast Model can only be used to verify the function correctness, and it is **NOT** cycle accurate. Any CPU cycle information obtained from the Fast Model can only provide **limited** and **rough references** at the **magnitude level**.



> [!NOTE]
>
> 1. The MDK project is configured to use `FVP_Corstone_SSE-300.exe` for debug. You can find it in `<Path for MDK>\ARM\avh-fvp\bin\models\` directory. 
> 2. You should build the CDE plugin first. The debug session will load it with the following command line:
>
>    ```shell
>    --plugin "$Karm\avh-fvp\plugins\CDELoader.dll" -C cpu0.has_cde=1 -C cpu0.cde_impl_name="ACI GPR Library" -C CDE.CDELoader.aci_object_files="[..\..\plugin\build\cde_plugin.dll]" -C CDE.CDELoader.aci_parameters="COPRO=1" -C core_clk.mul=10000000
>    ```
>     Here we assume that you have installed the fast model in the default path.
>
> 3. The example code uses a keyword like macro `__cycleof__()` to measure the cycles the code uses inside the `{...}`. This macro comes from a software tool, `perf_counter`. It is an [open-source project on Github](https://github.com/GorgonMeducer/perf_counter) and available as a [cmsis-pack](https://www.keil.arm.com/packs/perf_counter-gorgonmeducer/versions/). 

For more details, please check the [CDELoader section of the Fast Model Reference Guide](https://developer.arm.com/documentation/100964/1127/Plug-ins-for-Fast-Models/CDELoader?lang=en). 

# GPR-ACI Example

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

- `aci_gpr_init` to enable the related ACI accelerator.
- `aci_gpr_NS_access` which is called in secure mode to enable access in non-secure mode.

### Create AVH-FVP Plugin

The simulation of the CX1A instruction is implemented in the module `./plugin/cde_plugin.cpp` with the function `aci_fvp::exec_cx1`.  For `imm=0` the a simulation code for popc_u32 is called.

### Create Test Code

The test code verifies the execution of the `pop_u32` instruction. This test may be reused later also for validation of the hardware implementation.

### Use Custom Instructions

To use the custom instruction, just all the function `popc_u32` that is defined in the include file `./inc/aci_gpr_lib.h`.

------



# Get Started with GPR-ACI using Fast Model


### About the Platform and Example Content

To simulate the ACI instruction set, **ACI-GetStarted** contains a makefile project that generates the corresponding [CDE(Custom Datapath Extension) plugin](https://developer.arm.com/documentation/100964/1127/Plug-ins-for-Fast-Models/CDE?lang=en)  using GCC. We also use an MDK project to show the firmware design flow. 

> [!IMPORTANT]
>
> The MDK project utilizes the FVP with the CDE plugin for debugging.
>
> Starting from version 5.42, MDK provides **specially optimized FVPs and a CDE Loader** to enhance the user experience for ACI simulation. **Please ensure you have MDK v5.42 or later** when using ACI-GetStarted.



- **How to use ACI to access General Purpose Registers in Arm Compilers for acceleration**
> [!TIP]
>
> This is shown in the source files inside the `ACI/GPR/inc` and `ACI/GPR/src` directory

- **How to generate (compile) a CDE plugin that can be loaded into Fast Model for ACI simulation**
> [!TIP]
>
> 
> This is shown with a makefile project in the `ACI/GPR/plugin` directory; detailed guidance is covered in chapter 2 of this document.

- **How to load the CDE plugin and debug the firmware in Fast Model using MDK**

> [!TIP]
>
> This is shown with an MDK project in the `ACI/GPR/example/mdk` folder; detailed guidance is covered in chapter 3.



### Folder Structure

| Folder  | Description                                      |
| ------- | ------------------------------------------------ |
| example | the example project.                             |
| inc     | the folder containing ACI library header file(s) |
| src     | the folder containing ACI library source file(s) |
| plugin  | the CDE plugin makefile project                  |
| test    | test project                                     |


## 1 Prepare the Environment

### 1.1 Install the MDK

Please install the latest MDK from the [official website](https://www.keil.arm.com/)

> [!TIP]
>
> 1. [Community License](https://www.keil.arm.com/mdk-community/) is available and **ONLY** for non-commercial use. 



### 1.3 Install the MinGW-w64

MinGW-w64 is a free and open-source compiler toolchain for generating executables on Windows. Follow these steps to download it:

1. Visit the GitHub Releases Page:

   Open your browser and go to the MinGW-w64 binaries GitHub releases page:

   ```
   https://github.com/niXman/mingw-builds-binaries/releases
   ```

2. Select the Latest Version:

   - On the page, find the latest release version. For example, the current latest version is `14.2.0-rt_v12-rev0`.
   - Click on that version to expand its assets list.

3. Download the Appropriate Version:

   Download the build that uses **POSIX threads**, **SEH exception handling**, and **UCRT runtime**, i.e. please look for a file with a name similar to:

   ```
   x86_64-14.2.0-release-posix-seh-ucrt-rt_v12-rev0.7z
   ```

> [!TIP]
>
> **Understanding POSIX, SEH, and UCRT:**
>
> - **POSIX Threads (pthreads):** A threading model that allows concurrent execution in programs.
> - **SEH (Structured Exception Handling):** Windows' mechanism for handling exceptions and errors.
> - **UCRT (Universal C Runtime):** A modern C runtime library providing standard C library functions.

4. Extract the downloaded `.7z` file and place the content to `C:\mingw64` directory. 

5. Setting Environment Variables for MinGW-w64

   To use MinGW-w64 from the command line, you need to add it to your system's PATH environment variable.

   i. **Open Environment Variables Settings:**

      - Press `Win + X` and select **System** (or **System and Security > System**).
      - Click on **Advanced system settings** on the left panel.
      - In the **System Properties** window, click **Environment Variables**.

   ii. **Edit the System PATH Variable:**

      - Under **System variables**, find and select the **Path** variable.
      - Click **Edit**.

   iii. **Add MinGW-w64 to PATH:**

      - Click **New** and add the following path:

        ```
        C:\mingw64\bin
        ```

      - Ensure there are no extra spaces or characters.

      - Click **OK** to save the settings.

### 1.4 Install the Make

This guide will help you install the standalone Make tool on your Windows 10 or 11 system.

- **Visit the Official Download Page:**

   Go to the GnuWin32 project page for Make:

   ```
   http://gnuwin32.sourceforge.net/packages/make.htm
   ```

- **Download the Installer:**

   1. In the **Download** section, find the link labeled **Complete package, except sources**.
   2. Click on the link to download the installer.

- **Installation Process:**

   1. Follow the on-screen instructions provided by the installer.

   2. When prompted to choose the installation directory, you can accept the default location or specify a different one. For example:

     ```
     C:\Program Files (x86)\GnuWin32
     ```
   
   3. Proceed through the installation steps until it completes.

- **Setting Environment Variables for Make**

   To use the Make tool from the command line, you need to add its `bin` directory to your system's `PATH` environment variable. This allows the system to recognize the `make` command from any directory.

   1.  **Open System Environment Variables:**
       
       - Press `Win + R` to open the Run dialog box.
            - Type `sysdm.cpl` and press **Enter**. This opens the **System Properties** window and then click on **Advanced**.

   2. **Access Environment Variables:**

      - In the **System Properties** window, ensure you're on the **Advanced** tab.

      - Click on the **Environment Variables** button at the bottom.

   3. **Edit the System PATH Variable:**

      - Under the **System variables** section, scroll to find the variable named **Path**.

      - Select **Path** and click on the **Edit** button.

   4. **Add Make to the PATH Variable:**

      In the **Edit Environment Variable** window, you'll see a list of paths.
      - Click **New** to add a new entry at the bottom of the list.

      - Enter the path to the `bin` directory of your Make installation. If you used the default installation directory, the path is:

         ```
         C:\Program Files (x86)\GnuWin32\bin
         ```
> [!TIP]
>
>  Ensure the path is correct by navigating to it in File Explorer to confirm that `make.exe` is present in that directory.

   5.  **Save the Changes:**
   - Click **OK** to close the **Edit Environment Variable** window.



## 2 How to generate (compile) a CDE plugin that can be loaded into Fast Model for Helium-ACI simulation

### Step 1: Open the Command Prompt in the Project Directory

1. Open File Explorer and Navigate to the project directory `ACI\GPR\plugin`. 

2. Click on the address bar at the top of the File Explorer window. The current path will become editable. Type `cmd`in the address bar and Press **Enter**. A Command Prompt window will open, and it will be set to the current directory, i.e.  `ACI\MVE\plugin`.

> [!NOTE]
>
> This method opens the Command Prompt directly in the directory you're viewing, eliminating the need to navigate using `cd` commands.

### Step 2: Run Make to Compile the DLL

   Execute the `make` command to start the compilation.

   ```
   make clean all
   ```
   If everything goes well, you should find the `cde_plugin.dll` in the `build` folder.



## 3 How to load the CDE plugin and debug the firmware in Fast Model using MDK

### Step 1: Open the MDK project

Navigate to the folder `project\mdk` and open the MDK project `ACI-Example.uvmpw`.

> [!NOTE]
>
> If this is the first time you open this project, MDK might ask you to install the missing cmsis-packs via the Pack-Installer. If you have trouble to install those packs in Pack-Installer, here are the links of the packs. You can download and install them manually:
>
> - Arm-2D: https://www.keil.arm.com/packs/arm-2d-arm/versions/
> - SSE-300-BSP: https://www.keil.arm.com/packs/v2m_mps3_sse_300_bsp-arm/boards/
> - perf_counter: https://www.keil.arm.com/packs/perf_counter-gorgonmeducer/versions/



### Step 2: Compile and Debug

You can compile the project by pressing `F7`. You should see no errors (and probably some warnings) in the **Build Output**:

```
Program Size: Code=11224 RO-data=972 RW-data=20 ZI-data=11580  
".\Objects\ACI-Example.axf" - 0 Error(s), 0 Warning(s).
```

After that you can start a debug session by pressing `CTRL`+`F5`, in the **Debug (printf) View**, we can find the test report:

```c
-[Cycle Report]------------------------------------
Calculate Population Count in C total cycle count: 830394 [000cabba]
Correctness Validation: 0x00025e3b

-[Cycle Report]------------------------------------
Calculate Population Count in ACI total cycle count: 220394 [00035cea]
Correctness Validation: 0x00025e3b
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

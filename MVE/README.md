

# Get Started with MVE-ACI using Fast Model


### About the Platform and Example Content

To simulate the ACI instruction set, **ACI-GetStarted** contains a makefile project that generates the corresponding [CDE(Custom Datapath Extension) plugin](https://developer.arm.com/documentation/100964/1127/Plug-ins-for-Fast-Models/CDE?lang=en)  using GCC. We also use an MDK project to show the firmware design flow. 

> [!IMPORTANT]
>
> The MDK project utilizes the FVP with the CDE plugin for debugging.
>
> Starting from version 5.42, MDK provides **specially optimized FVPs and a CDE Loader** to enhance the user experience for ACI simulation. **Please ensure you have MDK v5.42 or later** when using ACI-GetStarted.



In addition to software simulation, this MDK project is based on [AN552 (which is the Arm® Corstone™ SSE-300 with Cortex®-M55 and Ethos™-U55: an example subsystem for MPS3)](https://developer.arm.com/documentation/dai0552/c/?lang=en), an FPGA image that runs on the MPS3 platform. [The Arm MPS3 FPGA prototyping board](https://developer.arm.com/Tools%20and%20Software/MPS3%20FPGA%20Prototyping%20Board) is a platform that allows designers to easily develop systems. For more information, please see [here](https://developer.arm.com/documentation/dai0552/c?lang=en). 

The AN552 implements a Helium-ACI instruction set for RGB565 image processing; however, the corresponding Fast Model does **not** provide the simulation of these Helium-ACI instructions. This allows the content of the project to demonstrate:

- **How to use Helium-ACI in Arm Compilers for acceleration**
> [!TIP]
>
> This is shown in the source files inside the `ACI/MVE/inc` and `ACI/MVE/src` directory

- **How to generate (compile) a CDE plugin that can be loaded into Fast Model for Helium-ACI simulation**
> [!TIP]
>
> 
> This is shown with a makefile project in the `ACI/MVE/plugin` directory; detailed guidance is covered in chapter 2 of this document.

- **How to load the CDE plugin and debug the firmware in Fast Model using MDK**

> [!TIP]
>
> This is shown with an MDK project in the `ACI/MVE/example/mdk` folder; detailed guidance is covered in chapter 3.



### Folder Structure

| Folder  | Description                                      |
| ------- | ------------------------------------------------ |
| example | the example project.                             |
| inc     | the folder containing ACI library header file(s) |
| src     | the folder containing ACI library source file(s) |
| plugin  | the CDE plugin makefile project                  |
| test    | test project                                     |

In the **example** folder, we use a typical algorithm in graphics processing—**image-copying-with-an-alpha-mask**—as a case study. We provide three versions of the same algorithm: a pure C implementation, a Helium-accelerated version, and a Helium-ACI-accelerated version. The copied output is displayed on an LCD panel simulated by the FVP, allowing users to visually compare and inspect the effects of these three implementations. Additionally, the `__cycleof__()` function is used to measure the CPU cycle count for each version.

In the chip design, the functional verification of hardware logic—especially newly added ACI instructions—relies on dedicated **test benches**. These test benches use C-based test cases tailored for specific functionalities.

To facilitate development and validation of ACI-related firmware alongside hardware development, we provide a software environment in the **test** folder that allows direct execution of test bench test cases.

> [!CAUTION]
>
> It is important to note that **FVP is not a cycle-accurate simulation model**. While `__cycleof__()` relies on system hardware counters such as SysTick or PMU for cycle measurement, these counters themselves are software-simulated in FVP and do not guarantee cycle accuracy. Therefore, the performance measurements obtained during simulation should be considered only as **rough estimates** rather than definitive benchmarks.



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

1. Open File Explorer and Navigate to the project directory `ACI\MVE\plugin`. 

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
Program Size: Code=75816 RO-data=17532 RW-data=248 ZI-data=24920  
".\Out\template.axf" - 0 Error(s), 0 Warning(s).
```

After that you can start a debug session by pressing `CTRL`+`F5`, you should see the **Corstone_SSE-300 Fast Model** windows as shown below:

![](./document/pictures/FastModel.png) 

In the **Debug (printf) View**, we can find the test report:

```c
ACI Example

-[Cycle Report]------------------------------------
Draw CMSIS Logo using C total cycle count: 379333 [0005c9c5]

-[Cycle Report]------------------------------------
Draw CMSIS Logo using Helium total cycle count: 75817 [00012829]

-[Cycle Report]------------------------------------
Draw CMSIS Logo using Helium-ACI total cycle count: 14373 [00003825]


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
>    --plugin "$Karm\avh-fvp\plugins\CDELoader.dll" -C cpu0.has_cde=1 -C cpu0.cde_impl_name="ARM_2D_RGB565_ACI" -C CDE.CDELoader.aci_object_files="[..\..\plugin\build\cde_plugin.dll]" -C CDE.CDELoader.aci_parameters="COPRO=1" -C core_clk.mul=10000000
>    ```
>     Here we assume that you have installed the fast model in the default path.
>
> 3. The example code uses a keyword like macro `__cycleof__()` to measure the cycles the code uses inside the `{...}`. This macro comes from a software tool, `perf_counter`. It is an [open-source project on Github](https://github.com/GorgonMeducer/perf_counter) and available as a [cmsis-pack](https://www.keil.arm.com/packs/perf_counter-gorgonmeducer/versions/). 

For more details, please check the [CDELoader section of the Fast Model Reference Guide](https://developer.arm.com/documentation/100964/1127/Plug-ins-for-Fast-Models/CDELoader?lang=en). 

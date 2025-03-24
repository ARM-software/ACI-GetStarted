# How to build CDE Plugins Locally in Windows

## 1 Prepare the Environment

### 1.1 Install the MinGW-w64

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

### 1.2 Install the Make

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

  1. **Open System Environment Variables:**

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
> Ensure the path is correct by navigating to it in File Explorer to confirm that `make.exe` is present in that directory.

      5.  **Save the Changes:**

   - Click **OK** to close the **Edit Environment Variable** window.



## 2 How to generate (compile) a CDE plugin

### Step 1: Open the Command Prompt in the Project Directory

1. Open File Explorer and Navigate to the project directory, e.g.  `ACI\GPR\plugin`. 

2. Click on the address bar at the top of the File Explorer window. The current path will become editable. Type `cmd`in the address bar and Press **Enter**. A Command Prompt window will open, and it will be set to the current directory, e.g.  `ACI\GPR\plugin`.

> [!NOTE]
>
> This method opens the Command Prompt directly in the directory you're viewing, eliminating the need to navigate using `cd` commands.

### Step 2: Run Make to Compile the DLL

   Execute the `make` command to start the compilation.

   ```
make clean all
   ```

   If everything goes well, you should find the `cde_plugin.dll` in the `build` folder.


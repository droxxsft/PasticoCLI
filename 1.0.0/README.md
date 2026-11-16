# Pastico CLI 1.0.0


**Pastico** is a fast, lightweight, and 100% native Windows command-line (CLI) jailbreak and device diagnostics utility built in pure C for **Apple A11 Bionic** devices running **iOS 16.7**.

No Linux live USBs, virtual machines, WSL, or manual compiling required. Just download, extract, and run directly from Windows Command Prompt (`cmd.exe`).

---

## What Can Pastico Do? (Core Features)

- **1-Click Semi-Tethered Rootless Jailbreak (`boot`):** Automatically triggers the hardware `checkm8` BootROM exploit (`gaster`), boots `PongoOS`, injects the iOS 16.7 Kernel Patchfinder (`KPF`), and uploads the Rootless `ramdisk` and `binpack` userland overlay via native USB (`pongoterm`). Once booted, the Loader app appears on your Home Screen to install **Sileo** or **Zebra**.
- **Deep Device & Jailbreak Diagnostics (`deviceinfo`):** Connects to any iOS device in Normal Mode to read hardware identifiers (`UDID`, `Serial Number`, `Product Type`, `CPU Architecture`, `iOS Version`) and automatically detects whether the device is **Jailbroken**, distinguishes between **Rootless (`/var/jb`)** and **Rootful (`FakeFS`)** environments, and lists installed jailbreak tools (`palera1n`, `Sileo`, `Zebra`, `Cydia`, `Filza`, `NewTerm`).
- **Smart USB Driver Assistant:** Automatically checks for required `libusbK` drivers during DFU and PongoOS transitions and launches the bundled `zadig.exe` helper if a driver needs to be installed.
- **DFU Hardware Verification (`check`):** Safely inspects connected DFU/Recovery devices (`CPID`, `ECID`, `PRODUCT`, `MODE`) before running any exploit.
- **Ready-to-Run & Portable:** Pre-compiled `pastico.exe` with all dependencies included. Can be added to Windows `PATH` and executed from any directory on your PC.

---

## Supported Devices & Requirements

- **Supported Hardware:** iPhone 8, iPhone 8 Plus, iPhone X (`A11 Bionic - CPID: 0x8015`)
- **Supported Firmware:** **iOS 16.7 Only** _(iOS 16.7.1 and newer are NOT supported)_
- **Operating System:** Windows 10 / Windows 11 (64-bit)
- **Passcode Rule (Crucial):** On A11 devices running iOS 16, **Passcode, Touch ID, and Face ID must be completely turned off** before running `pastico boot`.

---

## Quick Start Guide (No Compiling Needed)

### Step 1: Download & Extract

1. Click the green **`<> Code`** button at the top of this repository and select **`Download ZIP`** (or download the latest archive from the [Releases](../../releases) page).
2. Extract the ZIP archive to a folder on your PC (e.g., `C:\Pastico`).
3. _(Optional)_ Add `C:\Pastico` to your Windows **System PATH** environment variables so you can type `pastico` from any folder.

### Step 2: Check Device & Jailbreak Status (Normal Mode)

1. Ensure **Apple Mobile Device Support** (iTunes or Apple Devices app) is installed on your PC.
2. Connect your unlocked iPhone via USB and tap **Trust This Computer**.
3. Open Command Prompt (`cmd.exe`) inside the extracted folder and run:
   ```cmd
   pastico deviceinfo
   ```

### Step 3: Jailbreak Your Device (DFU Mode)

1. Turn off **Passcode / Touch ID / Face ID** in your iPhone Settings.
2. Put your iPhone 8 / 8 Plus / X into **DFU Mode**.
3. Open Command Prompt (`cmd.exe`) and verify the DFU connection:
   ```cmd
   pastico check
   ```
   _(If Windows does not recognize the DFU device, run `bin\zadig.exe`, select **Apple Mobile Device (DFU Mode)**, and install the **libusbK** driver)._
4. Start the jailbreak process:
   ```cmd
   pastico boot
   ```
5. Type `yes` to accept the legal disclaimer. When the device transitions into **PongoOS** mode for the first time, if prompted, install the **libusbK** driver for `PongoOS USB Device` using the pop-up Zadig window and press `ENTER`.
6. Once your iPhone boots to the Home Screen, open the **Loader** app and install **Sileo**.

---

## Command Reference

| Command              | Description                                                                |
| :------------------- | :------------------------------------------------------------------------- |
| `pastico boot`       | Executes the full Semi-Tethered Rootless jailbreak boot sequence.          |
| `pastico deviceinfo` | Displays full hardware info and detects Rootless/Rootful jailbreak status. |
| `pastico check`      | Verifies connected DFU/Recovery device hardware (`CPID: 0x8015`).          |
| `pastico reboot`     | Reboots a device out of Recovery/DFU mode back into normal iOS.            |
| `pastico --version`  | Displays current Pastico version and license details.                      |
| `pastico --help`     | Shows the built-in command-line help menu.                                 |

---

## Legal Disclaimer & Open-Source Credits

This software is provided **"AS-IS"** for educational and security research purposes only, without warranty of any kind. The user assumes all responsibility for any data loss, device instability, or warranty voiding. This repository contains **no** copyrighted Apple firmware files or piracy tools.

- **gaster:** Copyright (c) 0x7ff (MIT License)
- **PongoOS & KPF:** Copyright (c) palera1n / checkra1n team
- **libimobiledevice & libirecovery:** Copyright (c) libimobiledevice contributors (LGPL-2.1)
- **Zadig / libwdi:** Copyright (c) Pete Batard (LGPL-3.0)

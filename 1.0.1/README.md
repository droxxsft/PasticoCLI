# Pastico CLI

**Pastico** is a fast, lightweight, and 100% native Windows command-line (CLI) jailbreak and device diagnostics utility built in pure C for **Apple A11 Bionic** devices running **iOS 16.7**.

No Linux live USBs, virtual machines, WSL, or manual compiling required. Just download, extract, and run directly from Windows Command Prompt (`cmd.exe`).

---

## What Can Pastico Do? (Core Features)

- **1-Click Semi-Tethered Rootless Jailbreak (`jb`):** Automatically detects your iPhone in Normal Mode, reboots it into Recovery Mode, guides you through an interactive `palera1n`-style DFU countdown assistant, triggers the `checkm8` BootROM exploit (`gaster`), boots `PongoOS`, injects the iOS 16.7 Kernel Patchfinder (`KPF`), and uploads the Rootless `ramdisk` and `binpack` userland overlay via native USB (`pongoterm`). Once booted, open the Loader app on your Home Screen to install **Sileo** or **Zebra**.
- **Safe Mode Boot (`jb --safe-mode`):** Boots the device into a jailbroken state with all tweaks temporarily disabled, allowing you to troubleshoot or remove incompatible packages.
- **Normal Stock Boot (`boot`):** Cleanly exits DFU/Recovery mode and boots your device back into standard unjailbroken iOS.
- **Device Restart (`reboot`):** Restarts a connected iOS device from either Normal Mode or Recovery/DFU Mode.
- **Deep Device & Jailbreak Diagnostics (`deviceinfo`):** Reads hardware identifiers (`UDID`, `Serial Number`, `Product Type`, `CPU Architecture`, `iOS Version`), checks root filesystem (`AFC2`) access, distinguishes between **Rootless (`/var/jb`)** and **Rootful (`FakeFS`)** setups, and detects installed jailbreak tools (`palera1n`, `Sileo`, `Zebra`, `Cydia`, `Filza`, `NewTerm`).
- **Smart USB Driver Assistant:** Automatically detects missing `libusbK` drivers during DFU and PongoOS transitions and launches the bundled `zadig.exe` helper when needed.

---

## Supported Devices & Requirements

- **Supported Hardware:** iPhone 8, iPhone 8 Plus, iPhone X (`A11 Bionic - CPID: 0x8015`)
- **Supported Firmware:** **iOS 16.7 Only** _(iOS 16.7.1 and newer are NOT supported)_
- **Operating System:** Windows 10 / Windows 11 (64-bit)
- **Passcode Rule (Crucial):** On A11 devices running iOS 16, **Passcode, Touch ID, and Face ID must be completely turned off** before running `pastico jb`.

---

## Quick Start Guide (No Compiling Needed)

### Step 1: Download & Extract

1. Download the latest **`Pastico-v1.0.1-Windows-x64.zip`** from the [Releases](../../releases) page (or click **`<> Code > Download ZIP`**).
2. Extract the archive to a folder on your PC (e.g., `C:\Pastico`).
3. _(Optional)_ Add `C:\Pastico` to your Windows **System PATH** environment variables to run `pastico` from any directory.

### Step 2: Check Device & Jailbreak Status (Normal Mode)

1. Ensure **Apple Mobile Device Support** (iTunes or Apple Devices app) is installed on your PC.
2. Connect your unlocked iPhone via USB and tap **Trust This Computer**.
3. Open Command Prompt (`cmd.exe`) and run:
   ```cmd
   pastico deviceinfo
   ```

### Step 3: Jailbreak Your Device

1. Turn off **Passcode / Touch ID / Face ID** in your iPhone Settings.
2. Connect your iPhone 8 / 8 Plus / X via USB (can be powered on in Normal Mode or already in DFU/Recovery).
3. Open Command Prompt (`cmd.exe`) and run:
   ```cmd
   pastico jb
   ```
4. Accept the legal disclaimer (`yes`), press `ENTER` to let Pastico automatically enter Recovery Mode, and follow the on-screen **Interactive DFU Countdown Assistant**.
5. When transitioning into **DFU** or **PongoOS** mode for the first time, if prompted, install the **`libusbK`** driver using the bundled Zadig window and press `ENTER`.
6. Once your iPhone boots to the Home Screen, open the **Loader** app and install **Sileo**.

---

## Command Reference

| Command                  | Description                                                                |
| :----------------------- | :------------------------------------------------------------------------- |
| `pastico jb`             | Runs Auto-Recovery, Interactive DFU Assistant, and Rootless Jailbreak.     |
| `pastico jb --safe-mode` | Boots into Rootless Jailbreak with tweaks disabled (Safe Mode).            |
| `pastico boot`           | Boots device from DFU/Recovery mode into stock normal iOS.                 |
| `pastico reboot`         | Restarts a connected iOS device (Normal or Recovery mode).                 |
| `pastico deviceinfo`     | Displays full hardware info and detects Rootless/Rootful jailbreak status. |
| `pastico --version`      | Displays current Pastico version and license details.                      |
| `pastico --help`         | Shows the built-in command-line help menu.                                 |

---

## Legal Disclaimer & Open-Source Credits

This software is provided **"AS-IS"** for educational and security research purposes only, without warranty of any kind. The user assumes all responsibility for any data loss, device instability, or warranty voiding. This repository contains **no** copyrighted Apple firmware files or piracy tools.

- **gaster:** Copyright (c) 0x7ff (MIT License)
- **PongoOS & KPF:** Copyright (c) palera1n / checkra1n team
- **libimobiledevice & libirecovery:** Copyright (c) libimobiledevice contributors (LGPL-2.1)
- **Zadig / libwdi:** Copyright (c) Pete Batard (LGPL-3.0)

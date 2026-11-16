# Pastico

**Pastico** is a lightweight, 100% native Windows utility designed for **Apple A11 Bionic** devices running **iOS 16.7**. It combines a Semi-Tethered Rootless Jailbreak workflow with advanced iOS device and jailbreak diagnostics in a single portable package.

Unlike legacy solutions that require Linux live USBs, virtual machines, WSL, or manual compiling, Pastico runs natively on Windows out of the box.

---

## What Does Pastico Do?

- **Semi-Tethered Rootless Jailbreak:** Guides you through entering DFU mode with an interactive step-by-step countdown assistant, triggers the hardware `checkm8` exploit, boots `PongoOS`, applies the iOS 16.7 kernel patches, and loads the Rootless userland environment so you can install **Sileo** or **Zebra** directly on your device.
- **Safe Mode Booting:** Allows booting into a jailbroken state with all tweaks disabled so you can safely troubleshoot or remove problematic packages.
- **Normal Boot & Device Restart:** Cleanly exits Recovery/DFU mode to boot standard iOS, or restarts a connected device directly from Windows.
- **Device & Jailbreak Diagnostics:** Reads detailed hardware identifiers (`UDID`, `Serial Number`, `Model`, `Architecture`, `iOS Version`), checks root filesystem access, detects active jailbreak status, distinguishes between **Rootless** and **Rootful** architectures, and identifies installed jailbreak managers.
- **Built-in USB Driver Helper:** Automatically checks for required USB drivers during DFU and PongoOS transitions and launches the included Zadig tool when a driver setup is needed.

---

## Supported Devices & Firmware

- **Supported Hardware:** iPhone 8, iPhone 8 Plus, iPhone X (`A11 Bionic - CPID: 0x8015`)
- **Supported Firmware:** **iOS 16.7 Only** _(iOS 16.7.1 and newer versions are NOT supported)_
- **Operating System:** Windows 10 / Windows 11 (64-bit)
- **Passcode Requirement:** On A11 devices running iOS 16, **Passcode, Touch ID, and Face ID must be completely turned off** before jailbreaking.

---

## Download & Installation

No compiling or extra setup is required. Simply download the latest pre-built package from the **[Releases](../../releases)** page, extract the ZIP archive to a folder on your PC, and run `pastico.exe` via Windows Command Prompt.

---

## Legal Disclaimer & Open-Source Credits

This software is provided **"AS-IS"** for educational and security research purposes only, without warranty of any kind. The user assumes full responsibility for any data loss, device instability, or warranty voiding. This repository contains **no** copyrighted Apple firmware files or piracy tools.

- **gaster:** Copyright (c) 0x7ff (MIT License)
- **PongoOS & KPF:** Copyright (c) palera1n / checkra1n team
- **libimobiledevice & libirecovery:** Copyright (c) libimobiledevice contributors (LGPL-2.1)
- **Zadig / libwdi:** Copyright (c) Pete Batard (LGPL-3.0)

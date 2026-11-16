# Pastico v1.0.1-release (iOS 16.7 - A11 Rootless)

### What's New in v1.0.1

- **New Rootless Jailbreak Workflow (`jb`):** Replaced `boot` with `jb` as the primary jailbreak action, featuring automatic Normal-to-Recovery mode transition and an interactive `palera1n`-style DFU countdown assistant.
- **Safe Mode Support (`--safe-mode`):** Added Safe Mode booting to load the jailbroken environment with tweaks disabled for troubleshooting.
- **Reworked Stock Boot & Restart (`boot` / `reboot`):**
  - `boot` now cleanly exits DFU/Recovery mode and boots standard stock iOS.
  - `reboot` supports restarting connected devices in both Normal Mode and Recovery/DFU Mode.
- **Expanded Device Diagnostics (`deviceinfo`):**
  - Added **ECID (Hex & Decimal)** and **ChipID (`0x8015`)** hardware reporting.
  - Added **Model Number & Region** detection.
  - Added real-time **Battery Level & Charging Status** monitoring.
  - Added automatic **Pastico Compatibility Check** (`A11 + iOS 16.7` verification).
  - Added **`--save`** option to export full hardware and jailbreak reports to `Pastico_Device_Report.txt`.

### Supported Devices & Requirements

- **Hardware:** iPhone 8, iPhone 8 Plus, iPhone X (`A11 Bionic - CPID: 0x8015`)
- **Firmware:** **iOS 16.7 Only** _(16.7.1 and newer are NOT supported)_
- **Host OS:** Windows 10 / Windows 11 (64-bit)
- **Important:** Passcode, Touch ID, and Face ID must be disabled before jailbreaking.

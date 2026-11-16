# Pastico CLI v1.0.1-release (iOS 16.7 - A11 Rootless)

### What's New in v1.0.1

- **New `pastico jb` Command:** Replaced `boot` with `jb` as the primary Rootless jailbreak command.
- **Automatic Normal-to-Recovery Transition:** Automatically detects unlocked iOS devices in Normal Mode and reboots them into Recovery Mode via `ideviceenterrecovery` before starting the DFU sequence.
- **Interactive `palera1n`-Style DFU Assistant:** Added a live step-by-step countdown timer (4s Side + Volume Down -> 10s Volume Down) with real-time `0x8015` DFU hardware detection.
- **Safe Mode Support (`pastico jb --safe-mode`):** Added `--safe-mode` flag to boot with `xargs rootdev=md0 -s`, disabling tweaks for easy troubleshooting.
- **Reworked `pastico boot` & `pastico reboot`:**
  - `pastico boot` now cleanly exits DFU/Recovery mode and boots into stock iOS without jailbreaking.
  - `pastico reboot` now supports restarting devices in both Normal Mode (`idevicediagnostics`) and Recovery/DFU Mode (`irecovery`).
- **Removed Redundant `check` Command:** Streamlined CLI commands by integrating hardware checks directly into the `jb` workflow.

### Requirements

- **Hardware:** iPhone 8, iPhone 8 Plus, iPhone X (`A11 Bionic - CPID: 0x8015`)
- **Firmware:** iOS 16.7 Only (16.7.1+ is not supported)
- **Host OS:** Windows 10 / Windows 11 (x64)
- **Important:** Disable Passcode, Touch ID, and Face ID before running `pastico jb`.

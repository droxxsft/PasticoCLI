#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define VERSION "1.0.1-release (iOS 16.7 - A11 Rootless)"

void set_app_directory() {
    char *exe_path = (char *)calloc(MAX_PATH, sizeof(char));
    if (exe_path) {
        if (GetModuleFileNameA(NULL, exe_path, MAX_PATH) > 0) {
            char *last_slash = strrchr(exe_path, '\\');
            if (last_slash) {
                *last_slash = '\0';
                SetCurrentDirectoryA(exe_path);
            }
        }
        free(exe_path);
    }
}

void print_banner() {
    printf("====================================================\n");
    printf("   PASTICO CLI - iOS 16.7 (A11) Rootless Utility    \n");
    printf("   Powered by checkm8 & PongoOS Architecture        \n");
    printf("====================================================\n\n");
}

void print_help() {
    print_banner();
    printf("Usage: pastico <command> [options]\n\n");
    printf("Commands:\n");
    printf("  jb           Start Rootless Jailbreak (Auto-Recovery & DFU Assistant)\n");
    printf("               (Add --safe-mode to boot with tweaks disabled)\n");
    printf("  boot         Normal boot device from DFU/Recovery without Jailbreak\n");
    printf("  reboot       Restart connected device (Normal or Recovery mode)\n");
    printf("  deviceinfo   Display full device details & Root/Jailbreak status\n");
    printf("  --version    Show version and open-source license info\n");
    printf("  --help       Display this help menu\n\n");
    printf("Requirements:\n");
    printf("  - Hardware : iPhone 8, 8 Plus, X (A11 Bionic - CPID: 0x8015)\n");
    printf("  - Firmware : iOS 16.7 Only\n");
    printf("  - Notice   : Passcode, Touch ID, and Face ID must be disabled.\n");
}

void print_version() {
    printf("Pastico Version: %s\n", VERSION);
    printf("License: MIT License (Includes open-source checkm8 & PongoOS components)\n");
}

bool check_file_exists(const char *path) {
    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
        printf("[ERROR] Missing component: %s not found!\n", path);
        return false;
    }
    return true;
}

bool check_files() {
    bool ok1 = check_file_exists("bin/gaster.exe");
    bool ok2 = check_file_exists("bin/irecovery.exe");
    bool ok3 = check_file_exists("bin/pongoterm.exe");
    bool ok4 = check_file_exists("bin/Pongo.bin");
    bool ok5 = check_file_exists("bin/checkra1n-kpf-pongo");
    bool ok6 = check_file_exists("bin/ramdisk.dmg");
    bool ok7 = check_file_exists("bin/binpack.dmg");
    return ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7;
}

bool check_disclaimer() {
    char *response = (char *)calloc(64, sizeof(char));
    if (!response) return false;
    printf("[LEGAL DISCLAIMER]\n");
    printf("This software is provided 'AS-IS', without any warranty of any kind.\n");
    printf("The user assumes all responsibility for any data loss, software issues,\n");
    printf("or warranty voiding. This tool contains no copyrighted or piracy software.\n\n");
    printf("Do you accept these terms? (yes/no): ");
    bool accepted = false;
    if (fgets(response, 64, stdin) != NULL) {
        response[strcspn(response, "\n")] = 0;
        if (_stricmp(response, "yes") == 0 || _stricmp(response, "y") == 0) accepted = true;
    }
    free(response);
    return accepted;
}

bool is_dfu_connected() {
    return (system("bin\\irecovery.exe -q 2>nul | findstr /I \"DFU\" > nul 2>&1") == 0 &&
            system("bin\\irecovery.exe -q 2>nul | findstr /I \"0x8015\" > nul 2>&1") == 0);
}

void auto_enter_recovery_if_normal() {
    if (GetFileAttributesA("bin/ideviceinfo.exe") == INVALID_FILE_ATTRIBUTES ||
        GetFileAttributesA("bin/ideviceenterrecovery.exe") == INVALID_FILE_ATTRIBUTES) {
        return;
    }

    FILE *fp = _popen("bin\\ideviceinfo.exe -k UniqueDeviceID 2>nul", "r");
    char *buf = (char *)calloc(128, sizeof(char));
    char *udid = (char *)calloc(128, sizeof(char));
    if (fp && buf && udid) {
        while (fgets(buf, 128, fp) != NULL) {
            buf[strcspn(buf, "\r\n")] = 0;
            if (strlen(buf) > 10) strncpy(udid, buf, 127);
        }
        _pclose(fp);
    }

    if (udid && strlen(udid) > 10) {
        printf("\n[*] Connected iPhone detected in Normal Mode (UDID: %s).\n", udid);
        printf("[*] Press ENTER to automatically reboot your device into Recovery Mode...");
        getchar();

        char *cmd = (char *)calloc(256, sizeof(char));
        if (cmd) {
            snprintf(cmd, 256, "bin\\ideviceenterrecovery.exe %s > nul 2>&1", udid);
            printf("[*] Entering Recovery Mode, please wait...\n");
            system(cmd);
            free(cmd);
            Sleep(4000);
        }
    }
    free(buf);
    free(udid);
}

bool interactive_dfu_assistant() {
    if (is_dfu_connected()) {
        printf("[+] A11 Device (0x8015) already in DFU Mode!\n");
        return true;
    }

    auto_enter_recovery_if_normal();

    if (is_dfu_connected()) return true;

    printf("\n====================================================\n");
    printf("           INTERACTIVE DFU MODE ASSISTANT           \n");
    printf("====================================================\n");
    printf("Get ready to put your iPhone 8 / 8 Plus / X into DFU:\n");
    printf("  1. Keep your device connected via USB.\n");
    printf("  2. When ready, press ENTER to begin the countdown.\n");
    printf("----------------------------------------------------\n");
    printf("Press ENTER when you are ready...");
    getchar();

    printf("\n[STEP 1/2] Hold BOTH Side (Power) + Volume Down buttons together (4 seconds):\n");
    for (int i = 4; i > 0; i--) {
        printf("\r   -> Keep holding both buttons... %d sec remaining  ", i);
        fflush(stdout);
        Sleep(1000);
    }

    printf("\n\n[STEP 2/2] RELEASE Side Button NOW, keep holding ONLY Volume Down (10 seconds):\n");
    for (int i = 10; i > 0; i--) {
        printf("\r   -> Holding Volume Down... %d sec remaining (Waiting for DFU)  ", i);
        fflush(stdout);
        Sleep(1000);
        if (is_dfu_connected()) {
            printf("\n\n[+] Success! A11 Device (0x8015) entered DFU Mode!\n");
            return true;
        }
    }

    printf("\n\n[ERROR] DFU Mode timed out or device not recognized.\n");
    printf("[*] Hint: If Windows has not installed the DFU driver yet, select\n");
    printf("    'Apple Mobile Device (DFU Mode)' in Zadig and install 'libusbK'.\n");
    if (GetFileAttributesA("bin/zadig.exe") != INVALID_FILE_ATTRIBUTES) {
        printf("[*] Launching bundled Zadig utility...\n");
        system("start bin\\zadig.exe");
    }
    return false;
}

void print_device_key(const char *label, const char *key) {
    char *cmd = (char *)calloc(256, sizeof(char));
    char *buf = (char *)calloc(256, sizeof(char));
    char *val = (char *)calloc(256, sizeof(char));
    if (!cmd || !buf || !val) { free(cmd); free(buf); free(val); return; }
    snprintf(cmd, 256, "bin\\ideviceinfo.exe -k %s 2>nul", key);
    FILE *fp = _popen(cmd, "r");
    printf("%-18s : ", label);
    if (fp != NULL) {
        while (fgets(buf, 256, fp) != NULL) {
            buf[strcspn(buf, "\r\n")] = 0;
            if (strlen(buf) > 0) strncpy(val, buf, 255);
        }
        _pclose(fp);
    }
    printf("%s\n", strlen(val) > 0 ? val : "N/A");
    free(cmd); free(buf); free(val);
}
void cmd_deviceinfo() {
    print_banner();
    if (GetFileAttributesA("bin/ideviceinfo.exe") == INVALID_FILE_ATTRIBUTES) {
        printf("[ERROR] bin/ideviceinfo.exe not found!\n"); return;
    }
    printf("[*] Querying connected iOS device (Normal Mode)...\n");
    printf("----------------------------------------------------\n");
    FILE *test_fp = _popen("bin\\ideviceinfo.exe -k DeviceName 2>nul", "r");
    char *test_buf = (char *)calloc(128, sizeof(char));
    bool connected = false;
    if (test_fp != NULL && test_buf != NULL) {
        while (fgets(test_buf, 128, test_fp) != NULL) {
            if (strlen(test_buf) > 1) connected = true;
        }
        _pclose(test_fp);
    }
    free(test_buf);
    if (!connected) {
        printf("[ERROR] No iOS device detected! Unlock iPhone & tap Trust.\n"); return;
    }
    print_device_key("Device Name", "DeviceName");
    print_device_key("Product Type", "ProductType");
    print_device_key("Hardware Model", "HardwareModel");
    print_device_key("CPU Architecture", "CPUArchitecture");
    print_device_key("iOS Version", "ProductVersion");
    print_device_key("Build Version", "BuildVersion");
    print_device_key("Serial Number", "SerialNumber");
    print_device_key("UDID", "UniqueDeviceID");
    print_device_key("Activation State", "ActivationState");
    printf("----------------------------------------------------\n");
    printf("[*] Analyzing Root Filesystem & Jailbreak Status...\n");
    bool is_jb = false, root_ok = false, var_jb = false, is_rootful = false;
    bool sileo = false, zebra = false, cydia = false, palera1n = false, filza = false, term = false;

    if (GetFileAttributesA("bin/afcclient.exe") != INVALID_FILE_ATTRIBUTES) {
        FILE *afp = _popen("bin\\afcclient.exe --afc2 ls / 2>nul", "r");
        if (afp) {
            char *al = (char *)calloc(256, sizeof(char));
            while (al && fgets(al, 256, afp)) {
                if (strstr(al, "Applications") || strstr(al, "System")) { root_ok = true; is_jb = true; }
            }
            free(al); _pclose(afp);
        }
        if (root_ok) {
            FILE *jfp = _popen("bin\\afcclient.exe --afc2 ls /var 2>nul", "r");
            if (jfp) {
                char *jl = (char *)calloc(256, sizeof(char));
                while (jl && fgets(jl, 256, jfp)) { if (strstr(jl, "jb")) var_jb = true; }
                free(jl); _pclose(jfp);
            }
        }
    }
    if (GetFileAttributesA("bin/ideviceinstaller.exe") != INVALID_FILE_ATTRIBUTES) {
        FILE *fp = _popen("bin\\ideviceinstaller.exe list --all 2>nul", "r");
        if (fp) {
            char *ln = (char *)calloc(512, sizeof(char));
            while (ln && fgets(ln, 512, fp)) {
                if (strstr(ln, "Sileo")) { sileo = true; is_jb = true; }
                if (strstr(ln, "Zebra")) { zebra = true; is_jb = true; }
                if (strstr(ln, "Cydia")) { cydia = true; is_jb = true; is_rootful = true; }
                if (strstr(ln, "in.palera.loader") || strstr(ln, "palera1n")) { palera1n = true; is_jb = true; }
                if (strstr(ln, "Filza")) { filza = true; is_jb = true; }
                if (strstr(ln, "NewTerm") || strstr(ln, "ws.hbang.Terminal")) { term = true; is_jb = true; }
            }
            free(ln); _pclose(fp);
        }
    }
    if (root_ok && !var_jb) is_rootful = true;
    printf("Root (/) Access    : %s\n", root_ok ? "GRANTED (AFC2 Active)" : "RESTRICTED (Jailed / No AFC2)");
    if (is_jb) {
        printf("Jailbreak Status   : YES (Jailbroken)\n");
        printf("Jailbreak Type     : %s\n", is_rootful ? "Rootful (FakeFS / R/W Root)" : "Rootless (/var/jb Architecture)");
        printf("Detected Tools     : ");
        if (palera1n) printf("[palera1n] ");
        if (sileo) printf("[Sileo] ");
        if (zebra) printf("[Zebra] ");
        if (cydia) printf("[Cydia] ");
        if (filza) printf("[Filza] ");
        if (term) printf("[NewTerm] ");
        printf("\n");
    } else {
        printf("Jailbreak Status   : NO (Stock / Unjailbroken)\n");
        printf("Jailbreak Type     : N/A\n");
    }
    printf("====================================================\n");
}

void cmd_jb(bool safe_mode) {
    print_banner();
    if (!check_disclaimer()) { printf("\n[!] Aborted by user.\n"); return; }

    printf("\n[1/8] Verifying system & userland binaries...\n");
    if (!check_files()) return;

    printf("[2/8] Preparing A11 Bionic device (CPID: 0x8015) for DFU mode...\n");
    if (!interactive_dfu_assistant()) return;

    printf("[3/8] Triggering checkm8 BootROM exploit (gaster pwn)...\n");
    if (system("bin\\gaster.exe pwn") != 0) { printf("[ERROR] Exploit failed.\n"); return; }

    printf("[4/8] Resetting USB configuration...\n");
    system("bin\\gaster.exe reset");
    Sleep(1000);

    printf("[5/8] Booting PongoOS kernel...\n");
    system("bin\\irecovery.exe -f bin\\Pongo.bin");
    Sleep(3000);

    printf("[6/8] Injecting iOS 16.7 Kernel Patchfinder (KPF) via PongoOS USB...\n");
    if (system("bin\\pongoterm.exe -f bin\\checkra1n-kpf-pongo") != 0) {
        printf("[!] PongoOS USB driver missing! Select 'PongoOS USB Device' in Zadig and install 'libusbK'.\n");
        if (GetFileAttributesA("bin/zadig.exe") != INVALID_FILE_ATTRIBUTES) {
            system("start bin\\zadig.exe");
        }
        printf("[*] Press ENTER once the PongoOS driver is installed to continue...");
        getchar();
        system("bin\\pongoterm.exe -f bin\\checkra1n-kpf-pongo");
    }
    system("bin\\pongoterm.exe -c \"modload\"");

    printf("[7/8] Uploading Rootless Ramdisk & Userland Overlay...\n");
    system("bin\\pongoterm.exe -f bin\\ramdisk.dmg");
    system("bin\\pongoterm.exe -c \"ramdisk\"");
    system("bin\\pongoterm.exe -f bin\\binpack.dmg");
    system("bin\\pongoterm.exe -c \"overlay\"");

    printf("[8/8] Setting boot-args and booting iOS 16.7...\n");
    if (safe_mode) {
        printf("[*] Safe Mode enabled: Tweaks will be disabled on boot.\n");
        system("bin\\pongoterm.exe -c \"xargs rootdev=md0 -s\"");
    } else {
        system("bin\\pongoterm.exe -c \"xargs rootdev=md0\"");
    }
    system("bin\\pongoterm.exe -c \"bootx\"");

    printf("\n[+] Rootless Jailbreak boot sequence completed!\n");
    printf("[*] Once booted, open the Loader app on your Home Screen to install Sileo.\n");
}

void cmd_boot_normal() {
    printf("[*] Booting connected device normally (Stock iOS - No Jailbreak)...\n");
    if (system("bin\\irecovery.exe -q > nul 2>&1") == 0) {
        system("bin\\irecovery.exe -n > nul 2>&1");
        system("bin\\irecovery.exe -c \"setenv auto-boot true\" > nul 2>&1");
        system("bin\\irecovery.exe -c \"saveenv\" > nul 2>&1");
        system("bin\\irecovery.exe -c \"reboot\"");
        printf("[+] Normal boot signal sent to device.\n");
    } else {
        printf("[ERROR] No device detected in DFU or Recovery mode.\n");
        printf("[*] Hint: If your device is already powered on, use 'pastico reboot' to restart it.\n");
    }
}

void cmd_reboot() {
    printf("[*] Attempting to restart connected device...\n");
    if (GetFileAttributesA("bin/idevicediagnostics.exe") != INVALID_FILE_ATTRIBUTES) {
        if (system("bin\\idevicediagnostics.exe restart > nul 2>&1") == 0) {
            printf("[+] Restart command sent to iOS device (Normal Mode).\n");
            return;
        }
    }
    if (system("bin\\irecovery.exe -q > nul 2>&1") == 0) {
        system("bin\\irecovery.exe -c \"reboot\"");
        printf("[+] Reboot command sent to device (Recovery/DFU Mode).\n");
        return;
    }
    printf("[ERROR] No active iOS device found to restart.\n");
    printf("[*] Hint: Unlock your device and trust this computer, or ensure bin\\idevicediagnostics.exe is present.\n");
}

int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8);
    set_app_directory();
    if (argc < 2) { print_help(); return 0; }
    char *cmd = *(argv + 1);
    bool safe_mode = (argc >= 3 && strcmp(*(argv + 2), "--safe-mode") == 0);

    if (!strcmp(cmd, "--help") || !strcmp(cmd, "-h")) print_help();
    else if (!strcmp(cmd, "--version") || !strcmp(cmd, "-v")) print_version();
    else if (!strcmp(cmd, "deviceinfo")) cmd_deviceinfo();
    else if (!strcmp(cmd, "jb")) cmd_jb(safe_mode);
    else if (!strcmp(cmd, "boot")) cmd_boot_normal();
    else if (!strcmp(cmd, "reboot")) cmd_reboot();
    else { printf("[Error] Unknown command: '%s'\nRun 'pastico --help' for usage.\n", cmd); return 1; }
    return 0;
}
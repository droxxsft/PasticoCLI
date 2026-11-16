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
            if (last_slash) { *last_slash = '\0'; SetCurrentDirectoryA(exe_path); }
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
    printf("  deviceinfo   Display full hardware, battery & Jailbreak diagnostics\n");
    printf("               (Add --save to export report to Pastico_Device_Report.txt)\n");
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
    return check_file_exists("bin/gaster.exe") && check_file_exists("bin/irecovery.exe") &&
           check_file_exists("bin/pongoterm.exe") && check_file_exists("bin/Pongo.bin") &&
           check_file_exists("bin/checkra1n-kpf-pongo") && check_file_exists("bin/ramdisk.dmg") &&
           check_file_exists("bin/binpack.dmg");
}

bool check_disclaimer() {
    char *res = (char *)calloc(64, sizeof(char));
    if (!res) return false;
    printf("[LEGAL DISCLAIMER]\nThis software is provided 'AS-IS', without any warranty.\n");
    printf("The user assumes all responsibility. Do you accept? (yes/no): ");
    bool ok = false;
    if (fgets(res, 64, stdin) != NULL) {
        res[strcspn(res, "\n")] = 0;
        if (_stricmp(res, "yes") == 0 || _stricmp(res, "y") == 0) ok = true;
    }
    free(res);
    return ok;
}
bool is_dfu_connected() {
    return (system("bin\\irecovery.exe -q 2>nul | findstr /I \"DFU\" > nul 2>&1") == 0 &&
            system("bin\\irecovery.exe -q 2>nul | findstr /I \"0x8015\" > nul 2>&1") == 0);
}

void auto_enter_recovery_if_normal() {
    if (GetFileAttributesA("bin/ideviceinfo.exe") == INVALID_FILE_ATTRIBUTES ||
        GetFileAttributesA("bin/ideviceenterrecovery.exe") == INVALID_FILE_ATTRIBUTES) return;
    FILE *fp = _popen("bin\\ideviceinfo.exe -k UniqueDeviceID 2>nul", "r");
    char *buf = (char *)calloc(128, 1), *udid = (char *)calloc(128, 1);
    if (fp && buf && udid) {
        while (fgets(buf, 128, fp)) { buf[strcspn(buf, "\r\n")] = 0; if (strlen(buf) > 10) strncpy(udid, buf, 127); }
        _pclose(fp);
    }
    if (udid && strlen(udid) > 10) {
        printf("\n[*] Connected iPhone detected in Normal Mode (UDID: %s).\n", udid);
        printf("[*] Press ENTER to automatically reboot your device into Recovery Mode...");
        getchar();
        char *cmd = (char *)calloc(256, 1);
        if (cmd) { snprintf(cmd, 256, "bin\\ideviceenterrecovery.exe %s > nul 2>&1", udid); system(cmd); free(cmd); Sleep(4000); }
    }
    free(buf); free(udid);
}

bool interactive_dfu_assistant() {
    if (is_dfu_connected()) { printf("[+] A11 Device (0x8015) already in DFU Mode!\n"); return true; }
    auto_enter_recovery_if_normal();
    if (is_dfu_connected()) return true;
    printf("\n=== INTERACTIVE DFU MODE ASSISTANT ===\nPress ENTER when ready to start countdown...");
    getchar();
    printf("\n[STEP 1/2] Hold BOTH Side + Volume Down buttons together (4s):\n");
    for (int i = 4; i > 0; i--) { printf("\r   -> Keep holding both buttons... %d sec  ", i); fflush(stdout); Sleep(1000); }
    printf("\n\n[STEP 2/2] RELEASE Side Button NOW, keep holding ONLY Volume Down (10s):\n");
    for (int i = 10; i > 0; i--) {
        printf("\r   -> Holding Volume Down... %d sec (Waiting for DFU)  ", i); fflush(stdout); Sleep(1000);
        if (is_dfu_connected()) { printf("\n\n[+] Success! A11 Device (0x8015) entered DFU Mode!\n"); return true; }
    }
    printf("\n\n[ERROR] DFU Mode timed out. Install 'libusbK' via Zadig.\n");
    if (GetFileAttributesA("bin/zadig.exe") != INVALID_FILE_ATTRIBUTES) system("start bin\\zadig.exe");
    return false;
}

char *get_device_val(const char *domain, const char *key) {
    char *cmd = (char *)calloc(256, 1), *buf = (char *)calloc(256, 1), *val = (char *)calloc(256, 1);
    if (!val) { free(cmd); free(buf); return NULL; }
    strcpy(val, "N/A");
    if (!cmd || !buf) { free(cmd); free(buf); return val; }
    if (domain) snprintf(cmd, 256, "bin\\ideviceinfo.exe -q %s -k %s 2>nul", domain, key);
    else snprintf(cmd, 256, "bin\\ideviceinfo.exe -k %s 2>nul", key);
    FILE *fp = _popen(cmd, "r");
    if (fp) {
        while (fgets(buf, 256, fp)) {
            buf[strcspn(buf, "\r\n")] = 0;
            if (strlen(buf) > 0) { strncpy(val, buf, 255); *(val + 255) = '\0'; }
        }
        _pclose(fp);
    }
    free(cmd); free(buf);
    return val;
}
void cmd_deviceinfo(bool save_report, const char *orig_dir) {
    print_banner();
    if (GetFileAttributesA("bin/ideviceinfo.exe") == INVALID_FILE_ATTRIBUTES) { printf("[ERROR] bin/ideviceinfo.exe not found!\n"); return; }
    char *dev_name = get_device_val(NULL, "DeviceName");
    if (!dev_name || strcmp(dev_name, "N/A") == 0) { printf("[ERROR] No iOS device detected! Unlock iPhone & tap Trust.\n"); free(dev_name); return; }

    char *prod_type = get_device_val(NULL, "ProductType");
    char *hw_model = get_device_val(NULL, "HardwareModel");
    char *cpu_arch = get_device_val(NULL, "CPUArchitecture");
    char *ios_ver = get_device_val(NULL, "ProductVersion");
    char *build_ver = get_device_val(NULL, "BuildVersion");
    char *serial = get_device_val(NULL, "SerialNumber");
    char *udid = get_device_val(NULL, "UniqueDeviceID");
    char *act_state = get_device_val(NULL, "ActivationState");
    char *model_num = get_device_val(NULL, "ModelNumber");
    char *region = get_device_val(NULL, "RegionInfo");
    char *chip_id = get_device_val(NULL, "ChipID");
    char *ecid_dec = get_device_val(NULL, "UniqueChipID");
    char *bat_cap = get_device_val("com.apple.mobile.battery", "BatteryCurrentCapacity");
    char *bat_chg = get_device_val("com.apple.mobile.battery", "BatteryIsCharging");

    unsigned long long ecid_num = strtoull(ecid_dec, NULL, 10);
    int chip_num = atoi(chip_id);
    bool is_a11 = (chip_num == 32789 || strstr(chip_id, "8015") != NULL);
    bool is_167 = (strcmp(ios_ver, "16.7") == 0);

    printf("Device Name        : %s\nProduct Type       : %s\nHardware Model     : %s\n", dev_name, prod_type, hw_model);
    printf("Model / Region     : %s (%s)\n", model_num, region);
    printf("CPU Architecture   : %s (ChipID: 0x%X%s)\n", cpu_arch, chip_num, is_a11 ? " - A11 Bionic" : "");
    printf("ECID (Hex / Dec)   : 0x%llX / %s\n", ecid_num, ecid_dec);
    printf("iOS Version        : %s (Build: %s)\nSerial Number      : %s\nUDID               : %s\n", ios_ver, build_ver, serial, udid);
    printf("Activation State   : %s\nBattery Level      : %s%% (Charging: %s)\n", act_state, bat_cap, strcmp(bat_chg, "true") == 0 ? "Yes" : "No");
    printf("Pastico Support    : %s\n", (is_a11 && is_167) ? "SUPPORTED (Ready for Rootless JB)" : "UNSUPPORTED (Requires A11 & iOS 16.7)");
    printf("----------------------------------------------------\n");

    bool is_jb = false, is_rootful = false, sileo = false, zebra = false, cydia = false, palera1n = false, filza = false, term = false;
    if (GetFileAttributesA("bin/ideviceinstaller.exe") != INVALID_FILE_ATTRIBUTES) {
        FILE *fp = _popen("bin\\ideviceinstaller.exe list --all 2>nul", "r");
        if (fp) {
            char *ln = (char *)calloc(512, 1);
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
    if (is_jb) {
        printf("Jailbreak Status   : YES (Jailbroken)\nJailbreak Type     : %s\nDetected Tools     : ", is_rootful ? "Rootful" : "Rootless (/var/jb)");
        if (palera1n) printf("[palera1n] "); if (sileo) printf("[Sileo] "); if (zebra) printf("[Zebra] ");
        if (cydia) printf("[Cydia] "); if (filza) printf("[Filza] "); if (term) printf("[NewTerm] ");
        printf("\n");
    } else printf("Jailbreak Status   : NO (Stock / Unjailbroken)\nJailbreak Type     : N/A\n");
    printf("====================================================\n");

    if (save_report && orig_dir) {
        char *rp = (char *)calloc(MAX_PATH, 1);
        if (rp) {
            snprintf(rp, MAX_PATH, "%s\\Pastico_Device_Report.txt", orig_dir);
            FILE *rf = fopen(rp, "w");
            if (rf) {
                fprintf(rf, "Device: %s (%s) | iOS %s (%s)\nECID: 0x%llX | Serial: %s | UDID: %s\nBattery: %s%% | JB: %s\n",
                        dev_name, prod_type, ios_ver, build_ver, ecid_num, serial, udid, bat_cap, is_jb ? "YES" : "NO");
                fclose(rf); printf("[+] Report saved to: %s\n", rp);
            }
            free(rp);
        }
    }
    free(dev_name); free(prod_type); free(hw_model); free(cpu_arch); free(ios_ver); free(build_ver);
    free(serial); free(udid); free(act_state); free(model_num); free(region); free(chip_id); free(ecid_dec); free(bat_cap); free(bat_chg);
}

void cmd_jb(bool safe_mode) {
    print_banner();
    if (!check_disclaimer() || !check_files() || !interactive_dfu_assistant()) return;
    if (system("bin\\gaster.exe pwn") != 0) { printf("[ERROR] Exploit failed.\n"); return; }
    system("bin\\gaster.exe reset"); Sleep(1000);
    system("bin\\irecovery.exe -f bin\\Pongo.bin"); Sleep(3000);
    if (system("bin\\pongoterm.exe -f bin\\checkra1n-kpf-pongo") != 0) {
        printf("[!] PongoOS USB driver missing! Install 'libusbK' in Zadig and press ENTER...");
        if (GetFileAttributesA("bin/zadig.exe") != INVALID_FILE_ATTRIBUTES) system("start bin\\zadig.exe");
        getchar(); system("bin\\pongoterm.exe -f bin\\checkra1n-kpf-pongo");
    }
    system("bin\\pongoterm.exe -c \"modload\"");
    system("bin\\pongoterm.exe -f bin\\ramdisk.dmg"); system("bin\\pongoterm.exe -c \"ramdisk\"");
    system("bin\\pongoterm.exe -f bin\\binpack.dmg"); system("bin\\pongoterm.exe -c \"overlay\"");
    system(safe_mode ? "bin\\pongoterm.exe -c \"xargs rootdev=md0 -s\"" : "bin\\pongoterm.exe -c \"xargs rootdev=md0\"");
    system("bin\\pongoterm.exe -c \"bootx\"");
    printf("\n[+] Rootless Jailbreak boot sequence completed!\n");
}

void cmd_boot_normal() {
    if (system("bin\\irecovery.exe -q > nul 2>&1") == 0) {
        system("bin\\irecovery.exe -n > nul 2>&1");
        system("bin\\irecovery.exe -c \"setenv auto-boot true\" > nul 2>&1");
        system("bin\\irecovery.exe -c \"saveenv\" > nul 2>&1");
        system("bin\\irecovery.exe -c \"reboot\"");
        printf("[+] Normal boot signal sent.\n");
    } else printf("[ERROR] No device in DFU/Recovery mode.\n");
}

void cmd_reboot() {
    if (GetFileAttributesA("bin/idevicediagnostics.exe") != INVALID_FILE_ATTRIBUTES &&
        system("bin\\idevicediagnostics.exe restart > nul 2>&1") == 0) {
        printf("[+] Restart command sent (Normal Mode).\n"); return;
    }
    if (system("bin\\irecovery.exe -q > nul 2>&1") == 0) {
        system("bin\\irecovery.exe -c \"reboot\""); printf("[+] Reboot command sent (Recovery/DFU).\n"); return;
    }
    printf("[ERROR] No active iOS device found.\n");
}

int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8);
    char *orig_dir = (char *)calloc(MAX_PATH, 1);
    if (orig_dir) GetCurrentDirectoryA(MAX_PATH, orig_dir);
    set_app_directory();
    if (argc < 2) { print_help(); free(orig_dir); return 0; }
    char *cmd = *(argv + 1);
    bool flag_arg = (argc >= 3);
    if (!strcmp(cmd, "--help") || !strcmp(cmd, "-h")) print_help();
    else if (!strcmp(cmd, "--version") || !strcmp(cmd, "-v")) print_version();
    else if (!strcmp(cmd, "deviceinfo")) cmd_deviceinfo(flag_arg && strcmp(*(argv + 2), "--save") == 0, orig_dir);
    else if (!strcmp(cmd, "jb")) cmd_jb(flag_arg && strcmp(*(argv + 2), "--safe-mode") == 0);
    else if (!strcmp(cmd, "boot")) cmd_boot_normal();
    else if (!strcmp(cmd, "reboot")) cmd_reboot();
    else { printf("[Error] Unknown command: '%s'\n", cmd); free(orig_dir); return 1; }
    free(orig_dir);
    return 0;
}

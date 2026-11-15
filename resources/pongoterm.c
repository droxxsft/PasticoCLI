#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libusb.h>

#define PONGO_VID 0x05AC
#define PONGO_PID 0x4141

int send_file(libusb_device_handle *dev, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) { printf("[ERROR] Cannot open %s\n", filename); return -1; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buf = (unsigned char *)malloc(size);
    if (!buf) { fclose(f); return -1; }
    fread(buf, 1, size, f);
    fclose(f);

    uint32_t sz = (uint32_t)size;
    libusb_control_transfer(dev, 0x21, 2, 0, 0, NULL, 0, 1000);
    libusb_control_transfer(dev, 0x21, 1, 0, 0, (unsigned char *)&sz, 4, 5000);

    long sent = 0;
    while (sent < size) {
        int chunk = (size - sent > 0x8000) ? 0x8000 : (int)(size - sent);
        int transferred = 0;
        int r = libusb_bulk_transfer(dev, 0x02, buf + sent, chunk, &transferred, 5000);
        if (r < 0) { free(buf); return r; }
        sent += transferred;
    }
    free(buf);
    printf("[+] Uploaded %s (%ld bytes)\n", filename, size);
    return 0;
}

int send_cmd(libusb_device_handle *dev, const char *cmd) {
    char *cmd_buf = (char *)calloc(512, sizeof(char));
    if (!cmd_buf) return -1;
    snprintf(cmd_buf, 512, "%s\n", cmd);
    int r = libusb_control_transfer(dev, 0x21, 3, 0, 0, (unsigned char *)cmd_buf, (uint16_t)strlen(cmd_buf), 5000);
    free(cmd_buf);
    if (r < 0) return r;
    printf("[+] Executed PongoOS command: %s\n", cmd);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: pongoterm -f <file> | -c <command>\n");
        return 1;
    }
    char *mode = *(argv + 1);
    char *arg = *(argv + 2);

    libusb_init(NULL);
    libusb_device_handle *dev = libusb_open_device_with_vid_pid(NULL, PONGO_VID, PONGO_PID);
    if (!dev) {
        printf("[ERROR] PongoOS USB device (05AC:4141) not found!\n");
        libusb_exit(NULL);
        return 1;
    }
    libusb_claim_interface(dev, 0);

    int res = 0;
    if (strcmp(mode, "-f") == 0) res = send_file(dev, arg);
    else if (strcmp(mode, "-c") == 0) res = send_cmd(dev, arg);

    libusb_release_interface(dev, 0);
    libusb_close(dev);
    libusb_exit(NULL);
    return res < 0 ? 1 : 0;
}
import struct
import zlib

with open("palera1n-linux-x86_64", "rb") as f:
    data = f.read()

koly_pos = data.find(b"koly")
if koly_pos != -1:
    trailer = data[koly_pos : koly_pos + 512]
    xml_offset, = struct.unpack(">Q", trailer[216:224])
    xml_length, = struct.unpack(">Q", trailer[224:232])
    
    exact_dmg_size = xml_offset + xml_length + 512
    dmg_end = koly_pos + 512
    dmg_start = dmg_end - exact_dmg_size
    
    with open("bin/binpack.dmg", "wb") as out:
        out.write(data[dmg_start:dmg_end])
    print(f"[+] binpack.dmg Is OK! Byte: {exact_dmg_size} bayt (Baslangic: {dmg_start})")

rd_start = 4667140
try:
    rd_data = zlib.decompress(data[rd_start:], -15)
    with open("bin/ramdisk.dmg", "wb") as out:
        out.write(rd_data)
    print(f"[+] ramdisk.dmg Opened! Byte: {len(rd_data)} bayt")
except Exception:
    with open("bin/ramdisk.dmg", "wb") as out:
        out.write(data[rd_start:])
    print(f"[+] ramdisk.img Success Saved! Byte: {len(data) - rd_start} bayt")
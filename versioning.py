firmware_info_filename  = "..\\..\\..\\..\\firmware.nfo"
version_h_filename      = "..\\..\\..\\..\\src\\version.h"
build_define            = "FIRMWARE_BUILD"

def get_version(filename):
    try:
        with open(str(filename), "r") as f:
            counter = int(f.readline())
            f.close()
    except FileNotFoundError:
            counter = 0
    return counter

def update_version(filename, version):
    f = open(filename, "w")
    f.write(str(version))
    f.close()

def make_version_h(filename, version):
    f = open(filename, "w")
    f.write("#pragma once\n")
    f.write("\n")
    line = "#define " + build_define + " " + str(version) + "\n"
    f.write(line)
    f.close()



version = get_version(firmware_info_filename)+1
make_version_h(version_h_filename, version)
update_version(firmware_info_filename, version)

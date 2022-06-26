build_version_filename = "..\\..\\..\\..\\versioning.txt"
version_h_filename     = "..\\..\\..\\..\\src\\version.h"
build_define = "FIRMWARE_BUILD"

def get_version():
    try:
        with open(str(build_version_filename), "r") as f:
            counter = int(f.readline())
            f.close()
    except FileNotFoundError:
            counter = 1
    return counter

def update_version(version):
    filename = build_version_filename
    f = open(filename, "w")
    f.write(str(version))
    f.close()

def make_version_h(version):
    filename = version_h_filename
    f = open(filename, "w")
    f.write("#pragma once\n")
    f.write("\n")
    line = "#define " + build_define + " " + str(version) + "\n"
    f.write(line)
    f.close()



version = get_version()
make_version_h(version)
update_version(version+1)

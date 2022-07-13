import shutil
import os
env = DefaultEnvironment()

#firmware_info_filename  = "..\\..\\..\\..\\firmware\\firmware.nfo"
#version_h_filename      = "..\\..\\..\\..\\src\\version.h"
firmware_info_filename   = "../../../../firmware/firmware.nfo"
version_h_filename       = "../../../../src/version.h"
firmware_bin_filename    = "../../../../firmware/firmware.bin"
firmware_bin_destination = "./firmware/firmware.bin"
build_define             = "FIRMWARE_BUILD"

def get_version(filename):
    try:
        with open(str(filename), "r") as f:
            counter = int(f.readline())
            f.close()
    except FileNotFoundError:
            counter = 0
    return counter


def update_version(filename, version):
    os.makedirs(os.path.dirname(filename), exist_ok=True)
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

def post_build(source, target, env):
    src_path = str(source[0])
    dst_path = firmware_bin_destination
    version_str = str(get_version(firmware_info_filename))
    print("post_build() called, target:", src_path)
    os.makedirs(os.path.dirname(dst_path), exist_ok=True)
    shutil.copyfile(src_path, dst_path)

if os.path.exists(firmware_bin_filename):
    os.remove(firmware_bin_filename)

version = get_version(firmware_info_filename)+1
make_version_h(version_h_filename, version)
update_version(firmware_info_filename, version)

env.AddPostAction("buildprog", post_build)
env.AddPostAction("upload", post_build)

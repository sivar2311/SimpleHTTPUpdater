Import("env", "projenv")

import shutil
import os

build_version_filename = "versioning.txt"
build_define = "#define FIRMWARE_BUILD"

def get_version():
    try:
        with open(str(".\\"+build_version_filename), "r") as f:
            counter = int(f.readline())
            f.close()
    except FileNotFoundError:
            counter = 1
    

    return counter

def update_version(version):
    with open(build_version_filename, "w") as f:
        f.write(str(version))
        f.close()


def make_version_h(version):

    filename = ".\\src\\version.h"
    f = open(filename, "w")
    f.write("#pragma once\n")
    f.write("\n")
    line = build_define + " " + str(version) + "\n"
    f.write(line)
    f.close()

def move_firmware(platform):
    file_name = "firmware.bin"
    src_path = ".\\.pio\\build\\" + platform
    dst_path = ".\\firmware"
    src_file =  src_path + "\\" + file_name
    dst_file =  dst_path + "\\" + file_name

    if not os.path.exists(dst_path):
        os.mkdir(dst_path)

    shutil.copyfile(src_file, dst_file)

def make_firmware_info(version):
    f = open(".\\firmware\\firmware.nfo", "w")
    f.write(str(version))
    f.close()

def after_build(source, target, env):
    platform = env["PIOENV"]

    version = get_version()
    move_firmware(platform)
    make_firmware_info(version)
    update_version(version+1)

def before_build():
    version = get_version()
    make_version_h(version)


before_build()

env.AddPostAction("buildprog", after_build)


"""
    This file is a part of yoyoengine. (https://github.com/yoyolick/yoyoengine)
    Copyright (C) 2023  Ryan Zmuda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
"""

"""
    Script to assist in building a launcher game.
    Generates a CMakeLists.txt and runs the build commands.
"""

import os
import sys
import shutil
import subprocess
import json

# set some variables
script_version = "1.0.0"

# Get the current directory
current_dir = os.getcwd()

print("----------------------------------")
print("Yoyo Engine Build Script v" + script_version)
print("Ryan Zmuda 2023")
print("----------------------------------")

print("Working directory:\n" + current_dir)

# open settings.yoyo (json) and build.yoyo (json) and extract their contents so we can read them
settings_file = open("settings.yoyo", "r")
settings = json.load(settings_file)
settings_file.close()

build_file = open("build.yoyo", "r")
build = json.load(build_file)
build_file.close()

# Get details on the build and game (game name spaces->underscores)
game_name = settings["name"].replace(" ", "_")

build_platform = build["platform"]
build_cflags = build["cflags"]
build_engine_path = build["engine_build_path"]

# Check if the engine build path exists
if not os.path.exists(build_engine_path):
    print("Error: Engine build path \"" + build_engine_path + "\" does not exist.")
    print("Please set the engine build path in build.yoyo to the path of the engine build folder you want to use.")
    sys.exit()

# Create a build folder
if os.path.exists("./build"):
    shutil.rmtree("./build")
os.mkdir("./build")

# Populate our build with the engine build and custom files from the project.
# (THIS DOES NOT INCLUDE SOURCE FILES FROM THE PROJECT)
shutil.copytree(build_engine_path, "./build/" + build_platform)
shutil.copytree("./custom/include", "./build/" + build_platform + "/include", dirs_exist_ok=True)
shutil.copytree("./custom/lib", "./build/" + build_platform + "/lib", dirs_exist_ok=True)
shutil.copytree("./resources", "./build/" + build_platform + "/resources", dirs_exist_ok=True)
shutil.copyfile("./settings.yoyo", "./build/" + build_platform + "/settings.yoyo")

print("Building \"" + game_name + "\" for " + build_platform + " with flags: " + build_cflags)

# create a CMakeLists.txt file in that folder
cmake_file = open("./build/CMakeLists.txt", "w")

# write to that file the CMakeLists.txt template
cmake_file.write("cmake_minimum_required(VERSION 3.27.2)\n")

cmake_file.write("project(" + game_name + ")\n")

cmake_file.write("set(CMAKE_C_FLAGS \"${CMAKE_C_FLAGS} "+build_cflags+"\")\n")

cmake_file.write("set(SOURCES \""+current_dir+"/entry.c\")\n")
cmake_file.write("file(GLOB CUSTOM_SOURCES \""+current_dir+"/custom/src/*.c\")\n")

build_dir = current_dir + "/build/" + build_platform

cmake_file.write("include_directories("+build_dir+"/include)\n")

cmake_file.write("set(EXECUTIBLE_NAME " + game_name + ")\n")

cmake_file.write("add_executable(${EXECUTIBLE_NAME} ${SOURCES} ${CUSTOM_SOURCES})\n")

cmake_file.write("file(GLOB LIB_FILES "+build_dir+"/lib/*.so)\n")

cmake_file.write("target_link_libraries(${EXECUTIBLE_NAME} ${LIB_FILES})\n")

cmake_file.close()

print("----------------------------------")
print("Running cmake...")
print("----------------------------------")

# run cmake
subprocess.run(["cmake", "-B", "./build/" + build_platform, "-S", "./build"])

print("----------------------------------")
print("Running make...")
print("----------------------------------")

# run make
subprocess.run(["make", "-C", "./build/" + build_platform])

# print out the executible we built
print("----------------------------------")
print("Built Executible:\n" + current_dir + "/build/" + build_platform + "/" + game_name)
# print out "done" in green
print("\033[92m" + "Done!" + "\033[0m")

# if we recieved a command line argument, run the game
if len(sys.argv) > 1:
    if sys.argv[1] == "run":
        print("\n----------------------------------")
        print("Running game...")
        print("----------------------------------")
        subprocess.run(["./build/" + build_platform + "/" + game_name])
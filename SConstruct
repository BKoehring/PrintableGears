#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

cpp_dir = "Gear-cpp/"

includes = []
sources = []

for dir in os.listdir(cpp_dir):
	include = cpp_dir + dir + "/include"
	src = cpp_dir + dir + "/src"
	if os.path.isdir(include):
		print("Found " + dir)
		includes.append(include)
	if os.path.isdir(src):
		sources.append(Glob(src + "/*.cpp"))

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=includes)
env["CXXFLAGS"]=['/std:c++20']

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "Gears_gd/bin/libprintablegears.{}.{}.framework/libprintablegears.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "Gears_gd/bin/libprintablegears.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "Gears_gd/bin/libprintablegears.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    if env["platform"] == "linux":
        env["CXXFLAGS"]=['-std=c++20']
    library = env.SharedLibrary(
        "Gears_gd/bin/libprintablegears{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
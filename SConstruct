#!/usr/bin/env python
import os
import sys
# from scons_compiledb import compile_db # Import the compile_db function # Call the compile_db function to enable compile_commands.json generation 
# compile_db()


# Import the SConstruct from godot-cpp
env = SConscript("godot-cpp/SConstruct")

# Add necessary include directories
env.Append(CPPPATH=[
    "src/glm/",
    "src/oneapi/",
    "src/utility/",
    "src/",
    "src/sdf/",
    "src/voxel_terrain/",
    "src/voxel_terrain/meshing",
    "src/voxel_terrain/meshing/surface_nets",
])

# Add main source files
sources = Glob("src/*.cpp") + Glob("src/utility/*.cpp") + Glob("src/sdf/*.cpp") + \
      Glob("src/voxel_terrain/*.cpp") + Glob("src/voxel_terrain/meshing/*.cpp") + Glob("src/voxel_terrain/meshing/surface_nets/*.cpp")

# Set build output directories
env.VariantDir('build', 'src', duplicate=0)

# idk something about exceptions or something
env.Append(CCFLAGS=["/EHsc"])#, "/Zi", "/Od", "/FS"]) 

# Handle different platforms
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "project/addons/jar_voxel_terrain/bin/jar_voxel_terrain.{}.{}.framework/jar_voxel_terrain.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source='build/*.cpp',
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "project/addons/jar_voxel_terrain/bin/jar_voxel_terrain.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source='build/*.cpp',
        )
    else:
        library = env.StaticLibrary(
            "project/addons/jar_voxel_terrain/bin/jar_voxel_terrain.{}.{}.a".format(env["platform"], env["target"]),
            source='build/*.cpp',
        )
else:
    library = env.SharedLibrary(
        "project/addons/jar_voxel_terrain/bin/jar_voxel_terrain{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source='build/*.cpp',
    )

Default(library)

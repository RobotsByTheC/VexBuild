#!/usr/bin/env python3

import sys
import re
from pathlib import Path, PureWindowsPath, PurePosixPath
import json
import subprocess
from builtins import isinstance
import pathlib
import subprocess
import platform
from warnings import warn

include_regex = re.compile('\s*\#include\s+["<]([^">]+)*[">]')

def build():
    global project_dir
    global toolchain_dir
    
    # Expand directories and make them absolute
    project_dir = project_dir.expanduser().resolve()
    toolchain_dir = toolchain_dir.expanduser().resolve()
    
    setup_toolchain()
    
    debug("Toolchain directory: " + str(toolchain_dir))
    debug("Project directory: " + str(project_dir))
    
    # Check if the project and toolchain directories exist
    if not toolchain_dir.is_dir():
        raise FileNotFoundError("Toolchain directory does not exist or is not a directory.")
    if not project_dir.is_dir():
        raise FileNotFoundError("Project directory does not exist or is not a directory.")
    
    # Check if source directory exists
    global src_dir
    src_dir = project_dir / "src"
    if not src_dir.is_dir():
        raise FileNotFoundError("Source directory does not exist or is not a directory.")
    
    # Check if build directory exists and create it if not
    global build_dir
    build_dir = project_dir / "build"
    if build_dir.exists():
        debug("Build directory exists.")
        if not build_dir.is_dir():
            raise FileExistsError("Build location exists but is not directory.")
    else:
        debug("Creating build directory.")
        build_dir.mkdir()
    
    # Modification time cache file
    global modification_times_file
    modification_times_file = build_dir / "modification_times.cache"
    
    # Read the cached modification times from the file
    read_modification_times()
    
    # Create empty set for the files that need to be compiled
    global modified_files
    modified_files = set()
    
    # Create empty dict for the dependency tree
    global dependency_tree
    dependency_tree = dict()
    global source_files
    source_files = set()
    # Build the dependency tree for each source file
    # This also updates modification_times
    build_dependency_tree(src_dir)
        
    # Create the full list of files that need to be compiled, modified files
    # and their dependencies.
    modified_dependencies()
        
    modified_files = [f for f in modified_files if f.suffix == ".c"]

    output_files = []
    for f in modified_files:
        compile(f)
            
    if len(modified_files) != 0:
        link([build_dir / (f.stem + ".o") for f in source_files])
        
        
    # Write the updated modification times to the cache (only if build was
    # successful).
    write_modification_times()

def parse_args():
    import argparse
    parser = argparse.ArgumentParser()
    
    parser.add_argument("--debug", help="print debug messages", action="store_true")
    parser.add_argument("project_dir", help="project directory", nargs="?", default=".")
    
    default_toolchain_dir = Path(os.path.realpath(__file__)).parent / "Toolchain"
    parser.add_argument("--toolchain", help="vex toolchain location",
                        default=Path(os.getenv("VEX_TOOLCHAIN_HOME", default_toolchain_dir)))
    
    parser.add_argument("--upload", help="try to upload to the Vex controller", action="store_true")
    
    args = parser.parse_args()
    
    global debug_level
    global project_dir
    global upload_enabled
    global toolchain_dir
    debug_level = args.debug
    project_dir = Path(args.project_dir)
    upload_enabled = args.upload
    toolchain_dir = args.toolchain

def setup_toolchain():
    global mcc18
    global mplink
    global c18_header_dir
    global c18_lib
    c18_dir = toolchain_dir / "mcc18"
    c18_bin_dir = c18_dir / "bin"
    
    mcc18 = c18_bin_dir / "mcc18.exe"
    if not mcc18.exists():
        raise FileNotFoundError("Could not find mcc18.exe")
    
    mplink = c18_bin_dir / "mplink.exe"
    if not mplink.exists():
        raise FileNotFoundError("Could not find mplink.exe")
    
    c18_dir = to_windows_path(c18_dir)
    c18_header_dir = c18_dir / "h"
    c18_lib = c18_dir / "lib"
    
    global wpilib_dir
    global wpilib_vex_lib
    global wpilib_easyc_lib
    global wpilib_linker_script
    wpilib_dir = toolchain_dir / "WPILib" / "Vex"
    if not wpilib_dir.exists():
        raise FileNotFoundError("Could not find WPILib.")
    
    wpilib_dir = to_windows_path(wpilib_dir)
    wpilib_vex_lib = wpilib_dir / "Vex_library.lib"
    wpilib_easyc_lib = wpilib_dir / "easyCRuntime.lib"
    wpilib_linker_script = wpilib_dir / "18f8520.lkr"
    
    global vexctl
    vexctl_dir = toolchain_dir / "vexctl"
    
    os = get_os()
    if os[0] == "Linux":
        if os[1]:
            vexctl = vexctl_dir / "vexctl-linux-x86_64"
        
    if vexctl != None and not vexctl.exists():
        raise FileNotFoundError("Could not find vexctl")
        
def read_modification_times():
    global modification_times
    
    if modification_times_file.exists():
        debug("Modification time cache exists.")
        modification_times = json.load(modification_times_file.open())
    else:
        debug("Modification time cache does not exist.")
        modification_times = dict()
        
def write_modification_times():
    modification_times_file
    json.dump(modification_times, modification_times_file.open(mode='w'), indent=4)

def build_dependency_tree(sub_dir):
    for f in sub_dir.iterdir():
        if f.is_dir():
            build_dependency_tree(f)
        else:
            add_includes(f)
   
   
def add_includes(file):
    src_file = file.relative_to(src_dir)
    src_file_path = str(src_file)
    
    if src_file.suffix == ".c":
        source_files.add(src_file)
    
    m_time = file.stat().st_mtime
    if not str(src_file) in modification_times or modification_times[str(src_file)] != m_time:
        modified_files.add(src_file)
        modification_times[str(src_file)] = m_time
    
    for i, line in enumerate(file.open()):
        for match in include_regex.finditer(line):
            dep_file = src_file.parent / match.group(1)
            if (src_dir / dep_file).exists():
                if not dep_file in dependency_tree:
                    dependency_tree[dep_file] = set()
                dependency_tree[dep_file].add(src_file)
            else:
                warn(UserWarning("Could not find \"" + str(dep_file) + "\" included in \"" + str(src_file) + "\""))

def modified_dependencies():
    new_modified_files = set()
    for f in modified_files:
        new_modified_files.update(find_all_dependencies(f))
    modified_files.update(new_modified_files)
     
def find_all_dependencies(file):
    all_deps = set()
    if file in dependency_tree:
        direct_deps = dependency_tree[file]
        all_deps.update(direct_deps)
    
        for f in direct_deps:
            all_deps.update(find_all_dependencies(f))
        
    return all_deps

def compile(file):
    info("Compiling: " + str(file))
    
    args = []
    if get_os()[0] != "Windows":
        args.append("wine")

    output_file = to_windows_path(build_dir / (file.stem + ".o"))
    args.extend([str(mcc18), "-p=18F8520", "-w=2", "-D_VEX_BOARD",
                    "-I=" + str(c18_header_dir), "-I=" + str(wpilib_dir),
                    "-fo=" + str(output_file), str(to_windows_path(src_dir / file))])
    
    if subprocess.call(args) != 0:
        raise ChildProcessError("Failed to compile source file: " + str(file))
    
def link(output_files):
    info("Linking...")
    
    args = []
    if get_os()[0] != "Windows":
        args.append("wine")
        
    args.extend([str(mplink), str(wpilib_linker_script), "/a", "INHX32", "/w", "/m",
                 str(to_windows_path(build_dir / "Mapfile.map")),
                 "/o", str(to_windows_path(get_hex_file()))])
    args.extend([str(to_windows_path(f)) for f in output_files])
    args.extend(["/l", str(c18_lib), str(wpilib_vex_lib), str(wpilib_easyc_lib)])
        
    if subprocess.call(args) != 0:
        raise ChildProcessError("Failed to link executable.")
    
def upload(hex_file):
    if vexctl != None:
        if hex_file.exists():
            info("Uploading...")
            if subprocess.call([str(vexctl), "upload", str(hex_file)]) != 0:
                raise ChildProcessError("Failed to upload code to Vex controller.")
        else:
            raise FileNotFoundError("Hex file (" + str(hex_file) + ") does not exist.")
    else:
        info("Uploading is not yet supported by VexBuild on your platform.")

def to_windows_path(path):
    # A Windows path can only be created from an absolute POSIX path
    if isinstance(path, pathlib.PosixPath) and path.is_absolute():
        # Create a wine windows path
        path_str = str(path)
        path_str = "Z:" + path_str
        path = pathlib.PureWindowsPath(path_str)
    return path

# Get the absolute path to the hex output file
def get_hex_file():
    return build_dir / (project_dir.name + ".hex")
    
def info(msg):
    print(msg, flush=True)
    
def get_os():
    return (platform.system(), platform.machine().endswith("64"))
    
def debug(msg):
    if debug_level:
        info(msg)

if __name__ == "__main__":
    import os
    import warnings
    
    parse_args()
    
    # If debug mode is not enabled, only print warning messages, not their whole stack trace
    if not debug_level:
        warnings.showwarning = lambda message, category, filename, lineno: print("Warning:", message, flush=True, file=sys.stderr)
    
    try:
        # Build the program
        build()
    
        # If the upload flag was given, upload the program
        if upload_enabled:
            upload(get_hex_file())
    except (FileNotFoundError, ChildProcessError) as e:
        # Throw the exception if debug is enabled, otherwise just print it and exit
        if debug_level:
            raise e
        else:
            print("Error: " + str(e), flush=True, file=sys.stderr)
            exit(1)
    

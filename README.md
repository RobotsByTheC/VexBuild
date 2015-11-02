# VexBuild

### A cross-platform build system for the Vex PIC (v0.5) controller.

This system was created because there was no easy way to build code using the MPLAB C18 compiler in a consistent manner across Windows, Linux and OSX, all of which are used by different members of our team. It also was designed to be easily configured for use in an IDE (such as Eclipse).

#### Usage

`python3 vexbuild.py [-h] [--debug] [--toolchain TOOLCHAIN_DIR] [--upload] [PROJECT_DIR]`

By default, the project directory is set to the current directory.
The default toolchain directory is `vexbuild_location/Toolchain`, which should work in almost all cases.

An example project designed to be built by VexBuild is located [here](https://github.com/RobotsByTheC/SavageSoccer2015). This also contains an Eclipse project configured to use VexBuild.

#### Description

The build system starts by checking that the toolchain and project directories seem valid, and then attempts to build all the files source files in the `src/` directory.

To do this, it builds a dependency tree of the source files, using a simple regular expression to search for `#include` statements. This means that comments and `#ifdef`s around the `#include` will break the parser. This should be fine for most Vex code, which is fairly simple.

The script then checks for `build/modification_times.cache`. If it exists, then it checks for files that have changed, and adds them and their dependencies to the list of files that need to be built. Otherwise, all files are built.

Then it compiles each file, placing the output in `build/`. On OSes other than Windows, it tries to run the compiler in Wine.

If the compile is successful, the output files are linked and a hex output file is produced. It has the name of the project directory.

If the `--upload` flag was specified, the script will attempt to upload the code to the robot, using [vexctl](http://personalpages.tds.net/~jwbacon/Computer/roboctl.html). Currently the a compiled version is only included for Linux. It should be pretty simple to compile for OSX, but I don't have access to a machine to do it. I have yet to get a successful build on Windows, but I'm working on it. Currently the best option for Windows is to use [IFI loader](http://www.vexrobotics.com/wiki/PIC_Microcontroller_Downloads), but this process will not be automatic.

#### Requirements

- **All OSes**
  - [Python 3](https://www.python.org/)
  
- **Linux, OSX, and other Unicies**
  - [Wine](https://www.winehq.org/)

#### Limitations

- The dependency parser is rather naive, so comments and `#ifdef`s around `#include` statements will cause it to not function correctly. The simplicity of most Vex code should prevent this from being a big problem.
- There is no way to configure the layout of the project directory. All source files must be located in `src/` and all output will be sent to `build/`. This build system was designed to satisfy our use cases, so configuration was not a priority, but feel free to help add features.

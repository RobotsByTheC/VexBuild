# VexBuild

### A cross-platform build system for the Vex PIC (v0.5) controller.

This system was created because there was no easy way to build code using the MPLAB C18 compiler in a consistent manner across Windows, Linux and OSX, all of which are used by different members of our team. It also was designed to be easily configured for use in an IDE (such as Eclipse).

#### Usage

`python3 vexbuild.py [-h] [--debug] [--toolchain TOOLCHAIN] [--upload] [--dev DEV] [project_dir]`

By default, the project directory is set to the current directory.
The default toolchain directory is `vexbuild_location/Toolchain`, which should work in almost all cases.

An example project designed to be built by VexBuild is located [here](https://github.com/RobotsByTheC/SavageSoccer2015). This also contains an Eclipse project configured to use VexBuild.

#### Description

The build system starts by checking that the toolchain and project directories seem valid, and then attempts to build all the files source files in the `src/` directory.

To do this, it builds a dependency tree of the source files, using a simple regular expression to search for `#include` statements. This means that comments and `#ifdef`s around the `#include` will break the parser. This should be fine for most Vex code, which is fairly simple.

The script then checks for `build/modification_times.cache`. If it exists, then it checks for files that have changed, and adds them and their dependencies to the list of files that need to be built. Otherwise, all files are built.

Then it compiles each file, placing the output in `build/`. On OSes other than Windows, it tries to run the compiler in Wine.

If the compile is successful, the output files are linked and a hex output file is produced. It has the name of the project directory.

If the `--upload` flag was specified, the script will attempt to upload the code to the robot, using VexUpload, which is described below.


#### Requirements

- **All OSes**
  - [Python 3](https://www.python.org/)
  
- **Linux, OSX, and other Unicies**
  - [Wine](https://www.winehq.org/)

#### Limitations

- The dependency parser is rather naive, so comments and `#ifdef`s around `#include` statements will cause it to not function correctly. The simplicity of most Vex code should prevent this from being a big problem.
- There is no way to configure the layout of the project directory. All source files must be located in `src/` and all output will be sent to `build/`. This build system was designed to satisfy our use cases, so configuration was not a priority, but feel free to help add features.

# VexUpload

### A cross-platform uploader for the Vex PIC (v0.5) controller.

This program/module reads Intel hex files and uploads them to the Vex controller. It should work on all platforms where PySerial is supported.

#### Usage

`python3 vexupload.py [-h] [--debug DEBUG] [--dev DEV] hex_file`

If no serial device is specified, it looks for a PL2303 USB-serial converter (which is used by the Vex programmer), and failing that, picks the first serial port it finds.

Valid debug levels are "none" (the default), "verbose", "debug" and "insane". Only use "insane" if you have a huge terminal buffer.

#### Description

The uploader starts by reading the specified hex file into memory. It then attempts to erase the existing program on the controller, and then write the new one. More details are available by reading the code.

I implemented the bootloader communication protocol based on the source code for [vexctl](http://personalpages.tds.net/~jwbacon/Computer/roboctl.html), the [documentation for jifi](https://github.com/defunctzombie/jifi/wiki) and [Microchip Application Note 851](http://ww1.microchip.com/downloads/en/AppNotes/00851b.pdf). Thank you to Jason Bacon for writing vexctl and helping me find this information.

This uploader can be used by other applications as a module, but it is not designed for that, because it prints messages and progress bars.

#### Requirements

- [Python 3](https://www.python.org/)
- [pySerial](https://github.com/pyserial/pyserial)

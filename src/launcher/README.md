# Python launcher

A hack to allow VexBuild to be used in an Eclipse run configuration. Eclipse can't run programs in the PATH, only ones in the workspace.

On Windows, a C program is used to start Python. On Unices (Linux, OSX, etc.), a bash script does the same thing.

`vexbuild.py --copy-launcher` copies the appropriate launcher for the OS from this directory to `../launcher-bin`.

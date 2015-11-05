# Python launcher

A hack to allow VexBuild to be used in an Eclipse run configuration. Eclipse can't run programs in the PATH, only ones in the workspace.

This little piece of code simply tries to launch "python3" and, if that doesn't work, "python". Python must be in the PATH for this program to work. The code is compiled for Linux and Windows, currently. OSX binaries should be easy to create.

`vexbuild.py --copy-launcher` copies the appropriate launcher for the OS from this directory to `../launcher-bin`.

Quartz Operating System
=======================

## File organization

/src - Source and compilation files.<br>
&emsp;./x86 - Source files regarded to x86 architecture.<br>
&emsp;&emsp;./kernel - Kernel source files.<br>
&emsp;&emsp;./bootloader - Bootloader source files. Very `x86` specific.<br>
&emsp;&emsp;./kernlib - Kernel library source files<br>
&emsp;&emsp;./msvc-compile - Microsoft Visual C++ Project files.<br>
&emsp;&emsp;./bat-compile - .BAT script files for compilation on Windows platforms.<br>
/text - Contains descriptive files about the operating system.<br>

## Compilation process

### Windows

Prerequistutes for the build machine:

*	Have Microsoft Visual Studio 14 installed
*	Have Netwide Assembler installed

1. Set envriroment variables:
* `SET build_path=...`
* `SET src_path=...`
* `SET nasm_path=...`
2. Run /src/bat-compile/compile_all.bat

## Read more

Read files in /text directory.

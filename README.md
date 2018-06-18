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

*	Have Microsoft Visual Studio (14) installed
*	Have Netwide Assembler installed
*	Have DD for Windows installed
*	Capable to manage a FAT12 filesystem

1. Set envriroment variables (do not put the `" "` around the path):
* `SET build_path=...`
* `SET src_path=...`
* `SET nasm_path=...`
2. Run '/src/bat-compile/compile_all.bat'
* Execute 'echo %VS140COMNTOOLS%', and set the variable to the Visual Studio's 'tools' directory path if the command result does not satisfy.
3. Copy `stage_one_boot.bin` to the first sector (512 bytes) of a 1.44MB image file.
* Download DD for Windows
* Run 'dd --if=stage_one_boot.bin --of=disk.ima --conv=notrunc'
4. Mount the image file
* Use ImDisk(C) or similar software for this operation.
5. Copy all files directly to the file system.
* 'copy %build_path%/* <mount point, e.g. "A:/".> '

** Tip ** : Visual Studio will leave some junk in the specified build directory; Delete the files for a cleaner image.
## Read more

Read files in /text directory.


@call "%VS140COMNTOOLS%VsDevCmd.bat"
mkdir "%build_path%/system"
msbuild "%src_path%/msvc-compile/Quartz.sln" /t:rebuild /p:OutputPath="%build_path%/system/"
del "%src_path%/msvc-compile/Debug"
del "%src_path%/msvc-compile/Release"

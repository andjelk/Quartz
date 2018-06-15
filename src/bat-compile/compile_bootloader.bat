%nasm_dir% -f bin -i %src_path%/x86/bootloader/include/ %src_path%/x86/bootloader/stageone/bootloader_stage_one_source.asm -o %build_path%/stage_one_boot.bin
dd if=%build_path%/stage_one_boot.bin of=%build_path%/finishdisk.ima conv=notrunc
%nasm_dir% -f bin -i %src_path%/x86/bootloader/include/ %src_path%/x86/bootloader/stagetwo/bootloader_stage_two_source.asm -o %build_path%/qloader

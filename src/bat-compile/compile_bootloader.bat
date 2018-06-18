"%nasm_path%" -f bin -i "%src_path%/x86/bootloader/include/" "%src_path%/x86/bootloader/stageone/bootloader_stage_one_source.asm" -o "%build_path%/stage_one_boot.bin"
"%nasm_path%" -f bin -i "%src_path%/x86/bootloader/include/" "%src_path%/x86/bootloader/stagetwo/bootloader_stage_two_source.asm" -o "%build_path%/qloader"

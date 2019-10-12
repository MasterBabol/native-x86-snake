cd d:\works\
mkdir 2
cd d:\works\2s
mkdir startup

cd d:\program files\nasm

nasm -o d:\works\2s\startup\bootloader.o d:\works\2\startup\bootloader.asm
pause


cd d:\works\2s\startup

type bootloader.o > boot
move boot d:\works\

cd d:\works\
rmdir 2s

pause
cd d:\works\
mkdir 2s

cd d:\program files\nasm

nasm -f coff -o d:\works\2s\asmfunc.o d:\works\2\asmfunc.asm
pause

cd d:\works\djgpp\bin

djgcc -ffreestanding -c -o d:\works\2s\kernel.o d:\works\2\kernel.c |more
pause
djgcc -ffreestanding -c -o d:\works\2s\init.o d:\works\2\init\init.c |more
pause

djgcc -ffreestanding -c -o d:\works\2s\others.o d:\works\2\masters\others.c |more
pause

djgcc -ffreestanding -c -o d:\works\2s\memory.o d:\works\2\masters\memory.c |more
pause
djgcc -ffreestanding -c -o d:\works\2s\task.o d:\works\2\masters\task.c |more
pause
djgcc -ffreestanding -c -o d:\works\2s\keyboard.o d:\works\2\masters\keyboard.c |more
pause
djgcc -ffreestanding -c -o d:\works\2s\snake.o d:\works\2\games\snake.c |more
pause


cd d:\works\2s\

move *.* d:\works\djgpp\bin\

cd d:\works\djgpp\bin
djld asmfunc.o kernel.o init.o memory.o task.o others.o keyboard.o snake.o -o kernel --oformat binary -Ttext 0x00010000
del *.o
pause

move kernel d:\works\
pause

cd d:\works\
rmdir 2s

pause
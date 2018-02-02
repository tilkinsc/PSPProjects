@echo off
TITLE Compiling and Making EBOOT...

IF NOT EXIST bin mkdir bin
IF NOT EXIST o mkdir o

setlocal

call using %PSPToolchain% q

echo Compiling...

mingw32-make EBOOT.PBP

echo Moving resources...

move /y *.SFO bin
move /y *.PBP bin
move /y *.ELF bin

move /y *.o o

echo Copying root assets...

copy /y *.lua bin
copy /y *.txt bin

endlocal

echo Done.


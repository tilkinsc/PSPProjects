@echo off
TITLE Compiling and Making EBOOT...

IF NOT EXIST bin mkdir bin

setlocal

call using %PSPToolchain% q

echo Compiling...

make

echo Moving resources...

move /y *.SFO bin
move /y *.PBP bin
move /y *.ELF bin

endlocal

echo Done.
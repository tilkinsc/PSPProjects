@echo off
TITLE Cleaning environment of all unnecessary files...

set quiet=0

if [%1]==[q] (
	set quiet=1
)
call :init
goto end



:init
setlocal
echo Directory Blacklist: [bin] [o]
echo File Type Blacklist: [.o] [.PBP]
echo.
if [%quiet%]==[1] (
	goto main
	exit /b
)
echo Are you sure you want to delete the patterned files? (y/n)
set /p del_option=
if [%del_option%]==[n] exit /b
if [%del_option%]==[N] exit /b
if [%del_option%]==[y] goto main
if [%del_option%]==[Y] goto main
exit /b
endlocal
:eof_init



:main
setlocal
echo Removing directories...

IF EXIST bin rmdir /Q /S bin
IF EXIST o rmdir /Q /S o

echo Cleaning root...

set blacklist=.o,.PBP,
for %%i in (*.*) do (
	call :iterate_string_del_if_matches "%blacklist%" %%i
)
endlocal
exit /b
:eof_main



:iterate_string_del_if_matches
setlocal
set list=%~1
for /F "tokens=1* delims=," %%a in ("%list%") do (
	call :iterate_string_del_if_matches "%%b" %2
	call :del_if_matches %%a %2
)
endlocal
exit /b
:eof_iterate_string_del_if_matches



:del_if_matches
setlocal
if [%~x2]==[%1] (
	del /Q %2
	echo Removed %2
)
endlocal
exit /b
:eof_parse



:end

echo Done.
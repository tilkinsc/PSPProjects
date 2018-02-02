@echo off
TITLE Running EBOOT.PBP...

IF NOT EXIST bin/EBOOT.PBP (
	echo Failed to find EBOOT.PBP in bin!
	pause
	goto end
)

setlocal

call using %PSPToolchain% q

echo Running...

ppsspp.lnk "%~dp0/bin/EBOOT.PBP"

endlocal

echo Done.

:end

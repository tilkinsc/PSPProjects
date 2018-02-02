@echo off


setlocal
	call using %PSPToolchain%
	
	set name=mips_test
	
	set debug=-O0 -G3 -Wall
	set release=-O2 -G0 -Wall
	
	set TYPE=%debug%
	
	set INCDIR=-I. -Ic:/pspsdk/psp/sdk/include
	set LIBDIR=-L. -Lc:/pspsdk/psp/sdk/lib
	
	set LIBS=
	
	
	set CORE_LIBS=-lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpsputility -lpspuser -lpspkernel
	
	psp-gcc %TYPE% %INCDIR% %LIBDIR% -D_PSP_FW_VERSION=150 -c *.s %CORE_LIBS% %LIBS%
	psp-gcc %TYPE% %INCDIR% %LIBDIR% -D_PSP_FW_VERSION=150 -o %name%.elf *.o %CORE_LIBS% %LIBS%
	psp-fixup-imports %name%.elf
	mksfo 'FileIO' PARAM.SFO
	psp-strip %name%.elf -o %name%_strip.elf
	pack-pbp EBOOT.PBP PARAM.SFO NULL NULL NULL NULL NULL %name%_strip.elf NULL
	
endlocal

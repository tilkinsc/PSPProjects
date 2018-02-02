// include psp stuff
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>


// include commons stuff
#include "../common/callback.h"
#include "../common/ui.h"



// configure PSP stuff
#define VERS    1
#define REVS    0

PSP_MODULE_INFO("LUATEST", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 

// make printing easier on us
#define printf pspDebugScreenPrintf



int main(int argc, char** argv)
{
	// basic init
	setupExitCallback();
	pspDebugScreenInit();
	
	printf("%s\n", "After debug screen and callback init");
	printf("%s\n", "Before main");
	
	// run main logic, call script, check for errors
	while (isRunning()) {
		pspDebugScreenSetXY(0, 0);
		
		printf("%s\n", "Before clear");
		
		pspDebugScreenClear();
		
		printf("%s\n", "Before latching");
		printf("%s\n", "Still alive...");
	}
	
	sceKernelExitGame();
	return 0;
}


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

PSP_MODULE_INFO("UserInput", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 

// make printing easier on us
#define printf pspDebugScreenPrintf


int main(int argc, char** argv)
{
	// basic init
	setupExitCallback();
	pspDebugScreenInit();
	
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	while (isRunning()) {
		sceDisplayWaitVblankStart();
		pspDebugScreenClear();
		pspDebugScreenSetXY(0, 0);
		pollPad();
		pollLatch();
		if(isKeyDown(PSP_CTRL_CROSS))
			printf("Cross is down!\n");
		if(isKeyUp(PSP_CTRL_CROSS))
			printf("Cross is up!\n");
		if(isKeyHold(PSP_CTRL_CIRCLE))
			printf("Circle is down!\n");
		printf("%d,%d", getJX(), getJY());
	}
	
	sceKernelExitGame();
	return 0;
}


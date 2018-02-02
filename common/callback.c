
#include <pspkernel.h>

static int exitRequest  = 1;

int isRunning()
{
	return exitRequest;
}

int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 0;
	return 0;
}

int callbackThread(SceSize args, void *argp)
{
	int exit_callback;
	
	exit_callback = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(exit_callback);
	
	sceKernelSleepThreadCB();
	
	return 0;
}

void setupExitCallback()
{
	int exit_thread;
	
	exit_thread = sceKernelCreateThread("Callback Update Thread", callbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
	
	if(exit_thread >= 0)
		sceKernelStartThread(exit_thread, 0, 0);
}


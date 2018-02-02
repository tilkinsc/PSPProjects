
#include <pspctrl.h>
#include "ui.h"

static SceCtrlData data;
static SceCtrlLatch latch;

int getJX() {
	return data.Lx;
}

int getJY() {
	return data.Ly;
}

void pollPad() {
	sceCtrlReadBufferPositive(&data, 1);
}

void pollLatch() {
	sceCtrlReadLatch(&latch);
}

int isKeyDown(int key) {
	return (latch.uiMake & key);
}

int isKeyUp(int key) {
	return (latch.uiBreak & key);
}

int isKeyHold(int key) {
	return (latch.uiPress & key);
}



#ifndef COMMON_UI_H
#define COMMON_UI_H

int getJX();
int getJY();


void pollPad();
void pollLatch();

int isKeyDown(int key);
int isKeyUp(int key);
int isKeyHold(int key);

#endif

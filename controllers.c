#pragma once
#include "controllers.h"

void Controllers(Vector3 *vector)
{
		SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);

	if (pad.Buttons & PSP_CTRL_UP) {
		vector->y+= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_DOWN) {
		vector->y-= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_LEFT) {
		vector->x-= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_RIGHT) {
		vector->x+= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_CIRCLE) {
		vector->rotationX+= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_SQUARE) {
		vector->rotationX-= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_RTRIGGER) {
		vector->rotationY+= 0.1f;	
	}
	if (pad.Buttons & PSP_CTRL_LTRIGGER) {
		vector->rotationY-= 0.1f;
	}
	if (pad.Buttons & PSP_CTRL_TRIANGLE) {
		vector->forward+= 0.1f;
	}
	if(pad.Buttons & PSP_CTRL_CROSS) {
		pspDebugScreenPrintf("the universe is expanding");
	}
}

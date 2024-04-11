#pragma once
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <pspgu.h>
#include <pspgum.h>

#include "common/callbacks.h"
#include "common/vram.h"

#define print(x)  pspDebugScreenPrintf(x)
#define printv(floatVal)  pspDebugScreenPrintf("%f",floatVal)
#define printi(integerVal)  pspDebugScreenPrintf("%i",integerVal)
#define printd(doubleVal)  pspDebugScreenPrintf("%d",doubleVal)
#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

typedef struct Vector3 {
float x,y,z;
float rotationX,rotationY,rotationZ;
float forward;
}Vector3;

typedef struct Vertecies
{
	float u, v;
	unsigned int color;
	float x,y,z;
}Vertex;


static unsigned int __attribute__((aligned(16))) list[262144];
unsigned char logo_start[];
unsigned char logo_start2[64 * 64 * 64];

//why they use implicit declaration ???
void SetMatrix();
void Draw(Vertex* vertices);
void SetPosition(Vector3* position,Vector3* vector , int offset);
void SetRotation(Vector3* rotation,Vector3* vector);
void SetTexture();



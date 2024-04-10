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

#include "./common/callbacks.h"
#include "./common/vram.h"

struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};

typedef struct Vector3 {
float x,y,z;
float rotationX,rotationY,rotationZ;
float forward;
}Vector3;

static unsigned int __attribute__((aligned(16))) list[262144];
unsigned char logo_start[];

//why they use implicit declaration ???
void SetMatrix();
void Draw(struct Vertex* vertices);
void SetPosition(struct ScePspFVector3* position,Vector3* vector , int offset);
void SetRotation(struct ScePspFVector3* rotation,Vector3* vector);
void SetTexture();



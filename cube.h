#pragma once
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pspaudio.h>
#include <pspmp3.h>


#include <pspgu.h>
#include <pspgum.h>

#include "common/callbacks.h"
#include "common/vram.h"

#define print(x) pspDebugScreenPrintf(x)
#define printv(floatVal)  pspDebugScreenPrintf("%f",floatVal)
#define printi(integerVal) pspDebugScreenPrintf("%i",integerVal)
#define printd(doubleVal) pspDebugScreenPrintf("%d",doubleVal)
#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)


int fillStreamBuffer( int fd, int handle )
{
	char* dst;
	int write;
	int pos;
	// Get Info on the stream (where to fill to, how much to fill, where to fill from)
	int status = sceMp3GetInfoToAddStreamData( handle, &dst, &write, &pos);
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3GetInfoToAddStreamData returned 0x%08X\n", status);
	}

	// Seek file to position requested
	status = sceIoLseek32( fd, pos, SEEK_SET );
	if (status<0)
	{
		ERRORMSG("ERROR: sceIoLseek32 returned 0x%08X\n", status);
	}
	
	// Read the amount of data
	int read = sceIoRead( fd, dst, write );
	if (read < 0)
	{
		ERRORMSG("ERROR: Could not read from file - 0x%08X\n", read);
	}
	
	if (read==0)
	{
		// End of file?
		return 0;
	}
	
	// Notify mp3 library about how much we really wrote to the stream buffer
	status = sceMp3NotifyAddStreamData( handle, read );
	if (status<0)
	{
		ERRORMSG("ERROR: sceMp3NotifyAddStreamData returned 0x%08X\n", status);
	}
	
	return (pos>0);
}
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



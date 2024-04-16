#include "factory.h"
#include "data.h"
#include <pspaudio.h>
#include <pspmp3.h>
#include <psputility.h>

PSP_MODULE_INFO("Cube Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
#define MP3FILE "../starwars.mp3"


//#define ERRORMSG(...) { char msg[128]; sprintf(msg,__VA_ARGS__); error(msg); }

char	mp3Buf[16*1024]  __attribute__((aligned(64)));
short	pcmBuf[16*(1152/2)]  __attribute__((aligned(64)));

void SetPosition(Vector3 *position,Vector3 *vector , int offset)
{
    position->x = vector->x + offset;
    position->y = vector->y;
    position->z = vector->forward - 4.5f;
	sceGumTranslate(position);
};
void SetRotation(Vector3* rotation,Vector3* vector)
{
	rotation->x = vector->rotationX;
	rotation->y = vector->rotationY;
	rotation->z = 0;
	sceGumRotateXYZ(rotation);
};


int fillStreamBuffer( int fd, int handle )
{
	char* dst;
	int write;
	int pos;
	// Get Info on the stream (where to fill to, how much to fill, where to fill from)
	int status = sceMp3GetInfoToAddStreamData( handle, &dst, &write, &pos);
	if (status<0)
	{
		printi(status);
		//ERRORMSG("ERROR: sceMp3GetInfoToAddStreamData returned 0x%08X\n", status);
	}

	// Seek file to position requested
	status = sceIoLseek32( fd, pos, SEEK_SET );
	if (status<0)
	{
	printi(status);	
		//ERRORMSG("ERROR: sceIoLseek32 returned 0x%08X\n", status);
	}
	
	// Read the amount of data
	int read = sceIoRead( fd, dst, write );
	if (read < 0)
	{
	printi(status);
		//ERRORMSG("ERROR: Could not read from file - 0x%08X\n", read);
	}
	
	if (read==0)
	{
		printi(status);
		// End of file?
		return 0;
	}
	
	// Notify mp3 library about how much we really wrote to the stream buffer
	status = sceMp3NotifyAddStreamData( handle, read );
	if (status<0)
	{
		printi(status);
		//ERRORMSG("ERROR: sceMp3NotifyAddStreamData returned 0x%08X\n", status);
	}


	//printi(fd);
	//printi(handle);

	return (pos>0);
}



void SetTexture()
{
		sceGuTexMode(GU_PSM_4444,0,0,0);
		sceGuTexImage(0,64,64,64,logo_start);
		sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGBA);
		//sceGuTexEnvColor(0xffff00);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(1.0f,1.0f);
		sceGuTexOffset(0.0f,0.0f);
		//sceGuAmbientColor(0xffffffff);
};
void SetTexture2()
{
		sceGuTexMode(GU_PSM_4444,0,0,0);
		sceGuTexImage(0,64,64,64,logo_start2);
		sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGBA);
		//sceGuTexEnvColor(0xffff00);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(1.0f,1.0f);
		sceGuTexOffset(0.0f,0.0f);
		//sceGuAmbientColor(0xffffffff);
};


void SetMatrix()
{
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
};
void Draw(Vertex* vertices)
{
		sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
};

typedef struct {
int paused , handle, fd , numPlayed,volume,channel,lastDecoded,numChannels,samplingRate;
}DataSet;

void RunMusic(DataSet *set)
{
		if (!set->paused)
		{
			// Check if we need to fill our stream buffer
			if (sceMp3CheckStreamDataNeeded( set->handle )>0)
			{
				fillStreamBuffer( set->fd, set->handle );
			}
	

			// Decode some samples
			short* buf;
			int bytesDecoded;
			int retries = 0;
			// We retry in case it's just that we reached the end of the stream and need to loop
			for (;retries<1;retries++)
			{
				bytesDecoded = sceMp3Decode( set->handle, &buf );
				if (bytesDecoded>0)
					break;
				
				if (sceMp3CheckStreamDataNeeded( set->handle )<=0)
					break;
				
				if (!fillStreamBuffer( set->fd, set->handle ))
				{
					set->numPlayed = 0;
				}
			}
			if (bytesDecoded==0 || bytesDecoded==0x80671402)
			{
				set->paused = 1;
				sceMp3ResetPlayPosition( set->handle );
				set->numPlayed = 0;
				printi(set->numPlayed);
			}
			else
			{
				// Reserve the Audio channel for our output if not yet done
				if (set->channel<0 || set->lastDecoded!=bytesDecoded)
				{
					if (set->channel>=0)
						sceAudioSRCChRelease();
					
					set->channel = sceAudioSRCChReserve( bytesDecoded/(2*set->numChannels), set->samplingRate, set->numChannels );
				}
				// Output the decoded samples and accumulate the number of played samples to get the playtime
				set->numPlayed += sceAudioSRCOutputBlocking( set->volume, buf );
				//set->numPlayed = 0;
			}
		}
}


DataSet InitSound()
{
	// Load modules
	int status = sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC);
	if (status<0)
	{
	printi(0);
		//ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_AVCODEC) returned 0x%08X\n", status);
	}
	
	status = sceUtilityLoadModule(PSP_MODULE_AV_MP3);
	if (status<0)
	{
	printi(0);
		//ERRORMSG("ERROR: sceUtilityLoadModule(PSP_MODULE_AV_MP3) returned 0x%08X\n", status);
	}
	
	// Open the input file
	int fd = sceIoOpen( MP3FILE, PSP_O_RDONLY, 0777 );
	if (fd<0)
	{
	printi(0);
		//ERRORMSG("ERROR: Could not open file '%s' - 0x%08X\n", MP3FILE, fd);
	}
	
	// Init mp3 resources
	status = sceMp3InitResource();
	if (status<0)
	{
	printi(0);
		//ERRORMSG("ERROR: sceMp3InitResource returned 0x%08X\n", status);
	}
	
	// Reserve a mp3 handle for our playback
	SceMp3InitArg mp3Init;
	mp3Init.mp3StreamStart = 0;
	mp3Init.mp3StreamEnd = sceIoLseek32( fd, 0, SEEK_END );
	mp3Init.unk1 = 0;
	mp3Init.unk2 = 0;
	mp3Init.mp3Buf = mp3Buf;
	mp3Init.mp3BufSize = sizeof(mp3Buf);
	mp3Init.pcmBuf = pcmBuf;
	mp3Init.pcmBufSize = sizeof(pcmBuf);


	printi(mp3Init.mp3BufSize);
	
	int handle = sceMp3ReserveMp3Handle( &mp3Init );
	if (handle<0)
	{
	printi(0);
		//ERRORMSG("ERROR: sceMp3ReserveMp3Handle returned 0x%08X\n", handle);
	}
	
	// Fill the stream buffer with some data so that sceMp3Init has something to work with
	fillStreamBuffer(fd,handle);
	
	status = sceMp3Init(handle);
	if (status<0)
	{
	printi(0);
		//ERRORMSG("ERROR: sceMp3Init returned 0x%08X\n", status);
	}
	
	int channel = -1;
	int samplingRate = sceMp3GetSamplingRate(handle);
	int numChannels = sceMp3GetMp3ChannelNum(handle);
	int lastDecoded = 0;
	int volume = PSP_AUDIO_VOLUME_MAX;
	int numPlayed = 0;
	int paused = 0;
	int lastButtons = 0;
	int loop = 0;

DataSet set ;

set.paused = paused;
set.handle = handle;
set.fd = fd;
set.channel = channel;
set.numChannels = numChannels;
set.lastDecoded = lastDecoded;
set.volume = volume;

return set;
}




int main(int argc, char* argv[])
{
	int i;
	for(i = 0 ; i < 64 * 64 * 64 ; i++)
		logo_start[i] = i * i * i;
	for(i = 0 ; i < 64 * 64 * 64; i++)
		logo_start2[i] = 255;

	setupCallbacks();
	pspDebugScreenInit();
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// setup GU

	void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	// sceGuInit();

	// sceGuStart(GU_DIRECT,list);
	// sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
    // sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	// sceGuDepthBuffer(zbp,BUF_WIDTH);
	// sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	// sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	// sceGuDepthRange(65535,0);
	// sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	// sceGuEnable(GU_SCISSOR_TEST);
	// sceGuDepthFunc(GU_GEQUAL);
	// sceGuEnable(GU_DEPTH_TEST);
	// sceGuFrontFace(GU_CW);
	// sceGuShadeModel(GU_SMOOTH);
	// sceGuEnable(GU_CULL_FACE);
	// sceGuEnable(GU_TEXTURE_2D);
	// sceGuEnable(GU_CLIP_PLANES);
	// sceGuFinish();
	// sceGuSync(0,0);
	// sceDisplayWaitVblankStart();
	// sceGuDisplay(GU_TRUE);

	DataSet set = InitSound();

	Vector3 vector;
    Cube cube;
	cube.position = malloc(sizeof(Vector3));
	cube.rotation = malloc(sizeof(Vector3));
	int numberOfStars = 20;
	const int numberOfStarsInBackGround = 300;
	Projectile projs[numberOfStars];
	Projectile backgroundStars[numberOfStarsInBackGround];

	int j;
	for(j = 0 ; j < numberOfStarsInBackGround ; j++)
	{
		int randomNumberx = rand() % numberOfStarsInBackGround;
		int randomNumbery = rand() % numberOfStarsInBackGround;
		int randomNumberz = rand() % -10;

		Vector3 vector = {(-numberOfStarsInBackGround/2 + randomNumberx),
						  (-numberOfStarsInBackGround/2 + randomNumbery),
						   -100 * randomNumberz};
		Projectile proj = {vector, 0.6f};
		backgroundStars[j] = proj;
	}

	int k;
	for(k = 0 ; k < numberOfStars ; k++)
	{
		int randomNumberx = rand() % 10;
		int randomNumbery = rand() % 10;
		int randomNumberz = rand() % -20;

		Vector3 vector = {(randomNumberx - 5),
						  (randomNumbery - 5),
						   -100 * randomNumberz};
		Projectile proj = {vector, 1};
		projs[k] = proj;
	}


	while(running())
	{
		sceGuStart(GU_DIRECT,list);

		Controllers(&vector);

		// clear screen
		sceGuClearColor(0x000000);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

		InitCube(&cube);
		cube.CreateCube(cube.position,cube.rotation,&vector,3);
		Draw(vertices);

		int j;
		for(j = 0 ; j  <  numberOfStars ; j++)
		{
		UpdateProjectile(&projs[j]);
		Draw(vertices);

		if(IsCollided(cube.position , &projs[j].position))
		{
			vector.x = 200;
		}	
		}

		//background objects
		int k;
		for(k = 0 ; k < numberOfStarsInBackGround ; k++)
		{
			UpdateBackground(&backgroundStars[k].position,backgroundStars[k].speed,numberOfStarsInBackGround);
			Draw(vertices);
		}

		sceGuFinish();
		sceGuSync(0,0);

		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();


		RunMusic(&set);
	}
	free(cube.position);
	free(cube.rotation);
	sceGuTerm();

	sceKernelExitGame();
	return 0;
}

#include "factory.h"
#include "data.h"

PSP_MODULE_INFO("Cube Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

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

int main(int argc, char* argv[])
{
	int i;
	for(i = 0 ; i < 64 * 64 * 64 ; i++)
		logo_start[i] = i * i * i / (i + 20);
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

	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
    sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	Vector3 vector;

    Cube cube;
	cube.position = malloc(sizeof(Vector3));
	cube.rotation = malloc(sizeof(Vector3));

	Projectile projs[20];

	int k;
	for(k = 0 ; k < 1000 ; k++)
	{
		int randomNumberx = rand() % 1000;
		int randomNumbery = rand() % 1000;
		int randomNumberz = rand() % -20;

		Vector3 vector = {-100 + randomNumberx + -k,-100 + randomNumbery + -k,-100 * randomNumberz};
		Projectile proj = {vector, 0.6f};
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
		for(j = 0 ; j  <  1000 ; j++)
		{
		UpdateProjectile(&projs[j].position,projs[j].speed);
		Draw(vertices);
		if(IsCollided(cube.position , &projs[j].position))
		{
			vector.x = 200;
		}	
		}

		sceGuFinish();
		sceGuSync(0,0);

		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}
	free(cube.position);
	free(cube.rotation);
	sceGuTerm();

	sceKernelExitGame();
	return 0;
}

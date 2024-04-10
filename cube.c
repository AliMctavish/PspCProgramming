#include "factory.h"
#include "data.h"

PSP_MODULE_INFO("Cube Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

void SetPosition(struct ScePspFVector3 *position,Vector3 *vector , int offset)
{
    position->x = vector->x + offset;
    position->y = vector->y;
    position->z = vector->forward - 4.5f;
	sceGumTranslate(position);
};
void SetRotation(struct ScePspFVector3* rotation,Vector3* vector)
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
void Draw(struct Vertex* vertices)
{
		sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
};

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)


int main(int argc, char* argv[])
{
	int i;
	for(i = 0 ; i < 64 * 64 * 64 ; i++)
		logo_start[i] = i * i * i / (i + 20);

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

	// run sample
	Vector3 vector;
    Cube cube;

	//Projectile proj = {malloc(sizeof(struct ScePspFVector3)) ,{2,2,2} ,  }

	Projectile projectile;
	Projectile projectile2;

	cube.position = malloc(sizeof(struct ScePspFVector3));
	cube.rotation = malloc(sizeof(struct ScePspFVector3));
	projectile.position = malloc(sizeof(struct ScePspFVector3));
	projectile2.position = malloc(sizeof(struct ScePspFVector3));
	projectile.position->z = -25.0f;
	projectile2.position->z = -25.0f;

	while(running())
	{
		sceGuStart(GU_DIRECT,list);

		Controllers(&vector);

		// clear screen
		sceGuClearColor(0xffffff);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

		InitCube(&cube);
		cube.CreateCube(cube.position,cube.rotation,&vector,3);
		Draw(vertices);

		// int i;
		// for(i = 0 ; i < sizeof(projectiles) ; i++)
		// {
		// 	InitProjectile(&projectile);
		// 	projectile.CreateProjectile(projectile.position);

		// 	if(IsCollided(cube.position , projectile.position))
		// 	{
		// 		break;
		// 		vector.x = 200;
		// 	}	

		// 	Draw(vertices);
		// }

		if(IsCollided(cube.position , projectile.position))
		{
			break;
			vector.x = 200;
		}	

		//InitProjectile(&projectile);
		UpdateProjectile(projectile.position);
		Draw(vertices);


		//InitProjectile(&projectile2);
		projectile2.position->x = 3;
		UpdateProjectile(projectile2.position);


		if(IsCollided(cube.position , projectile2.position))
		{
			break;
			vector.x = 200;
		}	

		Draw(vertices);

		sceGuFinish();
		sceGuSync(0,0);

		sceDisplayWaitVblankStart();
		sceGuSwapBuffers();
	}

	free(cube.position);
	free(cube.rotation);
	free(projectile.position);
	free(projectile2.position);
	sceGuTerm();

	sceKernelExitGame();
	return 0;
}

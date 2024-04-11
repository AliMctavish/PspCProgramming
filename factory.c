#include "factory.h"

 
void CreateCube(Vector3 *position,Vector3 *rotation ,Vector3 *vector , int number)
{
	SetMatrix();
	SetPosition(position,vector,number);
	SetRotation(rotation,vector);
	SetTexture();
}

void UpdateProjectile(Vector3 *position,float speed)
{
	SetMatrix();
    position->z += speed;
	sceGumTranslate(position);
	sceGumRotateX(position->z * 0.1f);
	sceGumRotateY(position->z * 0.1f);
	SetTexture2();
}   

bool IsCollided(Vector3 *object1Pos , Vector3 *object2Pos)
{
	float boundaries = 2;

	float cubeLeft = object1Pos->x;
    float cubeRight = object1Pos->x + boundaries;
	float cubeUp = object1Pos->y;
	float cubeDown = object1Pos->y + boundaries;
	float cubeFront = object1Pos->z;
	float cubeBack = object1Pos->z + (boundaries - 1);

	float projectileLeft = object2Pos->x;
    float projectileRight = object2Pos->x + boundaries;
	float projectileUp = object2Pos->y;
	float projectileDown = object2Pos->y + boundaries;
	float projectileFront = object2Pos->z;
	float projectileBack = object2Pos->z + (boundaries- 1);


	if( projectileBack > cubeFront && projectileFront < cubeBack && cubeLeft < projectileRight && cubeRight > projectileLeft &&
        cubeUp < projectileDown && cubeDown > projectileUp)
		return true;

	return false;
}


void InitCube(Cube *cube)
{
	cube->CreateCube = &CreateCube;
}


void InitProjectile(Projectile *projectile)
{
	//projectile->UpdateProjectile = &UpdateProjectile;
}
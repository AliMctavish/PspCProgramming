#pragma once
#include "cube.h"
#include "types.h"


typedef struct Cube{
Vector3 *position;
Vector3 *rotation;
void (*CreateCube)(Vector3 *position,Vector3 *rotation ,Vector3 *vector , int number);
} Cube;

typedef struct Projectile{
Vector3 position;
float speed;
//void (*UpdateProjectile)(Vector3 *position);
}Projectile;

bool IsCollided(Vector3 *object1Pos , Vector3 *object2Pos);
void CreateCube(Vector3 *position,Vector3 *rotation ,Vector3 *vector , int number);
void UpdateProjectile(Vector3 *position,float speed);
void InitCube(Cube *cube);
void InitProjectile(Projectile *projectile);
void Controllers(Vector3* vector);

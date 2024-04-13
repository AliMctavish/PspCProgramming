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
}Projectile;

bool IsCollided(Vector3 *object1Pos , Vector3 *object2Pos);
void CreateCube(Vector3 *position,Vector3 *rotation ,Vector3 *vector , int number);
void UpdateProjectile(Projectile *projectile);
void UpdateBackground(Vector3 *position,float speed, int numberOfStarsInBackground);
void InitCube(Cube *cube);
void InitProjectile(Projectile *projectile);
void Controllers(Vector3* vector);

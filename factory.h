#pragma once
#include "cube.h"
#include "types.h"


typedef struct Cube{
struct ScePspFVector3 *position;
struct ScePspFVector3 *rotation;
void (*CreateCube)(struct ScePspFVector3 *position,struct ScePspFVector3 *rotation ,Vector3 *vector , int number);
} Cube;

typedef struct Projectile{
struct ScePspFVector3 *position;
float speed;
void (*CreateProjectile)(struct ScePspFVector3 *position);
}Projectile;

bool IsCollided(struct ScePspFVector3 *object1Pos , struct ScePspFVector3 *object2Pos);
void CreateCube(struct ScePspFVector3 *position,struct ScePspFVector3 *rotation ,Vector3 *vector , int number);
void CreateProjectile(struct ScePspFVector3 *position);
void InitCube(Cube *cube);
void InitProjectile(Projectile *projectile);
void Controllers(Vector3* vector);

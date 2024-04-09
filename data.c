#include "data.h"

struct Vertex __attribute__((aligned(16))) vertices[12*3] =
{
	{0, 0, 5,-1,-1, 1}, // 0
	{1, 0, 5,-1, 1, 1}, // 4
	{1, 1, 5, 1, 1, 1}, // 5

	{0, 0, 5,-1,-1, 1}, // 0
	{1, 1, 5, 1, 1, 1}, // 5
	{0, 1, 5, 1,-1, 1}, // 1

	{0, 0, 5,-1,-1,-1}, // 3
	{1, 0, 5, 1,-1,-1}, // 2
	{1, 1, 5, 1, 1,-1}, // 6

	{0, 0, 5,-1,-1,-1}, // 3
	{1, 1, 5, 1, 1,-1}, // 6
	{0, 1, 5,-1, 1,-1}, // 7

	{0, 0, 0, 1,-1,-1}, // 0
	{1, 0, 0, 1,-1, 1}, // 3
	{1, 1, 0, 1, 1, 1}, // 7

	{0, 0, 0, 1,-1,-1}, // 0
	{1, 1, 0, 1, 1, 1}, // 7
	{0, 1, 0, 1, 1,-1}, // 4

	{0, 0, 0,-1,-1,-1}, // 0
	{1, 0, 0,-1, 1,-1}, // 3
	{1, 1, 0,-1, 1, 1}, // 7

	{0, 0, 0,-1,-1,-1}, // 0
	{1, 1, 0,-1, 1, 1}, // 7
	{0, 1, 0,-1,-1, 1}, // 4

	{0, 0, 0x800080f,-1, 1,-1}, // 0
	{1, 0, 0x800080f, 1, 1,-1}, // 1
	{1, 1, 0x800080f, 1, 1, 1}, // 2

	{0, 0, 0x800080f,-1, 1,-1}, // 0
	{1, 1, 0x800080f, 1, 1, 1}, // 2
	{0, 1, 0x800080f,-1, 1, 1}, // 3


	{0, 0, 0x800080f,-1,-1,-1}, // 4
	{1, 0, 0x800080f,-1,-1, 1}, // 7
	{1, 1, 0x800080f, 1,-1, 1}, // 6

	{0, 0, 0x800080f,-1,-1,-1}, // 4
	{1, 1, 0x800080f, 1,-1, 1}, // 6
	{0, 1, 0x800080f, 1,-1,-1}, // 5
};
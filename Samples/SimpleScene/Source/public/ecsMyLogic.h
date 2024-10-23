#pragma once

#include <flecs.h>

struct Timer
{
	float timer;
	float max_time;
	bool timer_on;
};

struct CollisionSize
{
	float x;
	float y;
	float z;
	bool collision_on;
};
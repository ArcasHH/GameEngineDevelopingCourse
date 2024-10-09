#pragma once

#include <flecs.h>
#include <Vector.h>

struct Gun
{
	float timer;
	float reload_time;
	bool reloaded;
	bool is_shoot;
};

struct DespawnOnBouncePlane
{
	float timer;
	float despawn_time;
};

struct Bullet 
{ 
};

void RegisterEcsShooterSystems(flecs::world& world);
#pragma once

#include <flecs.h>
#include <Vector.h>


struct Gun
{
	float timer;
	float reload_time;

	bool reloaded;
	bool is_shoot;

	int ammo;
	int max_ammo;
};

struct DespawnOnBouncePlane
{
	float timer;
	float despawn_time;
	bool collide_ground;
};

struct FireRate 
{ 
	float timer;
	float time_for_one_shot;
	bool can_shoot;
};



void RegisterEcsShooterSystems(flecs::world& world);
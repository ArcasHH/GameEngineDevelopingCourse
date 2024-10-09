#pragma once

#include <flecs.h>
#include <Vector.h>

struct Gun
{
	bool reloaded;
};

struct Bullet 
{ 

};

void RegisterEcsShooterSystems(flecs::world& world);
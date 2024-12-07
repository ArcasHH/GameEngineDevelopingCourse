#pragma once

#include <flecs.h>

struct Position
{
	float x;
	float y;
	float z;
};

struct Rotation
{
	float x;
	float y;
	float z;
	float angle;
};

struct Velocity
{
	float x;
	float y;
	float z;
};

struct AngularVelocity
{
	float x;
	float y;
	float z;
	float speed;
};

struct Gravity
{
	float x;
	float y;
	float z;
};

struct BouncePlane
{
	float x;
	float y;
	float z;
	float w;
};

struct Bounciness
{
	float value;
};

struct ShiverAmount
{
	float value;
};

struct FrictionAmount
{
	float value;
};

struct Speed
{
	float value;
};


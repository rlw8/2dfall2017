#pragma once
#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"

typedef struct Entity_S
{
	char in_use;
	struct Entity_S *parent;

	Vector2D position;


	/*
	Add the filling:
	position,
	velocity,
	bounds,
	graphic,
	scale,
	frame,
	health,
	etc...
	*/
}Entity;







/*eol@eof*/
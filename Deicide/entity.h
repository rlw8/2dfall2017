#pragma once
#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"

typedef struct Entity_S
{
	char in_use;
	struct Entity_S *parent;

	Vector2D position;
	Sprite sprite;
	Vector2D velocity;

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

//initializes entity system, takes max number of entities
void entity_system_init(Uint32 maxe);

//finds a free spot in memory to create an entity, setting in use
//returns pointer to entity
Entity *entity_new();

//makes an entity and sets its variables, takes sprite position and velocity
void entity_spawn(Sprite sp, Vector2D pos, Vector2D vel);

//deletes an entity, using a pointer to said entity
void entity_delete(Entity *entity);

//updates all entities by adding velocity to where they are
void entity_update();

//draws the entity sprite over the entity, takes sprite and position
void entity_draw(Sprite sprite, Vector2D);

//draws all entities by using the entity draw function
void entity_draw_all();

//deletes all entities that are in use by using the entity delete function
void entity_system_close();

/*eol@eof*/
#pragma once
#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"

typedef struct Entity_S
{
	char in_use;
	struct Entity_S *parent;

	Vector2D position;
	Sprite *sprite;
	Vector2D velocity;
	char *name;
	float speed;
	char grounded;
	SDL_Rect box;

	int health;
	int damage;
	short facing;
	short invul;

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

//sets the first spot in the entity_list array to the player character
//Entity *player_init();

//finds a free spot in memory to create an entity, setting in use
//returns pointer to entity
Entity *entity_new();

//makes an entity and sets its variables, takes sprite position and velocity
void entity_spawn(Sprite *sp, Vector2D pos, Vector2D vel, char *name, float speed, SDL_Rect box, int hp, int dmg);

//deletes an entity, using a pointer to said entity
void entity_delete(Entity *entity);

//updates all entities by adding velocity to where they are
void entity_update();

//draws the entity sprite over the entity, takes sprite and position
void entity_draw(Sprite *sprite, Vector2D);

//draws all entities by using the entity draw function
void entity_draw_all();

//kills and entity using the entity delete function
void entity_die(Entity *self);

//kills all entities using the entity_die function
void entity_die_all();

//deletes all entities that are in use by using the entity delete function
void entity_system_close();

//damages first entity by damage of the second
void damage(Entity *hitter, Entity *hittee);

//makes something invulnerable for a set time
void invuln(Entity *thing_that_is_becoming_invulnerable, float time);

//checks if the player is colliding with the other entities
void entity_collision();

//moves the world based on where the player is located on the screen
void move_world();

/*eol@eof*/
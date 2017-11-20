#pragma once
#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"

void player_spawn(Entity *player, Sprite *sp, Vector2D pos,Vector2D vel, char *nam,	float spd, SDL_Rect bx, Uint32 hp, int damage);

void move(Entity *player, int neg);

void jump(Entity *player);

void drop();

void physics(Entity *player);
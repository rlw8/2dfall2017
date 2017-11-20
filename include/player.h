#pragma once
#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"

void move(Entity *self, int neg);

void jump(Entity *self);

void drop();
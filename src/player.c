#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"





void move(Entity *self, int neg)
{
	self->position.x = self->position.x + ((self->velocity.x * neg)* self->speed);
}

void jump()
{


}

void drop()
{

}

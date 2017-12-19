#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"
#include "simple_logger.h"


void player_spawn(
	Entity *player,
	Sprite *sp,
	Vector2D pos,
	Vector2D vel,
	char *nam,
	float spd,
	SDL_Rect bx,
	int hp,
	int dmg)
{
	player->sprite = sp;
	player->position = pos;
	player->velocity = vel;
	player->name = nam;
	player->speed = spd;
	player->box = bx;
	player->box.x = pos.x;
	player->box.y = pos.y;
	player->health = hp;
	player->damage = dmg;
	player->facing = 1;
}
/*
void move(Entity *player, int neg)
{
	player->velocity.x = 4;
	player->velocity.x *= neg;
	player->position.x = (player->position.x + ((player->velocity.x)* player->speed));
	//slog("position.x %f",player->position.x );
	//slog("neg %i", neg);
	//slog("velocity.x %f", player->velocity.x);
	//slog("speed %f",player->speed);
	//slog("moving");
}
*/
void jump(Entity *player)
{
	//slog("jumping");
	if (player->grounded == 1)
	{
		player->velocity.y = -3;
		player->grounded = 0;
	}
}

/*
void drop()
{

}

void physics(Entity *entity)
{
	if (entity->grounded == 0)
	{
		entity->velocity.y += .1;
		entity->position.y = (entity->position.y + (entity->velocity.y * entity->speed));
	}else entity->velocity.y = 0;

	
}
*/

/*eol@eof*/

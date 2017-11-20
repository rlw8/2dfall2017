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
}

void move(Entity *player, int neg)
{
	player->velocity.x = 4;
	player->position.x = (player->position.x + ((player->velocity.x * neg)* player->speed));
	//slog("position.x %f",player->position.x );
	//slog("neg %i", neg);
	//slog("velocity.x %f", player->velocity.x);
	//slog("speed %f",player->speed);
	//slog("moving");
}

void jump(Entity *player)
{
	slog("jumping");
	if (player->grounded == 1)
	{
		player->velocity.y = -3;
		player->grounded = 0;
	}
}

void drop()
{

}

void physics(Entity *player)
{
	if (player->grounded == 0)
	{
		player->velocity.y += .1;
		player->position.y = (player->position.y + (player->velocity.y * player->speed));
	}else player->velocity.y = 0;

	
}


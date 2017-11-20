#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"
#include "simple_logger.h"

Sprite *player_slash;
SDL_Rect aox = {NULL,NULL, 32, 64};
int hcol = 0, vcol = 0;

typedef struct
{
	Uint32 max_entities;
	Entity * entity_list;
}EntityManager;

static EntityManager entity_manager;

void entity_system_init(Uint32 maxe)
{
	if (!maxe)
	{
		slog("dont do that");
		return;
	}
	entity_manager.max_entities = maxe;
	entity_manager.entity_list = (Entity *)malloc(sizeof(Entity)*maxe);
	memset(entity_manager.entity_list, 0, sizeof(Entity)*maxe);
	slog("entity system initialized");
	atexit(entity_system_close);
}

/*
Entity *player_init()
{
	if (entity_manager.entity_list[0].in_use == 1) return;
	entity_manager.entity_list[0].in_use = 1;
	return &entity_manager.entity_list[0];
}
*/

Entity *entity_new()
{
	int i;

	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 0)
		{
			entity_manager.entity_list[i].in_use = 1;
			return &entity_manager.entity_list[i];
		}
	}
	slog("No more entity spots available");
	return NULL;
}
//Check through list, If any entitiy isnt in use, set INUSE to true and give the pointer

void entity_spawn(
	Sprite *sp,
	Vector2D pos,
	Vector2D vel,
	char *nam,
	float spd,
	SDL_Rect bx,
	int hp,
	int dmg)
{
	Entity *mon = entity_new();
	mon->sprite = sp;
	mon->position = pos;
	mon->velocity = vel;
	mon->name = nam;
	mon->speed = spd;
	mon->box = bx;
	mon->box.x = pos.x;
	mon->box.y = pos.y;
	mon->health = hp;
	mon->damage = dmg;
	mon->grounded = 0;
	slog("Entity created: %c", nam);
	
}

void entity_delete(Entity *entity)
{
	if (!entity)return;
	gf2d_sprite_delete(entity->sprite);
	entity->in_use = 0;
}
//Delete referenced entity

void entity_update()
{
	for (int i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use <= 0) break;
		
		if (entity_manager.entity_list[i].health == 0)  entity_die(&entity_manager.entity_list[i]);

		if (entity_manager.entity_list[i].name == "player attack") entity_manager.entity_list[i].health--;

		if (entity_manager.entity_list[i].name == "enemy")
		{
			for (int j = 0; j < entity_manager.max_entities; j++)
			{
				if (entity_manager.entity_list[j].name != "player" && entity_manager.entity_list[j].name != "player attack") break;
				
				damage(&entity_manager.entity_list[j], &entity_manager.entity_list[i]);
				
			}
		}
	}
	//Check for collision for damage
}

void entity_draw(Sprite *sprite, Vector2D position) 
{
	gf2d_sprite_draw_image(sprite, position);
}

void entity_draw_all()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 1)
		{
			entity_draw(entity_manager.entity_list[i].sprite, entity_manager.entity_list[i].position);
		}
	}
}

//kill an entity
void entity_die(Entity *self)
{
	//play death animation, then 
	slog(self->name);
	slog("has died");
	entity_delete(self);
}

void entity_die_all()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 1)
		{
			entity_die(&entity_manager.entity_list[i]);
		}
	}
}
void entity_system_close()
{
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 1)
		{
			entity_delete(&entity_manager.entity_list[i]);
		}
	}
}

void attack(Entity *spawner)
{
	if (spawner->name = "player")
	{
		if (spawner->velocity.x < 0) entity_spawn(player_slash, vector2d((spawner->position.x - 32), (spawner->position.y + 16)), vector2d(0, 0), "player attack", 1, aox, 20, spawner->damage);
		else entity_spawn(player_slash, vector2d((spawner->position.x + 32), (spawner->position.y + 16)), vector2d(0, 0), "player attack", 1, aox, NULL, spawner->damage);
	}
	else
	{
		//entity_spawn;
	}
}

void damage(Entity *hitee, Entity *hitter)
{
	if (hitee->health != NULL)
	{
		hitee->health -= hitter->damage;
	}
}

void invuln(Entity *bitch, float time)
{

}

void entity_collision(Entity *player)
{
	
	for (int i = 1; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 0)break;

		hcol = 0, vcol = 0;
		//slog("checking...");

		if ((player->position.y + player->box.h) < entity_manager.entity_list[i].position.y)
		{
			//slog("break 3");
		}else vcol++;

		if ((player->position.x + player->box.w) < entity_manager.entity_list[i].position.x)
		{
			//slog("break 1");
			//col = 1;
		}else hcol++;
		
		if ((entity_manager.entity_list[i].position.x + entity_manager.entity_list[i].box.w) < player->position.x)
		{
			//slog("break 2");
			//col = 1;
		}else hcol++;
		
		
		if ((entity_manager.entity_list[i].position.y + entity_manager.entity_list[i].box.h) < player->position.y)
		{
			//slog("break 4");
			//col = 1;
		}else vcol++;
		



		if (vcol <= 1)
		{
			player->grounded = 0;
			slog("found nothing vertically");
			break;
		}

		if (hcol <= 1)
		{
			slog("found nothing horizontally");
			break;
		}


		//slog("touched something");

		if (entity_manager.entity_list[i].name == "wall")
		{

			if ((entity_manager.entity_list[i].position.x + entity_manager.entity_list[i].box.w) < (player->position.x + 1))break;
			//if (entity_manager.entity_list[i].position.x <)


			//if ()break;

				/*
			//slog("touched wall");
			//checks against player moving left into a wall
			if (abs((entity_manager.entity_list[i].position.x + entity_manager.entity_list[i].box.w) - player->position.x) <= 1
				&& (player->velocity.x < 0)) {	player->velocity.x = 0; player->grounded = 1;}

			//checks if a player is moving right into a wall
			if (abs((entity_manager.entity_list[i].position.x - (player->position.x + player->box.w)) <= 1)
				&& (player->velocity.x > 0)) {	player->velocity.x = 0; player->grounded = 1;}

			//checks if a player is moving down into a wall(technically a floor, fuck you)
			if (abs(entity_manager.entity_list[i].position.y - (player->position.y + player->box.h)) <= 1
				&& (player->velocity.y > 0))	{player->velocity.y = 0; player->grounded = 1;}

			//checks if a player is moving up into a wall
			if (abs((entity_manager.entity_list[i].position.y + entity_manager.entity_list[i].box.h) - player->position.y) <= 1
				&& (player->velocity.y < 0)) player->velocity.y = 0;

				*/
		}
		
		if (entity_manager.entity_list[i].name == "enemy" || entity_manager.entity_list[i].name == "e_attack")
		{
			damage(player, &entity_manager.entity_list[i]);
		}
		
	}
}

/*eol@eof*/



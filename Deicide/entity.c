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
		if (entity_manager.entity_list[i].in_use <= 0) continue;

		entity_manager.entity_list[i].box.x = entity_manager.entity_list[i].position.x;
		entity_manager.entity_list[i].box.y = entity_manager.entity_list[i].position.y;

		if (entity_manager.entity_list[i].name == "wall") continue;
		
		if (entity_manager.entity_list[i].health == 0)  entity_die(&entity_manager.entity_list[i]);

		if (entity_manager.entity_list[i].name == "player attack") entity_manager.entity_list[i].health--;

		if (entity_manager.entity_list[i].name == "demon")
		{
			for (int j = 0; j < entity_manager.max_entities; j++)
			{
				if (entity_manager.entity_list[j].name != "player" && entity_manager.entity_list[j].name != "player attack") continue;
				
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
	if (spawner->name == "player")
	{
		if (spawner->velocity.x < 0) entity_spawn(player_slash, vector2d((spawner->position.x - 32), (spawner->position.y + 16)), vector2d(0, 0), "player attack", 1, aox, 200, spawner->damage);
		else entity_spawn(player_slash, vector2d((spawner->position.x + 32), (spawner->position.y + 16)), vector2d(0, 0), "player attack", 1, aox, 200, spawner->damage);
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
		if (hitter->damage == 0)return;
		hitee->health -= hitter->damage;
	}
}

void invuln(Entity *bitch, float time)
{

}

Entity* find_player()
{
	for (int i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].name == "player") return &entity_manager.entity_list[i];
	}

	slog("Player entity not found");
}

void entity_collision()
{

	for (int i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 0)continue;
		if (entity_manager.entity_list[i].name == "wall")continue;

		if (entity_manager.entity_list[i].name == "player")
		{
			for (int j = 0; j < entity_manager.max_entities; j++)
			{
				if (entity_manager.entity_list[j].in_use == 0)continue;
				if (entity_manager.entity_list[j].name == "player")continue;


				//Checking walls for collision
				if (entity_manager.entity_list[j].name == "wall")
				{
					//Checking if the center of the player is within the wall tile below them
					if ((entity_manager.entity_list[i].box.x + (entity_manager.entity_list[i].box.w / 2) <= (entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w)) &&
						entity_manager.entity_list[i].box.x + (entity_manager.entity_list[i].box.w / 2) >= entity_manager.entity_list[j].box.x)
					{
						//Checking if the player is directly above the tile if the first check passed
						if (((entity_manager.entity_list[i].box.y + entity_manager.entity_list[i].box.h + 1) >= entity_manager.entity_list[j].box.y) &&
							entity_manager.entity_list[i].box.y < (entity_manager.entity_list[j].box.y + entity_manager.entity_list[j].box.h))
						{
							if (entity_manager.entity_list[i].grounded == 0)
							{
								if (entity_manager.entity_list[i].position.y + entity_manager.entity_list[i].box.h >= (entity_manager.entity_list[j].position.y))
								{
									entity_manager.entity_list[i].position.y = entity_manager.entity_list[j].position.y - entity_manager.entity_list[i].box.h;
								}

								entity_manager.entity_list[i].grounded = 1;
							}
						}continue;
					} //Checking if the center of the player is within the wall tile beside them
					if ((entity_manager.entity_list[i].box.y + (entity_manager.entity_list[i].box.h / 2) >= (entity_manager.entity_list[j].box.y + entity_manager.entity_list[j].box.h)) &&
						(entity_manager.entity_list[i].box.y + (entity_manager.entity_list[i].box.h / 2) <= entity_manager.entity_list[j].box.y))
					{
						//Check for moving left against a wall
						if ((entity_manager.entity_list[i].box.x - (entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w)) < 0 &&
							(entity_manager.entity_list[i].box.x - (entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w)) > entity_manager.entity_list[j].box.w / 2)
						{
							entity_manager.entity_list[i].position.x = entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w;
							continue;
						}
					}continue;
				}


				if (entity_manager.entity_list[j].name == "demon" || entity_manager.entity_list[j].name == "boss")
				{
					if (entity_manager.entity_list[j].box.x > (entity_manager.entity_list[i].box.x + entity_manager.entity_list[i].box.w))continue;
					if ((entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w) < entity_manager.entity_list[i].box.x)continue;
					if (entity_manager.entity_list[j].box.y > (entity_manager.entity_list[i].box.y + entity_manager.entity_list[i].box.h))continue;
					if ((entity_manager.entity_list[j].box.y + entity_manager.entity_list[j].box.h) < entity_manager.entity_list[i].box.y)continue;

					damage(&entity_manager.entity_list[i], &entity_manager.entity_list[j]);
				}
			}
		}
		
		if (entity_manager.entity_list[i].name == "demon")
		{
			for (int j = 0; j < entity_manager.max_entities; j++)
			{
				if (entity_manager.entity_list[j].in_use == 0)continue;
				if (entity_manager.entity_list[j].name == "demon")continue;

				if (entity_manager.entity_list[j].name == "wall")
				{
					//Checking if demon is directly above wall tile in question
					if ((entity_manager.entity_list[i].box.x + (entity_manager.entity_list[i].box.w / 2) <= (entity_manager.entity_list[j].box.x + entity_manager.entity_list[j].box.w)) &&
						entity_manager.entity_list[i].box.x + (entity_manager.entity_list[i].box.w / 2) >= entity_manager.entity_list[j].box.x)
					{
						//Checking if the demon is directly on the wall tile
						if (((entity_manager.entity_list[i].box.y + entity_manager.entity_list[i].box.h + 1) >= entity_manager.entity_list[j].box.y) &&
							entity_manager.entity_list[i].box.y < (entity_manager.entity_list[j].box.y + entity_manager.entity_list[j].box.h))
						{
							if (entity_manager.entity_list[i].grounded == 0)
							{
								if (entity_manager.entity_list[i].position.y + entity_manager.entity_list[i].box.h >= (entity_manager.entity_list[j].position.y))
								{
									entity_manager.entity_list[i].position.y = entity_manager.entity_list[j].position.y - entity_manager.entity_list[i].box.h;
								}

								entity_manager.entity_list[i].grounded = 1;
							}
						}
					}
				}

				if (entity_manager.entity_list[j].name == "player")
				{

				}
			}
		}

		if (entity_manager.entity_list[i].name == "ghost")
		{
			Entity* player = find_player();

			//Finds the players position and slowly floats towards them
			if (entity_manager.entity_list[i].position.x > player->position.x)
			{
				entity_manager.entity_list[i].position.x--;
			} else
			{
				entity_manager.entity_list[i].position.x++;
			}

			if (entity_manager.entity_list[i].position.y > player->position.y)
			{
				entity_manager.entity_list[i].position.y--;
			} else
			{
				entity_manager.entity_list[i].position.y++;
			}
		}
	}
}


void move_world()
{
	Entity* player = find_player();
	
	//slog("%s", player->name);

	if (player->position.x > 1000) 
		if (player->velocity.x > 0)
		{
			for (int i = 0; i < entity_manager.max_entities; i++)
			{
				entity_manager.entity_list[i].position.x -= player->velocity.x;
			}
		}

	if (player->position.x < 200)
		if (player->velocity.x < 0)
		{
			for (int i = 0; i < entity_manager.max_entities; i++)
			{
				entity_manager.entity_list[i].position.x -= player->velocity.x;
			}
		}
}

/*eol@eof*/



#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"
#include "simple_logger.h"

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
	float spd)
{
	Entity *mon = entity_new();
	mon->sprite = sp;
	mon->position = pos;
	mon->velocity = vel;
	mon->name = nam;
	mon->speed = spd;
	mon->grounded = 0;
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
	int i;
	for (i = 0; i < entity_manager.max_entities; i++)
	{
		if (entity_manager.entity_list[i].in_use == 1 && &entity_manager.entity_list[i].velocity != (0,0))
		{
			entity_manager.entity_list[i].position.x = (entity_manager.entity_list[i].position.x + entity_manager.entity_list[i].velocity.x);
			entity_manager.entity_list[i].position.y = (entity_manager.entity_list[i].position.y + entity_manager.entity_list[i].velocity.y);
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

/*eol@eof*/
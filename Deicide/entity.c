#include "gf2d_vector.h"
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include "entity.h"

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

}

Entity entity_new()
{
	
}
//Check through list, If any entitiy isnt in use, set INUSE to true and give the pointer

void entity_delete(Entity)
{
	inuse = 0;
}
//Delete referenced entity

entity_move(int x, int y)
{
	
}

entity_update();

entity_draw();

/*eol@eof*/
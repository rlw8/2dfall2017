#pragma once
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "map_data.h"
#include <stdio.h>

typedef struct MapData
{
	char name[12];
	short tiles[50][30];
	short width;
	short height;

}MapData;

void map_file_start();

void load_map(const char *name, MapData *new_map);

void set_map(MapData *map, Sprite *sprite);
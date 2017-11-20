#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "map_data.h"
#include <stdio.h>

MapData *map;
FILE *infile;

char key[12];


void map_file_start()
{

	infile = fopen("mapfile.txt", "r");
	if (!infile)return;


}

/*
void load_map(char *map_name)
{
	fscanf(infile, "%s", key);
	switch (key[0])
	{
		case'm':
			if (!strcmp(key, "map"))
				*map = store_map(map_name);
	}
}
*/

MapData load_map(char *name)
{
	MapData new_map;
	char seeking = 1;
	char c;
	short value;
	fseek(infile, 0, SEEK_SET);
	
	while (seeking)
	{
		fscan(infile, "%s", key);
			if(!strcmp(key, name))seeking = 0;
	}

	new_map.width = fscan(infile, "%d", key);
	new_map.height = fscan(infile, "%d", key);

	for (int i = 0; i < new_map.height; i++)
	{
		for (int j = 0; j < new_map.width; j++)
		{
			c = fgetc(infile);
			value = c - '0';
			if (c == '\n')break;
			if (value < 0 || value > 5) slog("wrong data type loaded in map");

			new_map.times[i][j] = c;
		}
	}

	return new_map;
}

void set_map(MapData *map, Sprite *sprite)
{
	Vector2D tilepos;
	SDL_Rect standard_box = { NULL, NULL, 32, 32 };

	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			tilepos = vector2d(j * 32, i * 32);
			entity_spawn(sprite, tilepos, vector2d(0, 0), "wall", 1, standard_box, 1000, NULL);
		}
	}
}
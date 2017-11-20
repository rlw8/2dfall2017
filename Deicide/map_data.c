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

	//fclose("mapfile.txt");
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

void load_map(const char *name, MapData *new_map)
{
	char c;
	short value;
	fseek(infile, 0, SEEK_SET);
	
	while (true)
	{
		fscanf(infile, "%s", key);
			if(!strcmp(key, name))break;
	}


	fscanf(infile, "%d", &new_map->width);
	fscanf(infile, "%d", &new_map->height);
	if (fgetc(infile) == '\n');
	else slog("Error in file after height declared");

	slog("%c",new_map->height);
	slog("%c",new_map->width);

	for (short i = 0; i <= new_map->height; i++)
	{
		for (short j = 0; j <= new_map->width; j++)
		{
			c = fgetc(infile);
			value = c - '0';
			if (c == '\n')break;
			if (value < 0 || value > 5) 
			{
				slog("wrong data type loaded in map"); 
				continue;
			}
			else new_map->tiles[i][j] = c;
		}
	}

	//return;
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

			if (map->tiles[i][j] == '1')
			{
				entity_spawn(sprite, tilepos, vector2d(0, 0), "wall", 1, standard_box, 1000, NULL);
			}
		}
	}
}
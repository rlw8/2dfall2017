#include <SDL.h>
#include <SDL_mixer.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "player.h"
#include "gf2d_vector.h"
#include "map_data.h"
#include "physfs.h"

#define IMPULSE_LEFT 1
#define IMPULSE_RIGHT 2
#define IMPULSE_JUMP 4
#define IMPULSE_ATTACK 8

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
	int impulses = 0;
    const Uint8 * keys;
	char pbool = 0, pause = 0;
    Sprite *sprite, *sprite_torb, *sprite_player, *sp_brick, *player_slash, *sp_demon, *boss_hand_r, *boss_hand_l, *boss1, *ghost;
	SDL_Event event;
	Entity *player;
	Mix_Music *music = NULL;
	SDL_Rect boxx = { NULL, NULL, 32, 32 }, plox = { NULL, NULL, 28, 32 }, bboxx = { NULL, NULL, 64, 64 }, hbox = { NULL, NULL, 128, 128 };
	MapData castle;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Deicide",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(15);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);

	PHYSFS_init(NULL);


	//Initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO) == -1)
		slog("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	

    /*demo setup*/
	entity_system_init(1024);
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	//sprite_torb = gf2d_sprite_load_image("images/torb space.png");
	sprite_player = gf2d_sprite_load_image("images/trashe 2.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	sp_brick = gf2d_sprite_load_image("images/Brick1.png");
	player_slash = gf2d_sprite_load_image("images/player slash.png");
	sp_demon = gf2d_sprite_load_image("images/demon man.png");
	boss_hand_r = gf2d_sprite_load_image("images/bosshandr.png");
	boss_hand_l = gf2d_sprite_load_image("images/bosshandl.png");
	boss1 = gf2d_sprite_load_image("images/hairboss.png");
	ghost = gf2d_sprite_load_image("images/ghost.png");


	player = entity_new();

	player_spawn(player, sprite_player, vector2d(300, 100), vector2d(4, 0), "player", 1, boxx);
	player->speed = 1;

	
	entity_spawn(boss1, vector2d(400, 200), vector2d(0,0), "boss", 1, hbox, 100, 30);

	entity_spawn(boss_hand_r, vector2d(420, 200), vector2d(0, 0), "boss_hand", 1, boxx, 100, 30);

	//entity_spawn(sp_brick, vector2d(100, 350), vector2d(0, 0), "wall", 1, boxx, 1000, NULL);
	//entity_spawn(sp_brick, vector2d(132, 350), vector2d(0, 0), "wall", 1, boxx, 1000, NULL);
	//entity_spawn(sp_brick, vector2d(164, 350), vector2d(0, 0), "wall", 1, boxx, 1000, NULL);
	//entity_spawn(sp_brick, vector2d(164, 318), vector2d(0, 0), "wall", 1, boxx, 1000, NULL);

	entity_spawn(sp_demon, vector2d(200, 600), vector2d(2,0), "demon", 1, bboxx, 20, 20);

	entity_spawn(ghost, vector2d(600, 300), vector2d(0, 0), "ghost", 1, boxx, 10, 10);

	//entity_spawn(sprite_torb, vector2d(200, 100), vector2d(2,0), "torb");

	map_file_start();
	load_map("castle", &castle);
	set_map(&castle, sp_brick);

	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return false;
	
	music = Mix_LoadMUS("audio/You will be worse.mp3");

	if(music == NULL)
		slog("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(6);
	

    /*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		//impulses = 0;
		while (SDL_PollEvent(&event));
		{
			switch (event.type)
			{
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_RIGHT:
					impulses &= ~IMPULSE_RIGHT;
					break;

				case SDLK_LEFT:
					impulses &= ~IMPULSE_LEFT;
					break;

				case SDLK_ESCAPE:
					if (pause == 1) pbool = 1;

					if (pause == 0) pbool = 0;
					break;

					/*
				case SDLK_SPACE:
					impulses &= ~IMPULSE_JUMP;
					break; */
				}
				break;

			case SDL_KEYDOWN:

				//if (event.key.repeat)
					//continue;

				switch (event.key.keysym.sym)
				{

				case SDLK_LCTRL:
					//slog("attack pressed");
					impulses |= IMPULSE_ATTACK;
					break;

				case SDLK_RIGHT:
					impulses |= IMPULSE_RIGHT;
					//move(player, 1);
					break;

				case SDLK_LEFT:
					impulses |= IMPULSE_LEFT;
					//move(player, -1);
					break;

				case SDLK_SPACE:
					impulses |= IMPULSE_JUMP;
					//slog("space pressed");
					break;

				case SDLK_UP:
					impulses |= IMPULSE_JUMP;
					//slog("up pressed");
					break;

				case SDLK_ESCAPE:
					if (pause == 0) pause = 1;
					
					if (pbool == 1) pause = 0;
					break;


				}
			}
		}

		

		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;

		//entity_update();
		if (!pause) 
		{
			//physics(player);
			entity_collision();
			move_world();
			entity_update();

			if (impulses & IMPULSE_LEFT)
			{
				if (impulses & IMPULSE_RIGHT)
				{

				}
				else
				{
					move(player, -1);
				}
			}
			else
			{
				if (impulses & IMPULSE_RIGHT)
				{
					move(player, 1);
				}
			}

			if (impulses & IMPULSE_JUMP)
			{
				slog("Player jumped");
				jump(player);
				impulses &= ~IMPULSE_JUMP;
			}

			if (impulses & IMPULSE_ATTACK)
			{
				slog("Player attacked");
				attack(player);
				impulses &= ~IMPULSE_ATTACK;
			}
		}
		else 
		{
			//do pause menu stuff
		}

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
			
			entity_draw_all();
            
            //UI elements last

			/*
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),   
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
			*/

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_F4])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");

	//map_close(castle);

    return 0;	
}
/*eol@eof*/

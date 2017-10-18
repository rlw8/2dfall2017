#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "entity.h"
#include "player.h"
#include "gf2d_vector.h"

#define IMPULSE_LEFT 1
#define IMPULSE_RIGHT 2
#define IMPULSE_JUMP 3

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
	int impulses = 0;
    const Uint8 * keys;
    Sprite *sprite, *sprite_torb, *sprite_player;
	SDL_Event event;
	Entity *player;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(15);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
	entity_system_init(1024);
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	//sprite_torb = gf2d_sprite_load_image("images/torb space.png");
	sprite_player = gf2d_sprite_load_image("images/torb space.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);

	player = entity_new();

	player_spawn(player, sprite_player, vector2d(100, 100), vector2d(4, 3), "player", 1);
	player->speed = 1;

	//entity_spawn(sprite_torb, vector2d(200, 100), vector2d(2,0), "torb");


    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		//impulses = 0;
		while(SDL_PollEvent(&event));
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

					case SDLK_SPACE:
						impulses &= ~IMPULSE_JUMP;
						break;
				}
				break;

			case SDL_KEYDOWN:
				
				//if (event.key.repeat)
					//continue;

				switch (event.key.keysym.sym)
				{
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
						break;

					case SDLK_j:
						//entity_die_all();
						break;
				}
			}
		}

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
			jump(player);
		}

        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
		//entity_update();
		physics(player);
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
			
			entity_draw_all();
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);


        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

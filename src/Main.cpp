//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <ctime>

#include "Const.h"
#include "GameLoop.h"
#include "LTexture.h"
#include "GameState.h"
#include "Menu.h"

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

GameState *currentState = NULL;

//Main loop flag
bool quit = false;

/*GameState Stuff*/

void change_state()
{
    //If the state needs to be changed
    if( nextState != STATE_NULL )
    {
        //Delete the current state
        if( nextState != STATE_EXIT )
        {
            delete currentState;
        }

        //Change the state
        switch( nextState )
        {
            case STATE_MENU:
                currentState = new Menu();
                break;

            case STATE_GAME:
                currentState = new GameLoop();
                break;

            case STATE_EXIT:
            	quit = true;

        }

        //Change the current state ID
        stateID = nextState;

        //NULL the next state ID
        nextState = STATE_NULL;
    }
}
/*End GameState Stuff*/

bool init()
{
	//Initialization flag
	bool success = true;

	//Randomize rand seed
	srand(time(NULL));

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

				 //Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
				}

			}
		}
	}
	return success;
}


//NOTE: Need to think about better ways for managing multiple fonts
// With one gFont pointer, the last opened font overrides all the others
bool loadMedia()
{
	//Loading success flag
	bool success = true;

        //Open the font
        gFont = TTF_OpenFont( "../assets/PressStart2P.ttf", 6 );
        if( gFont == NULL )
        {
            printf( "Failed to load fps font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }

	return success;
}

void close()
{
	delete currentState;

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if ( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
    	//Set the current state ID
    	stateID = STATE_MENU;

        //Initialize Minesweeper Field
        currentState = new Menu();

		//Event handler
		SDL_Event e;

		//Set text color as green
		SDL_Color textColor = { 0, 0xFF, 0, 0xFF};

	    //The frames per second timer
	    LTimer fpsTimer;

	    //The frames per second cap timer
	    LTimer capTimer;

	    //In memory text stream
	    std::stringstream timeText;

	    //Scene textures
	    LTexture gFPSTextTexture;

        //Start counting frames per second
        int countedFrames = 0;
        fpsTimer.start();


		//While application is running
/**-----[GAME LOOP START!]-----**/
		while( !quit )
		{
			SDL_ShowCursor(SDL_DISABLE);

	        //Start cap timer
	        capTimer.start();

/**-----[INPUT EVENTS]-----**/
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }

                if ( !currentState->gameOver){
                    currentState->handleEvent( &e );
                }

            }
/**-----[LOGIC HANDLING]-----**/
            currentState->logic();
        	
        	//Change state if needed
        	change_state();

	        //Calculate and correct fps
	        float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
	        if( avgFPS > 2000000 )
	        {
	            avgFPS = 0;
	        }
        	
/**-----[RENDERING]-----**/

	        //Set text to be rendered
	        timeText.str( "" );
	        timeText << "FPS: " << avgFPS; 

	        //Render text
	        if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
	        {
	            printf( "Unable to render FPS texture!\n" );
	        }

            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gRenderer );

            //Render Grid
            currentState->render();

	        //Render textures
	        if (DBG_SHOW_FPS)
	        	gFPSTextTexture.render( ( SCREEN_WIDTH - gFPSTextTexture.getWidth() ), gFPSTextTexture.getHeight() / 2 );

            //Update screen
            SDL_RenderPresent( gRenderer );
			++countedFrames;

	        //If frame finished early
	        int frameTicks = capTimer.getTicks();
	        if( frameTicks < SCREEN_TICK_PER_FRAME )
	        {
	            //Wait remaining time
	            SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        	}

        }
	}

	//Free resources and close SDL
	close();

	return 0;
}

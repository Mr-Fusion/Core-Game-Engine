#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "Const.h"
#include "GameState.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Player.h"

#define DEFAULT_LIVES       3

class GameLoop : public GameState
{
    public:

    // Gameplay Stats/Values
    int currentLev;
    int lives;
    int score;

    // Gameplay Event Flags
    bool levelBegin;
    bool showLvl;
    bool gameOver;

    // Player Input Control Flags
    bool lInput,rInput,uInput,dInput;

    SDL_Color textColor;

    // Background color
    int bgR, bgG, bgB;

    // Sprite color
    int spR, spG, spB;

    // Scene textures
    LTexture livesTextTexture;
    LTexture lvlTextTexture;
    LTexture scoreTextTexture;

    // Timers for flag handling
    LTimer delayTimer;

    // Sound Effects
    Mix_Chunk *sfx_alienShot = NULL;
    Mix_Chunk *sfx_playerShot = NULL;
    Mix_Chunk *sfx_alienHitA = NULL;
    Mix_Chunk *sfx_alienHitB = NULL;
    Mix_Chunk *sfx_playerHit = NULL;

    // Game Entities
    SDL_Rect field;
    Player player;

    ///Constructor Function
    GameLoop(){

        currentLev = 0;

        showLvl = false;
        gameOver = false;

        bgR = bgG = bgB = 0x00;

        spR = spG = spB = 0xFF;

        textColor = { spR, spG, spB, 0xFF};

        field.x = 0;
        field.y = 0;
        field.w = SCREEN_WIDTH;
        field.h = SCREEN_HEIGHT;

        lInput = rInput = uInput = dInput = false;

        lives = 0;
        score = 0;

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Initialize playing field dimensions, difficulty, and appearance
            startGame();

            //Initialize and display graphical interface
            SDL_SetWindowSize(gWindow,SCREEN_WIDTH,SCREEN_HEIGHT);
        }

    }

    ///Deconstructor
    ~GameLoop(){
        printf("Gamestate Object Deconstructing...\n");

        //Free the sound effects
        Mix_FreeChunk( sfx_playerShot );
        sfx_playerShot = NULL;
        Mix_FreeChunk( sfx_alienShot );
        sfx_alienShot = NULL;
        Mix_FreeChunk( sfx_alienHitA );
        sfx_alienHitA = NULL;
        Mix_FreeChunk( sfx_alienHitB );
        sfx_alienHitB = NULL;
        Mix_FreeChunk( sfx_playerHit );
        sfx_playerHit = NULL;

        //Free loaded image
        livesTextTexture.free();
        lvlTextTexture.free();
        scoreTextTexture.free();
    }

    void startGame()
    {
        //Initialization goes here
        lives = DEFAULT_LIVES;
        score = 0;
        livesTextTexture.loadFromRenderedText( updateText("Lives: ", lives), textColor);
        scoreTextTexture.loadFromRenderedText( updateText("Score: ", score), textColor);
        goNextLevel();
    }

    // Function which streamlines the loading of sound chunks.
    bool loadSound(Mix_Chunk **chunk, std::string path){
        bool success = true;
        *chunk = Mix_LoadWAV(path.c_str());
        if ( chunk == NULL) {
            printf( "Failed to load %s! SDL_mixer Error: %s\n", path, Mix_GetError() );
            success = false;
        }
        return success;
    }

    //TODO: Can we streamline the sprite sheet creation into a function?
    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sound effects
        if ( !loadSound(&sfx_playerShot,"../assets/sfx_player_shot.wav" ) );
            success = false;
        if ( !loadSound(&sfx_alienShot,"../assets/sfx_alien_shot.wav" ) );
            success = false;
        if ( !loadSound(&sfx_alienHitA,"../assets/sfx_alien_hitA.wav" ) );
            success = false;
        if ( !loadSound(&sfx_alienHitB,"../assets/sfx_alien_hitB.wav" ) );
            success = false;
        if ( !loadSound(&sfx_playerHit,"../assets/sfx_player_hit.wav" ) );
            success = false;

        //Set text to be rendered
        msgText.str( " " );
        livesText.str( " " );

        //Render text
        livesTextTexture.loadFromRenderedText( updateText("Lives: ", lives), textColor);

        lvlTextTexture.loadFromRenderedText( updateText("Mission: ", currentLev), textColor);

        return success;
    }

    // Generic function for updating text strings with integers potentially appended at the end
    std::string updateText(std::string text, int num = -1){
        std::stringstream result;

        result.str("");
        result << text;

        if (num != -1)
            result << num;

        return result.str();
    }

    void goNextLevel(){

        // Prepare for next level here
        currentLev++;
        initLevel();
    }

    void initLevel() {

        // Update level string and display text on screen
        lvlTextTexture.loadFromRenderedText( updateText("Mission: ", currentLev), textColor);
        showLvl = true;

        // Set countdown to hide level text some time after level begins
        delayTimer.start();
        levelBegin = true;

    }

    ///Handles Player input
    void handleEvent( SDL_Event* e){

        int x, y;

        //Get mouse position
        if( e->type == SDL_MOUSEMOTION ){
            SDL_GetMouseState( &x, &y );
        }


        if( e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONUP ){
            set_next_state(STATE_MENU);
        }

        if (e->type == SDL_KEYDOWN) {
            switch (e->key.keysym.sym) {
                case SDLK_a:
                    lInput = true;
                break;

                case SDLK_d:
                    rInput = true;
                break;

                case SDLK_w:
                    uInput = true;
                break;

                case SDLK_s:
                    dInput = true;
                break;
                case SDLK_ESCAPE:
                    set_next_state(STATE_MENU);
                break;
            }
        }


        if (e->type == SDL_KEYUP) {
            switch (e->key.keysym.sym) {
                case SDLK_a:
                    lInput = false;
                break;

                case SDLK_d:
                    rInput = false;
                break;

                case SDLK_w:
                    uInput = false;
                break;

                case SDLK_s:
                    dInput = false;
                break;
            }
        }
        
    }

    // Main Game Loop logic flow
    void logic(){

        if (!gameOver){
            // Player Input/Control Logic
            if (lInput)
                player.moveLeft();

            if (rInput)
                player.moveRight();

            if (uInput)
                player.moveUp();

            if (dInput)
                player.moveDown();
        }

        if (levelBegin){
            if (delayTimer.getTicks() > 3000){
                delayTimer.stop();
                showLvl = false;
                levelBegin = false;
            }
        }
    }

    void render(){

        // Set background color and fill
        SDL_SetRenderDrawColor( gRenderer, bgR, bgG, bgB, 0xFF );
        SDL_RenderFillRect(gRenderer, &field);

        // Set sprite color
        SDL_SetRenderDrawColor( gRenderer, spR, spG, spB, 0xFF );
        player.render();

        // Update Text color and render
        livesTextTexture.setColor(spR, spG, spB);
        livesTextTexture.render(SCREEN_WIDTH - livesTextTexture.getWidth(), 1 );

        scoreTextTexture.setColor(spR, spG, spB);
        scoreTextTexture.render(5, 1 );

        if (showLvl) {
            lvlTextTexture.setColor(spR, spG, spB);
            lvlTextTexture.render(SCREEN_WIDTH/2 - lvlTextTexture.getWidth()/2, SCREEN_HEIGHT - lvlTextTexture.getHeight() * 2 );
        }

    }

};

#endif // GAMELOOP_H_INCLUDED
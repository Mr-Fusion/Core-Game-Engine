#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "Button.h"

enum {
    MENU_START_GAME = 0,
    MENU_SETTINGS = 1,
    MENU_QUIT = 2,
    MENU_SIZE = 3
};

class Menu : public GameState
{
    public:

    bool retInput;
    int curSelection;

    std::stringstream credit;

    LTexture titleText;
    LTexture startText;
    LTexture settingText;
    LTexture quitText;
    LTexture creditText;

    SDL_Color textColor;

    ///Constructor Function
    Menu(){

        retInput = false;
        curSelection = 0;

        textColor = { 0xFF, 0xFF, 0xFF };

        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
        	//Initialize Menu here

        	SDL_SetWindowSize(gWindow,SCREEN_WIDTH, SCREEN_HEIGHT);
        }

    }

    ///Deconstructor
    ~Menu(){
        printf("Menu Object Deconstructing...\n");

		//Destroy Menu assets here

        //Free loaded image
        titleText.free();
        startText.free();
        settingText.free();
        quitText.free();
        creditText.free();

    }

    bool loadMedia()
    {
        //Loading success flag
        bool success = true;

        //Load sprite sheet texture



        //Open the font
        gFont = TTF_OpenFont( "../assets/SAMSRG__.TTF", 60 );
        if( gFont == NULL )
        {
            printf( "Failed to load Pong Title font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            if( !titleText.loadFromRenderedText( TITLE, textColor ) )
            {
                printf( "Failed to render title text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/PressStart2P.ttf", 14 );
        if( gFont == NULL )
        {
            printf( "Failed to load Minesweeper font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            //Set text to be rendered
            credit.str( "" );
            credit << "By Joel Turner | Version: " << MAJOR_REV << "." << MINOR_REV << "." << INCREMENTAL_REV;

            //Render text
            if( !creditText.loadFromRenderedText( credit.str().c_str(), textColor ) )
            {
                printf( "Failed to render credit text!\n" );
                success = false;
            }
        }

        gFont = TTF_OpenFont( "../assets/SAMST___.TTF", 40 );
        if( gFont == NULL )
        {
            printf( "Failed to load Pong font! SDL_ttf Error: %s\n", TTF_GetError() );
            success = false;
        }
        else
        {
            //Render text
            if( !startText.loadFromRenderedText( "Start Game", textColor ) )
            {
                printf( "Failed to render startText!\n" );
                success = false;
            }
            if( !settingText.loadFromRenderedText( "Settings", textColor ) )
            {
                printf( "Failed to render seetingText!\n" );
                success = false;
            }
            if( !quitText.loadFromRenderedText( "Quit", textColor ) )
            {
                printf( "Failed to render quitText!\n" );
                success = false;
            }
        }
        return success;
    }
    
    ///Handles mouse event
    void handleEvent( SDL_Event* e){

        if (e->type == SDL_KEYDOWN) {
            switch (e->key.keysym.sym) {
                case SDLK_RETURN:
                    retInput = true;
                break;
                case SDLK_w:
                    curSelection--;
                    if (curSelection < 0)
                        curSelection = MENU_SIZE - 1;
                break;

                case SDLK_s:
                    curSelection++;
                    if (curSelection >= MENU_SIZE )
                        curSelection = MENU_START_GAME;
                break;
            }
        }
    }

    void logic(){

        // Set all options to white text by default
        startText.setColor(0xFF, 0xFF, 0xFF);
        settingText.setColor(0xFF, 0xFF, 0xFF);
        quitText.setColor(0xFF, 0xFF, 0xFF);

        // Highlight the appropriate option in red corresponding to the current cursor selection
        switch (curSelection) {
            case MENU_START_GAME:
                startText.setColor(0xFF, 0x00, 0x00);
            break;
            case MENU_SETTINGS:
                settingText.setColor(0xFF, 0x00, 0x00);
            break;
            case MENU_QUIT:
                quitText.setColor(0xFF, 0x00, 0x00);
            break;
        }

        if (retInput){
            switch (curSelection) {
                case MENU_START_GAME:
                    set_next_state( STATE_GAME );
                break;
                case MENU_SETTINGS:
                    //set_next_state( int newState );
                break;
                case MENU_QUIT:
                    set_next_state( STATE_EXIT );
                break;
            }
            retInput = false;
        }
    }

    void render(){

        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        // SDL_RenderFillRect(gRenderer, &paddle);

        titleText.render(SCREEN_WIDTH/2 - titleText.getWidth()/2, 30);
        startText.render(SCREEN_WIDTH/2 - startText.getWidth()/2, SCREEN_HEIGHT/2 - startText.getHeight() );
        settingText.render(SCREEN_WIDTH/2 - settingText.getWidth()/2, SCREEN_HEIGHT/2);
        quitText.render(SCREEN_WIDTH/2 - quitText.getWidth()/2, SCREEN_HEIGHT/2 + quitText.getHeight());
        creditText.render(SCREEN_WIDTH/2 - creditText.getWidth()/2, SCREEN_HEIGHT - 60);
    }

};

#endif // MENU_H_INCLUDED
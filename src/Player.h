#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "Const.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Bullet.h"

#define SHIP_HEIGHT		8
#define SHIP_WIDTH		60
#define SHIP_VELOCITY		5

#define BULLET_HEIGHT       8
#define BULLET_WIDTH        2
#define BULLET_VELOCITY     -10

class Player
{
    public:

    	SDL_Rect shipDim, bulDim;

        SDL_Point bulVel;

    ///Constructor Function
    Player(){

    	shipDim.h = SHIP_HEIGHT;
    	shipDim.w = SHIP_WIDTH;
    	shipDim.x = SCREEN_WIDTH/2 - SHIP_WIDTH/2;
    	shipDim.y = SCREEN_HEIGHT - SHIP_HEIGHT * 2;

        bulDim.h = BULLET_HEIGHT;
        bulDim.w = BULLET_WIDTH;
        bulDim.x = 0;
        bulDim.y = 0;

        bulVel.x = 0;
        bulVel.y = BULLET_VELOCITY;

    }

    ///Deconstructor
    ~Player(){
        printf("Gamestate Object Deconstructing...\n");


    }

    bool checkCollision( SDL_Rect foreignObj){
        if (shipDim.y > ( foreignObj.y + foreignObj.h ) )
            return false;

        if (shipDim.y + shipDim.h < foreignObj.y)
            return false;
                
        if (shipDim.x + shipDim.w < foreignObj.x)
            return false;
                    
        if (shipDim.x > foreignObj.x + foreignObj.w)
            return false;

        return true;

    }

    Bullet* shoot() {
        bulDim.x = shipDim.x + shipDim.w/2;
        bulDim.y = shipDim.y;
        return new Bullet(bulDim,bulVel);
        //play sound effect?
        //graphical effects?
    }

    SDL_Rect getDim() {
        return shipDim;
    }

    void moveLeft(){
    	shipDim.x -= SHIP_VELOCITY;
    	if (shipDim.x < 0)
    		shipDim.x = 0;
    }

    void moveRight() {
        shipDim.x += SHIP_VELOCITY;
        if (shipDim.x > SCREEN_WIDTH - shipDim.w)
            shipDim.x = SCREEN_WIDTH - shipDim.w;
    }

    void moveUp(){
        shipDim.y -= SHIP_VELOCITY;
        if (shipDim.y < 0)
            shipDim.y = 0;
    }

    void moveDown() {
        shipDim.y += SHIP_VELOCITY;
        if (shipDim.y > SCREEN_HEIGHT - shipDim.h)
            shipDim.y = SCREEN_HEIGHT - shipDim.h;
    }

    void render(){

    	SDL_RenderFillRect(gRenderer, &shipDim);
    }

};
#endif // PLAYER_H_INCLUDED
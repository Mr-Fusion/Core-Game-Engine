#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "GameEntity.h"
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

class Player : public GameEntity
{
    public:

    	SDL_Rect dim, bulDim;

        SDL_Point bulVel;

    ///Constructor Function
    Player(){

    	dim.h = SHIP_HEIGHT;
    	dim.w = SHIP_WIDTH;
    	dim.x = SCREEN_WIDTH/2 - SHIP_WIDTH/2;
    	dim.y = SCREEN_HEIGHT - SHIP_HEIGHT * 2;

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

    Bullet* shoot() {
        bulDim.x = dim.x + dim.w/2;
        bulDim.y = dim.y;
        return new Bullet(bulDim,bulVel);
        //play sound effect?
        //graphical effects?
    }

    void moveLeft(){
    	dim.x -= SHIP_VELOCITY;
    	if (dim.x < 0)
    		dim.x = 0;
    }

    void moveRight() {
        dim.x += SHIP_VELOCITY;
        if (dim.x > SCREEN_WIDTH - dim.w)
            dim.x = SCREEN_WIDTH - dim.w;
    }

    void moveUp(){
        dim.y -= SHIP_VELOCITY;
        if (dim.y < 0)
            dim.y = 0;
    }

    void moveDown() {
        dim.y += SHIP_VELOCITY;
        if (dim.y > SCREEN_HEIGHT - dim.h)
            dim.y = SCREEN_HEIGHT - dim.h;
    }

};
#endif // PLAYER_H_INCLUDED
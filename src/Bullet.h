#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "Const.h"
#include "LTimer.h"
#include "LTexture.h"

class Bullet
{
    public:

    	SDL_Rect dim;

        SDL_Point vel;

    ///Constructor Function
    Bullet(SDL_Rect iDim, SDL_Point iVel){

        dim = iDim;
        vel = iVel; //Placeholder

    }

    ///Deconstructor
    ~Bullet(){
        printf("Bullet Object Deconstructing...\n");


    }

    SDL_Rect getDim() {
        return dim;
    }

    void hit() {
        //play sound effect or trigger more logic here
    }

    bool offScreen() {
        if (dim.y + dim.h < 0)
            return true;
        if (dim.y > SCREEN_HEIGHT)
            return true;
        if (dim.x + dim.w < 0)
            return true;
        if (dim.x > SCREEN_WIDTH)
            return true;
        return false;
    }

    bool checkCollision( SDL_Rect foreignObj){
        if (dim.y > ( foreignObj.y + foreignObj.h ) )
            return false;

        if (dim.y + dim.h < foreignObj.y)
            return false;
                
        if (dim.x + dim.w < foreignObj.x)
            return false;
                    
        if (dim.x > foreignObj.x + foreignObj.w)
            return false;

        return true;

    }

    void move(){

        dim.y += vel.y;
        dim.x += vel.x;

    }

    void render(){

    	SDL_RenderFillRect(gRenderer, &dim);
    }

};
#endif // BULLET_H_INCLUDED
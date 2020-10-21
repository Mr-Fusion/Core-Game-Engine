#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "GameEntity.h"
#include "Const.h"
#include "LTimer.h"
#include "LTexture.h"

class Bullet : public GameEntity
{
    public:

    ///Constructor Function
    Bullet(SDL_Rect iDim, SDL_Point iVel){

        dim = iDim;
        vel = iVel; //Placeholder

    }

    ///Deconstructor
    ~Bullet(){
        printf("Bullet Object Deconstructing...\n");
    }

    void hit() {
        //play sound effect or trigger more logic here
    }

};
#endif // BULLET_H_INCLUDED
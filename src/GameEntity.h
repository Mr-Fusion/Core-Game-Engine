#ifndef GAMEENTITY_H_INCLUDED
#define GAMEENTITY_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
#include "Const.h"
#include "LTexture.h"

class GameEntity
{
    public:

    	SDL_Rect dim;
        SDL_Point vel;

    ///Deconstructor
    virtual ~GameEntity(){}

    // Set/Get entity dimensions (x,y,w,h)
    void setDim(SDL_Rect d) { dim = d; }
    SDL_Rect getDim() { return dim; }

    // Set/Get entity velocity (x,y)
    void setVel(int x, int y){ vel.x = x; vel.y = y; }
    SDL_Point getVel() { return vel; }

    // Set entity position
    void setPos(int x, int y){
        dim.x = x;
        dim.y = y;
    }

    // Update location coordiates based on current velocity
    void move(){
        dim.y += vel.y;
        dim.x += vel.x;
    }

    // Reports whether entity is entirely off the screen.
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

    // Reports whether entity dimensions overlap a specified recangular area
    bool checkCollision( SDL_Rect foreignObj){
        if (dim.y >= ( foreignObj.y + foreignObj.h ) )
            return false;
        if (dim.y + dim.h <= foreignObj.y)
            return false;
        if (dim.x + dim.w <= foreignObj.x)
            return false;
        if (dim.x >= foreignObj.x + foreignObj.w)
            return false;
        return true;
    }

    // Display the entity on the screen
    void render(){ SDL_RenderFillRect(gRenderer, &dim); }

};
#endif // GAMEENTITY_H_INCLUDED
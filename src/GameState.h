#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

//Game state base class

enum GameStates
{
    STATE_NULL,
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
};

//State variables
int stateID = STATE_NULL;
int nextState = STATE_NULL;


class GameState
{
    public:
    virtual void handleEvent(SDL_Event* e) = 0;
    virtual void logic() = 0;
    virtual void render() = 0;
    virtual ~GameState(){};

    //remove this
    bool gameOver = 0;

    void set_next_state( int newState )
    {
        //If the user doesn't want to exit
        if( nextState != STATE_EXIT )
        {
            //Set the next state
            nextState = newState;
        }
    }
};

#endif // GAMESTATE_H_INCLUDED
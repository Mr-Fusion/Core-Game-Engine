#ifndef LTIMER_H_INCLUDED
#define LTIMER_H_INCLUDED

#include <SDL.h>
#include <stdlib.h>
//#include <Const.h>

//The application time based timer
class LTimer
{
    public:


		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;

    LTimer()
    {
        //Initialize the variables
        mStartTicks = 0;
        mPausedTicks = 0;

        mPaused = false;
        mStarted = false;
    }

    void start()
    {
        //Start the timer
        mStarted = true;

        //Unpause the timer
        mPaused = false;

        //Get the current clock time
        mStartTicks = SDL_GetTicks();
    	mPausedTicks = 0;
    }

    void stop()
    {
        //Stop the timer
        mStarted = false;

        //Unpause the timer
        mPaused = false;

    	//Clear tick variables
    	mStartTicks = 0;
    	mPausedTicks = 0;
    }

    void pause()
    {
        //If the timer is running and isn't already paused
        if( mStarted && !mPaused )
        {
            //Pause the timer
            mPaused = true;

            //Calculate the paused ticks
            mPausedTicks = SDL_GetTicks() - mStartTicks;
    		mStartTicks = 0;
        }
    }

    void unpause()
    {
        //If the timer is running and paused
        if( mStarted && mPaused )
        {
            //Unpause the timer
            mPaused = false;

            //Reset the starting ticks
            mStartTicks = SDL_GetTicks() - mPausedTicks;

            //Reset the paused ticks
            mPausedTicks = 0;
        }
    }

    Uint32 getTicks()
    {
    	//The actual timer time
    	Uint32 time = 0;

        //If the timer is running
        if( mStarted )
        {
            //If the timer is paused
            if( mPaused )
            {
                //Return the number of ticks when the timer was paused
                time = mPausedTicks;
            }
            else
            {
                //Return the current time minus the start time
                time = SDL_GetTicks() - mStartTicks;
            }
        }

        return time;
    }

    bool isStarted()
    {
    	//Timer is running and paused or unpaused
        return mStarted;
    }

    bool isPaused()
    {
    	//Timer is running and paused
        return mPaused && mStarted;
    }

};



#endif // LTIMER_H_INCLUDED

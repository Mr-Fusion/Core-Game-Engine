#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//Globally used font
extern TTF_Font *gFont = NULL;

#define DBG_SHOW_FPS        false

#define TITLE				"Game_Title"

#define MAJOR_REV           0
#define MINOR_REV           0
#define INCREMENTAL_REV     0

//Screen dimension constants
#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480
#define SCREEN_FPS          60

const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

#endif // CONST_H_INCLUDED

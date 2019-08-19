#ifndef LTEXTURE_H_INCLUDED
#define LTEXTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <cmath>
#include <stdio.h>
#include <string>


//Texture wrapper class
class LTexture
{
	public:
        //The actual hardware texture and image dimensions
		SDL_Texture* tex;
		int w;
		int h;

		//Initializes variables
		LTexture(){
            tex = NULL;
            w = 0;
            h = 0;
		}

		//Deallocates memory
		~LTexture(){
            printf("LTexture Object Deconstructing...\n");
            free();
        }

		//Loads image at specified path
		bool loadFromFile( std::string path){
            free();     //Get rid of preexisting texture
            SDL_Texture* newTexture = NULL;     //The final texture
            SDL_Surface* loadedSurface = IMG_Load( path.c_str() );      //Load image at specified path
            if( loadedSurface == NULL ) {
                printf( "Unable to load image %s! SDL_image Error: %s\n"
                       , path.c_str(), IMG_GetError() );
            } else {
                SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format
                    , 0, 0xFF, 0xFF ) );    //Color key image
                newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );   //Create texture from surface pixels
                if( newTexture == NULL ) {
                    printf( "Unable to create texture from %s! SDL Error: %s\n"
                           , path.c_str(), SDL_GetError() );
                } else {    //Get image dimensions
                    w = loadedSurface->w;
                    h = loadedSurface->h;
                }
                SDL_FreeSurface( loadedSurface );   //Get rid of old loaded surface
            }
            tex = newTexture;
            return tex != NULL; //Return success
        }

        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor){
            free();     //Get rid of preexisting texture
            SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str()
                , textColor );      //Render text surface
            if( textSurface == NULL ) {
                printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
            } else {
                tex = SDL_CreateTextureFromSurface( gRenderer, textSurface );    //Create texture from surface pixels
                if( tex == NULL ) {
                    printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                } else {
                    w = textSurface->w;
                    h = textSurface->h; //Get image dimensions
                }
                SDL_FreeSurface( textSurface ); //Get rid of old surface
            }
            return tex != NULL; //Return success
        }

        bool hasTexture(){
            return tex != NULL;
        }

		//Deallocates texture
		void free(){
            if( tex != NULL ) {
                SDL_DestroyTexture( tex );
                tex = NULL;
                w = 0;
                h = 0;
            }
		}

        void setColor( Uint8 red, Uint8 green, Uint8 blue ) {
            SDL_SetTextureColorMod( tex, red, green, blue );
        }

        void setBlendMode( SDL_BlendMode blending ) {
            SDL_SetTextureBlendMode( tex, blending );
        }

        void setAlpha( Uint8 alpha ) {
            SDL_SetTextureAlphaMod( tex, alpha );
        }

        void setText(std::string text, SDL_Color textColor) {

            //Render text
            if( !loadFromRenderedText( text, textColor ) )
            {
                printf( "Unable to set text: %s!\n", text);
            }
        }

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE){
            SDL_Rect renderQuad = { x, y, w, h };   //Set rendering space and render to screen
            if( clip != NULL ) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h; //Set clip rendering dimensions
            }
            SDL_RenderCopyEx( gRenderer, tex, clip, &renderQuad, angle, center, flip ); //Render to screen
		}

		//Get image dimensions
        int getWidth() { return w; }
        int getHeight() { return h; }
};

#endif // LTEXTURE_H_INCLUDED

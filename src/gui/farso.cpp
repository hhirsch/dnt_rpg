/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "farso.h"


/************************************************************
 *                        Farso_Init                        *
 ************************************************************/
void Farso_Init(SDL_Surface **screen, string title, int width, int height,
                bool fullScreen)
{
    /* Start Openning the screen  */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
    {
        printf(gettext("Argh! Can't init SDL!\n"));
        exit(1);
    }
    atexit(SDL_Quit);

    /* Define the resolution */
    Farso_DefineResolution(screen, title, width, height, fullScreen);
    
    /* Define ignored events */
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
    SDL_EventState(SDL_QUIT, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    dntFont fnt;
    fnt.init();
}

/************************************************************
 *                  Farso_DefineResolution                  *
 ************************************************************/
void Farso_DefineResolution(SDL_Surface **screen, string title, 
                            int width, int height,
                            bool fullScreen)
{
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
   
    /*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );*/

    int flags = SDL_DOUBLEBUF | SDL_OPENGL;

    if(fullScreen)
    {
      flags |= SDL_FULLSCREEN;
    }
    
    SCREEN_X = width;
    SCREEN_Y = height;

    *screen = SDL_SetVideoMode(SCREEN_X, SCREEN_Y, 32, flags);
    if ( *screen == NULL ) 
    {
       printf(gettext("Oxi! Can't ajust video mode: %s!\n"),SDL_GetError());
       exit(2);
    }

    SDL_WM_SetCaption(title.c_str(),"");
}

/************************************************************
 *                         Farso_End                        *
 ************************************************************/
void Farso_End(SDL_Surface *screen)
{
   dntFont fnt;
   fnt.end();
}



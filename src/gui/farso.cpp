/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "farso.h"


/************************************************************
 *                        Farso_Init                        *
 ************************************************************/
void Farso_Init(SDL_Surface **screen, string title)
{
    /* Start Openning the screen  */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
    {
        printf(gettext("Argh! Can't init SDL!\n"));
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
   
    /*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );*/

    *screen = SDL_SetVideoMode(SCREEN_X, SCREEN_Y, 32, /*SDL_FULLSCREEN |*/
                               /*SDL_SWSURFACE |*/ SDL_DOUBLEBUF | SDL_OPENGL);
    if ( *screen == NULL ) 
    {
       printf(gettext("Oxi! Can't ajust video mode: %s!\n"),SDL_GetError());
       exit(2);
    }

    SDL_WM_SetCaption(title.c_str(),"");

    /* Define ignored events */
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
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
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
    
}

/************************************************************
 *                         Farso_End                        *
 ************************************************************/
void Farso_End(SDL_Surface *screen)
{
}



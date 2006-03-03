/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "farso.h"

void Farso_Iniciar(SDL_Surface **screen, char* titulo)
{

    
    //Comeca a abrir a tela
    if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0 ) 
    {
        erro_Mensagem("Argh! Can't init SDL!\n",1);
    }
    atexit(SDL_Quit);
   
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );

    *screen = SDL_SetVideoMode(800, 600, 16, /*SDL_FULLSCREEN | SDL_HWSURFACE |*/ SDL_DOUBLEBUF | SDL_OPENGL);
    if ( screen == NULL ) 
    {
       erro_Mensagem("Oxi! Can't ajust video mode!\n",2);
    }

    SDL_WM_SetCaption(titulo,"");
    //Terminou de abrir a janela


    //Define quais eventos irao ser ignorados
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
    //SDL_EventState(SDL_QUIT, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
    
}

void Farso_Terminar(SDL_Surface *screen)
{
}



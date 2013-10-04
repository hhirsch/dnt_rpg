/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "farso.h"
#include "mouse.h"

#include <iostream>
using namespace std;

namespace Farso
{

/************************************************************
 *                        Farso_Init                        *
 ************************************************************/
void init(SDL_Surface **screen, string title, int width, int height,
          bool fullScreen, int antiAliasingSamples, int stencilBufferSize)
{
    /* Start Openning the screen  */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) 
    {
        cerr << "Argh! Can't init SDL!" << endl;
        exit(1);
    }
    atexit(SDL_Quit);

    /* Define the resolution */
    defineResolution(screen, title, width, height, fullScreen,
                     antiAliasingSamples, stencilBufferSize);
    
    /* Define ignored events */
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

    Font fnt;
    fnt.init();

    MouseCursor cursors;
    cursors.init();

    Colors cor;
    cor.init();
}

/************************************************************
 *                  Farso_DefineResolution                  *
 ************************************************************/
void defineResolution(SDL_Surface **screen, string title, 
                      int width, int height,
                      bool fullScreen, int antiAliasingSamples,
                      int stencilBufferSize)
{
   Options opts;
   SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
   SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );

   if(stencilBufferSize > 0)
   {
      SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, stencilBufferSize);
   }

   if(antiAliasingSamples > 0)
   {
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antiAliasingSamples);
   }

   int flags = SDL_DOUBLEBUF | SDL_OPENGL;

   if(fullScreen)
   {
      flags |= SDL_FULLSCREEN;
   }

   opts.setCurrentScreen(width, height);
   SCREEN_X = width;
   SCREEN_Y = height;

   *screen = SDL_SetVideoMode(SCREEN_X, SCREEN_Y, 32, flags);
   if ( *screen == NULL ) 
   {
      cerr << "Oxi! Can't ajust video mode: " << SDL_GetError() << endl;

      if(stencilBufferSize > 8)
      {
         cerr << "Trying again with lesser stencil." << endl;
         /* Quit the SDL  */
         SDL_Quit();
         /* Restart it  */
         init(screen, title, width, height, fullScreen, antiAliasingSamples, 8);
      }
      else if(antiAliasingSamples > 0)
      {
         cerr << "Trying again without AntiAliasing." << endl;
         /* Quit the SDL  */
         SDL_Quit();
         /* Restart it  */
         init(screen, title, width, height, fullScreen, 0, stencilBufferSize);
      }
      else
      {
         cerr << "Something must be wrong! " << endl
            << "Try editing ~/.dccnitghtmare/options.cfg" << endl;
         exit(2);
      }
   }

   SDL_WM_SetCaption(title.c_str(),"");
}

/************************************************************
 *                         Farso_End                        *
 ************************************************************/
void end(SDL_Surface *screen)
{
   Font fnt;
   fnt.end();

   MouseCursor cursors;
   cursors.finish();
}

}


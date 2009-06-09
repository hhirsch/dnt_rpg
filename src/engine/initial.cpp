/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libintl.h>
#include "initial.h"
#include "util.h"
#include <SDL/SDL_image.h>

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

/***************************************************************
 *                     Constructor                             *
 ***************************************************************/
initialScreen::initialScreen()
{
   gui  = NULL;
   buttonNew = NULL;
   buttonContinue = NULL;
   buttonSave = NULL;
   buttonLoad = NULL;
   buttonOptions = NULL;
   buttonExit = NULL;
}

/***************************************************************
 *                      Destructor                             *
 ***************************************************************/
initialScreen::~initialScreen()
{
}

/***************************************************************
 *                          run                                *
 ***************************************************************/
int initialScreen::run(int Status,GLdouble proj[16],
                       GLdouble modl[16], GLint viewPort[4],
                       GLuint tituloID, sound* snd, guiInterface* interf)
{
   Uint32 tempo;
   Uint32 tempoAnterior = 0;
   done = false;
   guiObject* object = NULL;
   int eventInfo = FARSO_EVENT_NONE;
   cursor cursors;
   gui = interf;

   int xPos = (int)(SCREEN_X / 2.0);
   int yPos = (int)(SCREEN_Y / 2.0);

   string dnt = "DNT - ";
   dnt += VERSION;

   window* jan = gui->insertWindow(xPos-64,yPos-64,xPos+64,yPos+64, dnt);  
   jan->setAttributes(false,true,false,false);
   if(Status == ON_INIT)
   {
      buttonNew = jan->getObjectsList()->insertButton(30,20,98,38, 
                                                      gettext("New"),1);
   }
   else
   {
      buttonContinue = jan->getObjectsList()->insertButton(30,20,98,38,
                                           gettext("Continue"),1);
   }
   buttonSave = jan->getObjectsList()->insertButton(30,40,98,58,
                                           gettext("Save"),1);
   buttonSave->setAvailable(Status != ON_INIT);
   buttonLoad = jan->getObjectsList()->insertButton(30,60,98,78,
                                               gettext("Load"),1);
   buttonOptions = jan->getObjectsList()->insertButton(30,80,98,98,
                                            gettext("Options"),1);
   buttonExit = jan->getObjectsList()->insertButton(30,100,98,118,
                                           gettext("Exit"),1);
   gui->openWindow(jan);

   glDisable(GL_LIGHTING);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   Uint8 *keys;
   int x,y;

   while (!done)
   {
      tempo = SDL_GetTicks();
      if((tempo - tempoAnterior) >= 20) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         glClearColor(0,0,0,1);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         
         object = gui->manipulateEvents(x,y,Mbotao,keys, eventInfo);
         if(eventInfo != FARSO_EVENT_NONE)
         {
             if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
             {
                 if(object == (guiObject*) buttonNew)
                 {
                    result = NEW_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonContinue)
                 {
                    result = CONTINUE_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonLoad)
                 {
                    result = LOAD_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonSave)
                 {
                    result = SAVE_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonExit)
                 {
                    result = EXIT_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonOptions)
                 {
                    result = OPTIONS;
                    done = true;
                 }
             }
         }
         glPushMatrix();
            draw2DMode();
            
            textureToScreen(tituloID, 0,0, SCREEN_X-1,SCREEN_Y-1, 800,600);
            gui->draw(proj,modl,viewPort);

            glPushMatrix();
               cursors.draw(x, y);
            glPopMatrix();

            draw3DMode(OUTDOOR_FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();
      }
      else if((19 - (int)(tempo - tempoAnterior)) > 0 ) 
      {
         SDL_Delay((19 - (tempo - tempoAnterior)) );
      }
   }

   gui->closeWindow(jan);

   glEnable(GL_LIGHTING);
   return(result);
}


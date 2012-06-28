/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "initial.h"
#include "util.h"
#include "../lang/translate.h"
#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif
using namespace std;

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
   menuWindow = NULL;
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
                       GLuint tituloID, sound* snd, 
                       Farso::GuiInterface* interf)
{
   Uint32 tempo;
   Uint32 tempoAnterior = 0;
   done = false;
   Farso::GuiObject* object = NULL;
   int eventInfo = Farso::EVENT_NONE;
   cursor cursors;
   gui = interf;

   int xPos = (int)(Farso::SCREEN_X / 2.0);
   int yPos = (int)(Farso::SCREEN_Y / 2.0);

   string dnt = "DNT - ";
   dnt += VERSION;

   menuWindow = gui->insertWindow(xPos-64,yPos-64,xPos+64,yPos+64, dnt);  
   menuWindow->setAttributes(false,true,false,false);
   
   if(Status == ON_INIT)
   {
      buttonNew = menuWindow->getObjectsList()->insertButton(30,20,98,38, 
                                                      gettext("New"),1);
   }
   else
   {
      buttonContinue = menuWindow->getObjectsList()->insertButton(30,20,98,38,
                                           gettext("Continue"),1);
   }
   buttonSave = menuWindow->getObjectsList()->insertButton(30,40,98,58,
                                           gettext("Save"),1);
   buttonSave->setAvailable(Status != ON_INIT);
   buttonLoad = menuWindow->getObjectsList()->insertButton(30,60,98,78,
                                               gettext("Load"),1);
   buttonOptions = menuWindow->getObjectsList()->insertButton(30,80,98,98,
                                            gettext("Options"),1);
   buttonExit = menuWindow->getObjectsList()->insertButton(30,100,98,118,
                                           gettext("Exit"),1);
   menuWindow->setExternPointer(&menuWindow);
   gui->openWindow(menuWindow);

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   Uint8 *keys;
   int x,y;

   while (!done)
   {
      /* Verify active window (force it to be the menu one, if not modal) */
      if( (gui->getActiveWindow() != menuWindow) && 
          (!gui->getActiveWindow()->isModal()) )
      {
         menuWindow->activate();
      }

      tempo = SDL_GetTicks();
      if((tempo - tempoAnterior) >= 20) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         /* Clear events */
         SDL_Event event;
         while(SDL_PollEvent(&event))
         {
         }
         keys = SDL_GetKeyState(NULL);
         glClearColor(0,0,0,1);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         
         object = gui->manipulateEvents(x,y,Mbotao,keys, eventInfo);
         if(eventInfo != Farso::EVENT_NONE)
         {
             if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
             {
                 if(object == (Farso::GuiObject*) buttonNew)
                 {
                    result = NEW_GAME;
                    done = true;
                 }
                 else if(object == (Farso::GuiObject*) buttonContinue)
                 {
                    result = CONTINUE_GAME;
                    done = true;
                 }
                 else if(object == (Farso::GuiObject*) buttonLoad)
                 {
                    result = LOAD_GAME;
                    done = true;
                 }
                 else if(object == (Farso::GuiObject*) buttonSave)
                 {
                    result = SAVE_GAME;
                    done = true;
                 }
                 else if(object == (Farso::GuiObject*) buttonExit)
                 {
                    result = EXIT_GAME;
                    done = true;
                 }
                 else if(object == (Farso::GuiObject*) buttonOptions)
                 {
                    result = OPTIONS;
                    done = true;
                 }
             }
         }
         glPushMatrix();
            draw2DMode();
            
            Farso::textureToScreen(tituloID, 0,0, 
                  Farso::SCREEN_X-1,Farso::SCREEN_Y-1, 800,600);
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

   gui->closeWindow(menuWindow);

   glEnable(GL_LIGHTING);
   return(result);
}


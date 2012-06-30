/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "editor.h"
#include "../../engine/util.h"
#include "../../gui/dntfont.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               editor                                   //
//                                                                        //
////////////////////////////////////////////////////////////////////////////


/************************************************************************
 *                             Constructor                              *
 ************************************************************************/
editor::editor()
{
   int i;
   /* Alloc the visible Matrix */
   viewMatrix = new GLfloat*[6];
   for(i = 0; i < 6; i++)
   {
      viewMatrix[i] = new GLfloat[4];
   }

   /* Get current data directories */
   dir.findDataDirectories();


   Farso::Options farOpt;
   farOpt.setNeededDefaults(dir.getRealFile(DNT_FONT_ARIAL),
         dir.getRealFile("cursors/Walk.png"),
         dir.getRealFile("icons/maximize.png"),
         dir.getRealFile("icons/close.png"),
         "Maximize", "Close", "Confirm", "Cancel");

   /* Initialize SDL/Farso/etc */
   Farso::init(&screen,"DNT's Particle Editor", 800, 600,false, 0, 8);

   /* Get OpenGL Extensions */
   ext.defineAllExtensions();

   /* Create the view */
   createView(screen);
   
   /* Clear */
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glClearDepth(1.0);

   /* Some details  */ 
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   /* Some light */
   GLfloat light_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
   glDisable(GL_LIGHTING); 

   /* Create the camera */
   gameCamera = new camera();

   /* Create the GUI */
   gui = new Farso::GuiInterface("");

   /* Nullify elements */
   p = NULL;
   fileWindow = NULL;
   particleWindow = NULL;
   actWindow = NULL;
   curFileName = "";
}

/************************************************************************
 *                              Destructor                              *
 ************************************************************************/
editor::~editor()
{
   int i;

   /* Free Current Particle */
   if(p)
   {
      deleteParticle();
   }
   if(particleWindow)
   {
      delete(particleWindow);
   }
   /* Free GUI */
   delete(gui);
   /* And free the camera */
   delete(gameCamera);
   /* Clear the visibleMatrix */
   for(i = 0; i < 6; i++)
   {
      delete[] viewMatrix[i];
   }
   delete[] viewMatrix;
   Farso::end(screen);
}

/************************************************************************
 *                           createView                                 *
 ************************************************************************/
void editor::createView(SDL_Surface *screen)
{
   glViewport (0, 0, (GLsizei) screen->w, (GLsizei) screen->h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLsizei) screen->w / (GLsizei) screen->h, 1.0, 650.0);
   glGetIntegerv(GL_VIEWPORT, viewPort);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/************************************************************************
 *                          deleteParticle                              *
 ************************************************************************/
void editor::deleteParticle()
{
   if(p)
   {
      delete(p);
   }
   p = NULL;
}

/************************************************************************
 *                          createParticle                              *
 ************************************************************************/
bool editor::createParticle()
{
   if(p)
   {
      /* Must delete the existing one! */
      deleteParticle();
   }

   p = new partAux();

   return(true);
}

/************************************************************************
 *                          updateParticle                              *
 ************************************************************************/
void editor::updateParticle()
{
   /* Must verify if has some particle opened */
   if(!p)
   {
      return;
   }

   p->doStep(NULL);
}

/****************************************************************
 *                      Open File  Window                       *
 ****************************************************************/
void editor::openFileWindow(bool load)
{
   if(fileWindow)
   {
       /* Close the current opened */
       gui->closeWindow(fileWindow);
   }

   fileLoading = load;
   fileWindow = gui->insertWindow(200,100,460,285,"File");
   fileSelector = fileWindow->getObjectsList()->insertFileSel(5,18,load,
                                                          "../data/particles/");
   fileSelector->setFilter(".par");
   if( (!fileLoading) && (!curFileName.empty()) )
   {
      fileSelector->setFileName(curFileName);
   }
   fileWindow->setAttributes(false,true,false,false);
   fileWindow->setExternPointer(&fileWindow);
   gui->openWindow(fileWindow);

}

/************************************************************************
 *                           createWindows                              *
 ************************************************************************/
void editor::createWindows()
{
   /* Actions Window */
   actWindow = gui->insertWindow(0,0,127,63,"Actions");
   actWindow->setExternPointer(&actWindow);
   buttonSave = actWindow->getObjectsList()->insertButton(5,27,43,45,"Save",0);
   buttonLoad = actWindow->getObjectsList()->insertButton(44,27,82,45,"Load",0);
   buttonExit = actWindow->getObjectsList()->insertButton(83,27,122,45,
                                                          "Exit",0);
   gui->openWindow(actWindow);

   particleWindow = new partWindow(gui);
}

/************************************************************************
 *                           treatGuiEvents                             *
 ************************************************************************/
void editor::treatGuiEvents()
{
   int eventInfo = Farso::EVENT_NONE;

   done = false;

   /* Get events */
   Farso::GuiObject* obj = gui->manipulateEvents(mouseX, mouseY, mButton, 
                                          keys, eventInfo);

   /* Write current number of particles */
   particleWindow->writeCurParticles();

   if(eventInfo == Farso::EVENT_NONE)
   {
      /* No Event, so must treat Camera Input */
      gameCamera->doIO(keys, mButton, mouseX, mouseY, mouseWheel, DELTA_CAMERA);
   }
   else
   {
      if(particleWindow->treat(obj, eventInfo))
      {
      }
      else if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
      {
         /* Exit was pressed! */
         if(obj == buttonExit)
         {
            done = true;
         }
         else if(obj == buttonSave)
         {
            openFileWindow(false);
         }
         else if(obj == buttonLoad)
         {
            openFileWindow(true);
         }
      }
      /* File Selectors Things */
      else if(eventInfo == Farso::EVENT_FILE_SEL_ACCEPT)
      {
         if(fileWindow)
         {
            if(obj == (Farso::GuiObject*)fileSelector) 
            {
               curFileName = fileSelector->getFileName();
               gui->closeWindow(fileWindow);
               if(fileLoading)
               {
                  createParticle();
                  p->load(curFileName);
                  particleWindow->set(p);
               }
               else
               {
                  /* Save the Particle with desired fileName */
                  if(p)
                  {
                     Farso::Warning warn;
                     if(p->save(curFileName))
                     {
                        warn.show("Message", "File was saved!", gui);
                     }
                     else
                     {
                        warn.show("Error", "Can't save file!", gui);
                     }
                  }
               }
            }
         }
      }
      else if(eventInfo == Farso::EVENT_FILE_SEL_CANCEL)
      {
         if(fileWindow)
         {
            /* Just close the window */
            if(obj == (Farso::GuiObject*)fileSelector) 
            {
               gui->closeWindow(fileWindow);
            }
         }
      }
   }
}

/************************************************************************
 *                                render                                *
 ************************************************************************/
void editor::render()
{
   glClearColor(0,0,0,0);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   glLoadIdentity();
   
   /* Redefine Camera Position */
   gameCamera->lookAt(NULL);
   updateFrustum(viewMatrix,proj,modl);

   /* Part Draw */
   glPushMatrix();
      updateParticle();
   glPopMatrix();

   glColor3f(1.0,1.0,1.0);

   glDisable(GL_BLEND);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);

   draw2DMode(); 
      gui->draw(proj,modl,viewPort);
      
      /* Cursor */
      glPushMatrix();
         curs.draw(mouseX, mouseY, 0);
      glPopMatrix();


   draw3DMode(INDOOR_FARVIEW);

   glFlush();
   SDL_GL_SwapBuffers();
}

/************************************************************************
 *                                 run                                  *
 ************************************************************************/
void editor::run()
{
   createWindows();

   /* Flux Control Variables */
   Uint32 lastUpdate = 0;
   Uint32 varTime = 0;
   Uint32 time = 0;
   done = false;

   /* Set the initial camera position for partEditor */
   gameCamera->setTheta(0);
   gameCamera->setD(80);

   /* Main loop */
   while(!done)
   {
      if(actWindow == NULL)
      {
         /* exit when actWindow is closed */
         done = true;
      }

      time = SDL_GetTicks();
      varTime = (time - lastUpdate);
      if(varTime >= UPDATE_RATE)
      {
         lastUpdate = SDL_GetTicks();

         /* Get Keyboard and Mouse State */
         SDL_PumpEvents();
         mButton = SDL_GetMouseState(&mouseX,&mouseY);
         keys = SDL_GetKeyState(NULL);

         /* Let's check some events */
         SDL_Event event;
         mouseWheel = 0;
         while(SDL_PollEvent(&event))
         {
            if(event.type == SDL_QUIT)
            {
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
               if(event.button.button == SDL_BUTTON_WHEELUP)
               {
                  mouseWheel++;
               }
               else if(event.button.button == SDL_BUTTON_WHEELDOWN)
               {
                  mouseWheel--;
               }
            }
         }



         /* Treat GUI Events */
         treatGuiEvents();

         /* Render Things */
         render();
      }
      else
      {
         int t = SDL_GetTicks();
         if(UPDATE_RATE - (t - time) > 5)
            SDL_Delay(UPDATE_RATE - (t - time));
      }
   }
}


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


#include "levelup.h"
#include "classwindow.h"
#include "skillwindow.h"
#include "featsel.h"
#include "culling.h"
#include "cursor.h"
#include "options.h"

/* Some State constants */
#define LEVEL_UP_CANCEL         0
#define LEVEL_UP_CLASS_WINDOW   1
#define LEVEL_UP_SKILL_WINDOW   2
#define LEVEL_UP_TALENT_WINDOW  3
#define LEVEL_UP_FINISHED       4


/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
levelUp::levelUp(character* c, featsList* ft)
{
   current = c;
   features = ft;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
levelUp::~levelUp()
{
   current = NULL;
}

/***********************************************************************
 *                            doLevelUp                                *
 ***********************************************************************/
void levelUp::doLevelUp(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   Farso::GuiObject* obj;
   Uint8 mButton;
   Uint8* keys;
   options option;
   dirs dir;
   int x,y;
   int time=0, lastTime=0;
   int eventInfo, res;
   cursor cursors;

   featSelWindow* talentWindow = NULL;
   classWindow *clWindow = NULL;
   skillWindow *skWindow = NULL;
   classe* selClass = current->actualClass[0];

   /* Unapply effects */
   current->getEffects()->unApply();

   /* Create the new GUI */
   Farso::GuiInterface* gui = new Farso::GuiInterface("");

   /* Create the background */
   GLuint tituloId;
   SDL_Surface* img = IMG_Load(
         dir.getRealFile("texturas/general/inicio.png").c_str());
   glGenTextures(1,&tituloId);
   Farso::setTexture(img,tituloId);
   SDL_FreeSurface(img);

   /* Open the class window */
   clWindow = new classWindow(gui, &selClass);
   state = LEVEL_UP_CLASS_WINDOW;

   /* Run, until canceled or finished */
   while((state != LEVEL_UP_CANCEL) && (state != LEVEL_UP_FINISHED))
   {
      time = SDL_GetTicks();
      if(time - lastTime >= UPDATE_RATE)
      {
         lastTime = time;
         /* Clear things */
         glClearColor(0,0,0,1);
         glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

         /* Get Events */
         SDL_PumpEvents();
         /* Clear events */
         SDL_Event event;
         while(SDL_PollEvent(&event))
         {
         }
         keys = SDL_GetKeyState(NULL);
         mButton = SDL_GetMouseState(&x,&y);
         obj = gui->manipulateEvents(x,y,mButton,keys, eventInfo);

         glPushAttrib(GL_ENABLE_BIT);

         glDisable(GL_FOG);
         glDisable(GL_LIGHTING);

         /* Render Things */
         glPushMatrix();
            draw2DMode();
            Farso::textureToScreen(tituloId,0,0,
                  Farso::SCREEN_X-1,Farso::SCREEN_Y-1,800,600);
            gui->draw(proj,modl,viewPort);
            glPushMatrix();
               cursors.draw(x, y);
            glPopMatrix();
            draw3DMode(option.getFarViewFactor()*OUTDOOR_FARVIEW);
         glPopMatrix();
         
         glPopAttrib();
         
         glFlush();
         SDL_GL_SwapBuffers();

         /* Treat current opened window events */
         switch(state)
         {
            /* Treat CLass Windows Events */
            case LEVEL_UP_CLASS_WINDOW:
            {
               res = clWindow->treat(obj, eventInfo, gui);
               if(res == CLASSW_CONFIRM)
               {
                  /* Delete the current class window */
                  delete(clWindow);

                  /* Verify if can take a level at the class */
                  if(current->canClass(selClass))
                  {
                     /* Can, so, we must go to the skill window */
                     int points = current->getOtherLevelSkillPoints(selClass);
                     current->sk.setAvaiblePoints(points + 
                                                current->sk.getAvaiblePoints());
                     skWindow = new skillWindow(&current->sk, gui, 
                                                current->getLevel(selClass)+1);
                     state = LEVEL_UP_SKILL_WINDOW;
                  }
                  else
                  {
                     /* Can't, so must alert user and keep at the 
                      * class window */                     
                     Farso::Warning warn;
                     clWindow = new classWindow(gui, &selClass);
                     state = LEVEL_UP_CLASS_WINDOW;
                     warn.show(gettext("Warning"), 
                               gettext("You can't multiclass more!"), gui);

                  }
               }
               else if(res == CLASSW_CANCEL)
               {
                  /* Must cancel the level up */
                  state = LEVEL_UP_CANCEL;
                  delete(clWindow);
               }

            }
            break;

            /* Treat skill window */
            case LEVEL_UP_SKILL_WINDOW:
            {
               res = skWindow->treat(obj, eventInfo, gui);
               if(res == SKILLW_CONFIRM)
               {
                  int totalNewTalents = 0;
                  int totalLevel = current->getLevel()+1;
                  if( ((totalLevel % 5) == 0) ||
                      ((totalLevel % 5) == 3) )
                  {
                     totalNewTalents = 1;
                  }
                  delete(skWindow);
                  state = LEVEL_UP_TALENT_WINDOW;
                  talentWindow = new featSelWindow(gui);

                  talentWindow->open(current, features, totalNewTalents); 
               }
               else if(res == SKILLW_CANCEL)
               {
                  /* Must undo the previous class free skills points */
                  int points = current->getOtherLevelSkillPoints(selClass);
                  current->sk.setAvaiblePoints(current->sk.getAvaiblePoints() -
                                               points);
                  /* Must go back to the class window */
                  delete(skWindow);
                  state = LEVEL_UP_CLASS_WINDOW;
                  clWindow = new classWindow(gui, &selClass);
               }
            }
            break;
            /* Treat featSel Window */
            case LEVEL_UP_TALENT_WINDOW:
            {
               res = talentWindow->treat(obj, eventInfo);
               if(res == TALENT_WINDOW_CONFIRM)
               {
                  /* Apply all new permanent feats */
                  talentWindow->applyAllNewPermanentFeats();
                  /* Done with level up */
                  delete(talentWindow);
                  state = LEVEL_UP_FINISHED;
               }
               else if(res == TALENT_WINDOW_CANCEL)
               {
                  /* Go back to the skills window */
                  skWindow = new skillWindow(&current->sk, gui, 
                                             current->getLevel(selClass)+1);
                  state = LEVEL_UP_SKILL_WINDOW;
               }
            }
            break;
         }
      }
      else
      {
         SDL_Delay(UPDATE_RATE-2);
      }
   }

   if(state == LEVEL_UP_FINISHED)
   {
      /* Must apply the class choosed */
      current->getNewClassLevel(selClass);
   }

   /* Reapply effects */
   current->getEffects()->unApply();

   /* Clear the temporary gui, and done! */
   delete(gui);
   glDeleteTextures(1,&tituloId);
}


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

#include "particles.h"
#include "../../gui/messages.h"

#include <iostream>
using namespace std;
using namespace dntMapEditor;

#define STATE_PLANES     800
#define STATE_GRASS_INIT 801
#define STATE_LAKE_INIT  802

/*****************************************************************
 *                          Constructor                          *
 *****************************************************************/
Particles::Particles(Map* acMap)
{
   actualMap = acMap;
   actualParticle = NULL;
   state = -1;
   particleType = -1;
   previousText = "";
}

/*****************************************************************
 *                           Destructor                          *
 *****************************************************************/
Particles::~Particles()
{
   state = -1;
   actualMap = NULL;
   particleType = -1;
   string particleFileName = "";
   deleteParticle();
}

/*****************************************************************
 *                          deleteParticle                       *
 *****************************************************************/
void Particles::deleteParticle()
{
   if(actualParticle)
   {
      delete(actualParticle);
      actualParticle = NULL;
   }
}

/*****************************************************************
 *                          verifyAction                         *
 *****************************************************************/
void Particles::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                             Uint8 mButton, Uint8* keys, GuiIO* gui, 
                             partController* pS, GLdouble proj[16],
                             GLdouble modl[16], GLint viewPort[4],
                             string selectedText,
                             GrassWindow* grWindow, 
                             WaterWindow* wtWindow,
                             Map* actualMap)
{
   int tool = gui->getTool();

   /* Set the system controller to the windows */
   if(grWindow)
   {
      //grWindow->setPartSystem(pS);
   }
   if(wtWindow)
   {
      wtWindow->setPartSystem(pS);
   }

   if( ( (tool != state) || (selectedText != previousText) ) && 
       (state != STATE_PLANES) && (state != TOOL_PARTICLE_GRASS))
   {
      deleteParticle();
   }

   previousText = selectedText;

   /* Grass TOOL */
   if( tool == TOOL_PARTICLE_GRASS )
   {
      if(state == TOOL_PARTICLE_GRASS)
      {
         if(mButton & SDL_BUTTON(1))
         {
            x1 = mouseX;
            z1 = mouseZ;
            state = STATE_GRASS_INIT;
         }
      }
      else if(state == STATE_GRASS_INIT)
      {
         if(!(mButton & SDL_BUTTON(1)))
         {
            GLfloat tmp;
            if(x2 < x1)
            {
               tmp = x2;
               x2 = x1;
               x1 = tmp;
            }
            if(z2 < z1)
            {
               tmp = z2;
               z2 = z1;
               z1 = tmp;
            }

#if 0            
            GLfloat povValue = 700.0;
            int total = (int)floor((z2-z1)*(x2-x1) / povValue);
            grass* gr = (grass*) pS->addParticle(DNT_PARTICLE_TYPE_GRASS, 
                                                 x1, z1, x2, z2, 
                                                 total, 3.0, selectedText); 
            grWindow->setGrass(gr);
            state = TOOL_PARTICLE_GRASS;
#endif
         }
         x2 = mouseX;
         z2 = mouseZ;
      }
      else
      {
         state = TOOL_PARTICLE_GRASS;
         //particleType = DNT_PARTICLE_TYPE_GRASS;
      }
      
   }

   /* Lake TOOL */
   else if(tool == TOOL_PARTICLE_LAKE)
   {
      if(state == TOOL_PARTICLE_LAKE)
      {
         if(mButton & SDL_BUTTON(1))
         {
            x1 = mouseX;
            z1 = mouseZ;
            state = STATE_LAKE_INIT;
         }
      }
      else if(state == STATE_LAKE_INIT)
      {
         if(!(mButton & SDL_BUTTON(1)))
         {
            GLfloat tmp;
            if(x2 < x1)
            {
               tmp = x2;
               x2 = x1;
               x1 = tmp;
            }
            if(z2 < z1)
            {
               tmp = z2;
               z2 = z1;
               z1 = tmp;
            }

            /* Add the lake */
            actualMap->addLake(x1,z1,x2,z2);

            state = TOOL_PARTICLE_LAKE;
         }
         x2 = mouseX;
         z2 = mouseZ;
      }
      else
      {
         state = TOOL_PARTICLE_LAKE;
         particleType = -1;
      }
   }

   /* default particle TOOL */
   else if( (tool == TOOL_PARTICLE_DEFAULT) && (!actualParticle) )
   {
      state = TOOL_PARTICLE_DEFAULT; 
      particleType = DNT_PARTICLE_SYSTEM_TYPE_DEFAULT;
      string fileToOpen = selectedText;
      if(fileToOpen != "../data/Particles/")
      {
         height = 0;
         actualParticle = new particleSystem();
         if(!actualParticle->load(fileToOpen))
         {
            cerr << "Error opening: " << fileToOpen << endl;
            delete(actualParticle);
            actualParticle = NULL;
            return;
         }
         actualParticle->definePosition(mouseX, height, mouseZ);
      }
      else
      {
         actualParticle = NULL;
      }
   }

   /* Default particle Actions (Up, down, etc) */
   if( (actualParticle) )
   {
      if(keys[SDLK_EQUALS])
      {
         height += 1;
      }
      if(keys[SDLK_MINUS])
      {
         height -= 1;
      }

      actualParticle->definePosition(mouseX, height, mouseZ);

      if(mButton & SDL_BUTTON(1))
      {
         if( (tool != TOOL_PARTICLE_GRASS) && (tool != TOOL_PARTICLE_LAKE))
         {

            particleSystem* p = pS->addParticle(mouseX, height, mouseZ,
                  actualParticle->getFileName());
            /* Set the water window (in reality now works for any kind
             * of particle systems) */
            wtWindow->setWater(p);
            deleteParticle();
            gui->setTool(TOOL_NONE);
         }
         while(mButton & SDL_BUTTON(1))
         {
            /* Wait for Mouse Button Release */
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
}

/*****************************************************************
 *                          drawTemporary                        *
 *****************************************************************/
void Particles::drawTemporary(GLfloat** matriz)
{
   if(state == STATE_GRASS_INIT)
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.01,0.1,0.8);
      glBegin(GL_QUADS);
         glVertex3f(x1, 1, z1);
         glVertex3f(x2, 1, z1);
         glVertex3f(x2, 1, z2);
         glVertex3f(x1, 1, z2);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   else if(state == STATE_LAKE_INIT)
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.5,0.4,0.9);
      glBegin(GL_QUADS);
         glVertex3f(x1, -4, z1);
         glVertex3f(x2, -4, z1);
         glVertex3f(x2, -4, z2);
         glVertex3f(x1, -4, z2);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   else if(actualParticle != NULL)
   {
      actualParticle->doStep(NULL);
   }
}


/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "objects.h"

#define OBJECTS_STATE_NONE    0
#define OBJECTS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
objects::objects(Map* acMap)
{
   actualTool = 0;
   actualMap = acMap;
   state = OBJECTS_STATE_NONE;
   actualObstacle = NULL;
   objectFile = "";
   obstacleX = 0;
   obstacleY = 0;
   obstacleZ = 0;
   obstacleOrientation = 0;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
objects::~objects()
{
   actualMap = NULL;
   actualObstacle = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void objects::verifyAction(Uint8* keys, 
                           GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int mouseXw, int mouseYw,
                           int tool, GLdouble proj[16], 
                           GLdouble modl[16], GLint viewPort[4])
{
   //FIXME -> remove the object when not insert!
   if( (tool == TOOL_OBSTACLE_ADD) && (actualObstacle != NULL))
   {
      state = OBJECTS_STATE_ADD;

      if( (mX != mouseXw) || (mY != mouseYw))
      {
         mX = mouseXw;
         mY = mouseYw;
         obstacleX = mouseX;
         obstacleZ = mouseZ;
      }
      
      /* Insert it on map */
      if(mButton & SDL_BUTTON(1))
      {
         insertObject(mouseX, mouseZ, obstacleOrientation, actualMap, 
                      actualObstacle, 
                      (int)(mouseX / actualMap->squareSize()), 
                      (int)(mouseZ / actualMap->squareSize()));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }

         /* No more with the actual */
         actualObstacle = NULL;
      }

      /* Rotate Left/Right object */
      else if(mButton & SDL_BUTTON(2))
      {
         obstacleOrientation += 1;
      }
      else if(mButton & SDL_BUTTON(3))
      {
         obstacleOrientation -= 1;
      }

      /* Up/Down Object */
      if(keys[SDLK_w])
      {
         obstacleY += 0.1;
      }
      if(keys[SDLK_s])
      {
         obstacleY -= 0.1;
      }
      if(keys[SDLK_t])
      {
         obstacleY += 1.0;
      }
      if(keys[SDLK_g])
      {
         obstacleY -= 1.0;
      }
      if(keys[SDLK_0])
      {
         obstacleY = 0;
      }
   }
   else
   {
      state = OBJECTS_STATE_NONE;
      actualObstacle = NULL;
      objectFile = "";
   }
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void objects::drawTemporary()
{
   if( (state == OBJECTS_STATE_ADD) && (actualObstacle != NULL))
   {
      actualObstacle->xPosition = obstacleX;
      actualObstacle->yPosition = obstacleY + 
                                 actualMap->getHeight(obstacleX, obstacleZ);
      actualObstacle->zPosition = obstacleZ;
      actualObstacle->orientationY = obstacleOrientation;
      actualObstacle->draw(false);
   }
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void objects::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                          Map* acMap, object* obj, int qx, int qz)
{
   //TODO, mark with no collision some pickable objects
   acMap->insertObject(xReal, 
                       obstacleY + actualMap->getHeight(xReal, zReal), 
                       zReal, 0.0f, orObj, 0.0f, obj, qx, qz, 1); 
}

/******************************************************************
 *                       defineActualObject                       *
 ******************************************************************/
void objects::defineActualObject(object* obj, string fileName)
{
   if(obj)
   {
      actualObstacle = obj;
      objectFile = fileName;
   }
}

/******************************************************************
 *                       getObjectFileName                        *
 ******************************************************************/
string objects::getObjectFileName()
{
   return(objectFile);
}


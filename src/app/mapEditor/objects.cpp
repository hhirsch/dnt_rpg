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

#include "objects.h"
using namespace std;
using namespace dntMapEditor;

#define OBJECTS_STATE_NONE    0
#define OBJECTS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
Objects::Objects(Map* acMap)
{
   actualTool = 0;
   actualMap = acMap;
   state = OBJECTS_STATE_NONE;
   actualObstacle = NULL;
   objectFile = "";
   obstacleX = 0;
   obstacleY = 0;
   obstacleZ = 0;
   angleX = 0;
   angleY = 0;
   angleZ = 0;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
Objects::~Objects()
{
   actualMap = NULL;
   actualObstacle = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
sceneNode* Objects::verifyAction(Uint8* keys, 
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
         insertObject(actualMap, actualObstacle, 
                      (int)(actualObstacle->scNode->getPosX() / 
                            actualMap->squareSize()), 
                      (int)(actualObstacle->scNode->getPosZ() / 
                            actualMap->squareSize()));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }

         /* No more with the actual */
         sceneNode* sc = actualObstacle->scNode;
         actualObstacle = NULL;
         return(sc);
      }

      /* Rotate Left/Right object */
      else if(mButton & SDL_BUTTON(2))
      {
         if(keys[SDLK_x])
         {
            angleX += 1;
         }
         if(keys[SDLK_z])
         {
            angleZ += 1;
         }
         if(keys[SDLK_y])
         {
            angleY += 1;
         }
      }
      else if(mButton & SDL_BUTTON(3))
      {
         if(keys[SDLK_x])
         {
            angleX -= 1;
         }
         if(keys[SDLK_z])
         {
            angleZ -= 1;
         }
         if(keys[SDLK_y])
         {
            angleY -= 1;
         }
      }

      /* Up/Down Object */
      if(keys[SDLK_r])
      {
         obstacleY += 0.1;
      }
      if(keys[SDLK_f])
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
      if(keys[SDLK_BACKSPACE])
      {
         angleX = 0;
         angleY = 0;
         angleZ = 0;
      }
      actualObstacle->scNode->set(obstacleX, obstacleY, obstacleZ,
            angleX, angleY, angleZ);
   }
   else
   {
      state = OBJECTS_STATE_NONE;
      actualObstacle = NULL;
      objectFile = "";
   }
   return(NULL);
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void Objects::drawTemporary()
{
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Objects::insertObject(Map* acMap, object* obj, int qx, int qz)
{
   //TODO, mark with no collision some pickable Objects
   acMap->insertObject(obj->scNode->getPosX(), 
                       obj->scNode->getPosY(),
                       obj->scNode->getPosZ(),
                       obj->scNode->getAngleX(), 
                       obj->scNode->getAngleY(),
                       obj->scNode->getAngleZ(), obj, qx, qz, 1); 
}

/******************************************************************
 *                       defineActualObject                       *
 ******************************************************************/
void Objects::defineActualObject(object* obj, string fileName)
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
string Objects::getObjectFileName()
{
   return(objectFile);
}

/******************************************************************
 *                          deleteObject                          *
 ******************************************************************/
void Objects::deleteObject()
{
   if(actualObstacle)
   {
      delete(actualObstacle);
      actualObstacle = NULL;
      objectFile = "";
   }
}


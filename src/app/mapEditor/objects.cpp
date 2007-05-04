#include "objects.h"

#define OBJECTS_STATE_NONE    0
#define OBJECTS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
objects::objects(Map* map, modelList* usedModels)
{
   actualMap = map;
   state = OBJECTS_STATE_NONE;
   models = usedModels;
   actualObstacle = NULL;
   objectFile = "";
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
objects::~objects()
{
   actualMap = NULL;
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void objects::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                           Uint8 mButton, int mouseXw, int mouseYw,
                           int tool, GLdouble proj[16], 
                           GLdouble modl[16], GLint viewPort[4])
{
   if(tool == TOOL_OBSTACLE_ADD)
   {
      state = OBJECTS_STATE_ADD;

      if( (mX != mouseXw) || (mY != mouseYw))
      {
         mX = mouseXw;
         mY = mouseYw;
         obstacleX = mouseX;
         obstacleZ = mouseZ;
      }
      
      if( (mButton & SDL_BUTTON(1)) && (actualObstacle != NULL) )
      {
         insertObject(mouseX, mouseZ, obstacleOrientation, actualMap, 
                      actualObstacle, 
                      (int)(mouseX / SQUARESIZE), (int)(mouseZ / SQUARESIZE));
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
      else if((mButton & SDL_BUTTON(2)) && (actualObstacle != NULL))
      {
         obstacleOrientation += 1;
      }
      else if((mButton & SDL_BUTTON(3)) && (actualObstacle != NULL))
      {
         obstacleOrientation -= 1;
      }
   }
   else
   {
      state = OBJECTS_STATE_NONE;
      actualObstacle = NULL;
   }
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void objects::drawTemporary()
{
   if( (state == OBJECTS_STATE_ADD) && (actualObstacle != NULL))
   {
      glTranslatef(0.0, actualMap->getHeight(obstacleX, obstacleZ), 0.0);
      actualObstacle->draw(obstacleX, obstacleZ, 0, obstacleOrientation);
   }
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void objects::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                          Map* map, mapObject* obj, int qx, int qz)
{
   map->insertObject(xReal, zReal, orObj, obj, qx, qz); 
}

/******************************************************************
 *                       defineActualObject                       *
 ******************************************************************/
void objects::defineActualObject(mapObject* obj, string fileName)
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


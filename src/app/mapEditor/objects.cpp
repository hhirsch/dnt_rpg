#include "objects.h"

#define OBJECTS_STATE_NONE    0
#define OBJECTS_STATE_ADD     1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
objects::objects(Map* acMap, modelList* usedModels)
{
   actualTool = 0;
   actualMap = acMap;
   state = OBJECTS_STATE_NONE;
   if(usedModels == NULL)
   {
      printf("No modelList at objects mapEditor controller!\n");
   }
   models = usedModels;
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
      actualObstacle->orientation = obstacleOrientation;
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
                       zReal, orObj, obj, qx, qz, 1); 
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


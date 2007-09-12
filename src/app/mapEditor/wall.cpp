#include "wall.h"

#define WALL_STATE_OTHER     0
#define WALL_STATE_ADD_INIT  1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
wallController::wallController(Map* acMap)
{
   actualMap = acMap;
   state = WALL_STATE_OTHER;
   actualWall = NULL;
   limitSquare = false;
}

/******************************************************
 *                      Destructor                    *
 ******************************************************/
wallController::~wallController()
{
   actualMap = NULL;
}

/******************************************************
 *                        getWall                     *
 ******************************************************/
wall* wallController::getWall()
{
   wall* aux;
   bool doneWalls2 = false;

   /* Search on Walls */
   aux = actualMap->walls;
   
   if( (!doneWalls2) && (aux == NULL) )
   {
      /* Not Found, So Search on 1/2 Walls */
      aux = actualMap->curbs;
      doneWalls2 = true;
   }

   while(aux != NULL)
   {
      if( ( ((mX >= aux->x1) && (mX <= aux->x2))  ||
            ((mX-1 >= aux->x1) && (mX-1 <= aux->x2)) ||
            ((mX+1 >= aux->x1) && (mX+1 <= aux->x2)) ) &&
          ( ((mZ >= aux->z1) && (mZ <= aux->z2))  ||
            ((mZ-1 >= aux->z1) && (mZ-1 <= aux->z2)) ||
            ((mZ+1 >= aux->z1) && (mZ+1 <= aux->z2)) ) )
      {
         return(aux);
      }
      aux = aux->next;

      if( (!doneWalls2) && (aux == NULL) )
      {
         /* Not Found, So Search on 1/2 Walls */
         aux = actualMap->curbs;
         doneWalls2 = true;
      }
   }
   return(NULL);
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void wallController::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, Uint8* keys,
                        int tool, GLuint actualTexture)
{
   actualTool = tool;
   texture = actualTexture;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;

   if(keys[SDLK_b])
   {
      limitSquare = true;
   }

   if(tool == TOOL_WALL_ADD_X)
   {
      doWall(true, false, true);
   }
   else if(tool == TOOL_WALL2_ADD_X)
   {
      doWall(true, false, false);
   }
   else if(tool == TOOL_WALL_ADD_Z)
   {
      doWall(false, true, true);
   }
   else if(tool == TOOL_WALL2_ADD_Z)
   {
      doWall(false, true, false);
   }
   else if(tool == TOOL_WALL_TEXTURE)
   {
      actualWall = getWall();
      doTexture();
   }
   else if( (tool == TOOL_WALL_LESS_VER_TEXTURE) || 
            (tool == TOOL_WALL_MORE_VER_TEXTURE) ||
            (tool == TOOL_WALL_LESS_HOR_TEXTURE) ||
            (tool == TOOL_WALL_MORE_HOR_TEXTURE))
   {
      actualWall = getWall();
      doModifyVerHorTexture();
   }
}

/******************************************************
 *                      drawTemporary                 *
 ******************************************************/
void wallController::drawTemporary()
{
   glDisable(GL_LIGHTING);
   if(state == WALL_STATE_OTHER)
   {
      glColor3f(0.1, 0.8, 0.8);
      glBegin(GL_QUADS);
      glVertex3f(mX-1, 0.5, mZ-1);
      glVertex3f(mX-1, 0.5, mZ+1);
      glVertex3f(mX+1, 0.5, mZ+1);
      glVertex3f(mX+1, 0.5, mZ-1);
      glEnd();
   }
   glEnable(GL_LIGHTING);
   glColor3f(1.0,1.0,1.0);
}

/******************************************************
 *                doModifyVerHorTexture()             *
 ******************************************************/
void wallController::doModifyVerHorTexture()
{
   state = WALL_STATE_OTHER;
   if( (actualWall) && (mB & SDL_BUTTON(1)) )
   {
      switch(actualTool)
      {
         case TOOL_WALL_LESS_VER_TEXTURE:
            actualWall->dY--;
         break;
         case TOOL_WALL_MORE_VER_TEXTURE:
            actualWall->dY++;
         break;
         case TOOL_WALL_LESS_HOR_TEXTURE:
            actualWall->dX--;
         break;
         case TOOL_WALL_MORE_HOR_TEXTURE:
            actualWall->dX++;
         break;
      }
   }
}

/******************************************************
 *                        doTexture()                 *
 ******************************************************/
void wallController::doTexture()
{
   state =  WALL_STATE_OTHER;
   if( (actualWall) && ((mB & SDL_BUTTON(1))))
   {
      actualWall->texture = texture;
   }
}

/******************************************************
 *                        doWall()                    *
 ******************************************************/
void wallController::doWall(bool X, bool Z, bool full)
{
   if( (state == WALL_STATE_OTHER) && (mB & SDL_BUTTON(1)))
   {
      state = WALL_STATE_ADD_INIT;
      limitSquare = false;
      actualWall = new(wall);
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
      if(full)
      {
         actualWall->next = actualMap->walls;
         actualMap->walls = actualWall;
      }
      else
      {
         actualWall->next = actualMap->curbs;
         actualMap->curbs = actualWall;
      }
      actualWall->x1 = mX;
      actualWall->z1 = mZ;
      actualWall->texture = texture;
      if( X )
      {
          actualWall->x2 = mX;
          if( !full )
          {
             actualWall->z2 = mZ+2.5;  
          }
          else
          {
             actualWall->z2 = mZ+10;
          }
      }
      else
      {
          actualWall->z2 = mZ;
          if ( !full )
          {
             actualWall->x2 = mX+2.5;
          }
          else
          {
             actualWall->x2 = mX+10; 
          }
      }
   }
   else if((state == WALL_STATE_ADD_INIT) && (mB & SDL_BUTTON(1)))
   {
      if(X)
      {
         actualWall->x2 = mX;
         if(limitSquare)
         {
            float cmp = ((int)(actualWall->z1) / actualMap->squareSize())*actualMap->squareSize();
            actualWall->x1 = ((int)floor((actualWall->x1) / actualMap->squareSize()))*actualMap->squareSize();
            actualWall->x2 = ((int)floor((actualWall->x2) / actualMap->squareSize()))*actualMap->squareSize();
            actualWall->z1 = ((int)floor((actualWall->z1 / actualMap->squareSize())))*actualMap->squareSize();
            if(cmp < actualWall->z1)
            {
               if(full)
                  actualWall->z1 = actualWall->z1-10;
               else
                  actualWall->z1 = actualWall->z1-2.5;
            }
            if(full)
            {
               actualWall->z2 = actualWall->z1+10;
            }
            else
            {
               actualWall->z2 = actualWall->z1+2.5;
            }
         }
      }
      else
      {
         actualWall->z2 = mZ;
         if(limitSquare)
         {
            float cmp = ((int)(actualWall->x1) / actualMap->squareSize()) 
                         * actualMap->squareSize();
            actualWall->z1 = ((int)floor((actualWall->z1) / actualMap->squareSize()))
                              * actualMap->squareSize();
            actualWall->z2 = ((int)floor((actualWall->z2) / actualMap->squareSize()))
                              * actualMap->squareSize();
            actualWall->x1 = ((int)floor((actualWall->x1 / actualMap->squareSize())))
                             * actualMap->squareSize();
            if(cmp < actualWall->x1)
            {
               if(full)
                  actualWall->x1 = actualWall->x1-10;
               else
                  actualWall->x1 = actualWall->x1-2.5;
            }
            if(full)
            {
               actualWall->x2 = actualWall->x1+10;
            }
            else
            {
               actualWall->x2 = actualWall->x1+2.5;
            }
         }
      }
   }
   else if( (state == WALL_STATE_ADD_INIT) && (!(mB & SDL_BUTTON(1))))
   {
      state = WALL_STATE_OTHER;

      /* Invert Coordinates, if needed */
      if(actualWall->x2 < actualWall->x1)
      {
         float aux = actualWall->x1;
         actualWall->x1 = actualWall->x2;
         actualWall->x2 = aux;
      }
      if(actualWall->z2 < actualWall->z1)
      {
         float aux = actualWall->z1;
         actualWall->z1 = actualWall->z2;
         actualWall->z2 = aux;
      }
      
      actualWall = NULL;
   }
}

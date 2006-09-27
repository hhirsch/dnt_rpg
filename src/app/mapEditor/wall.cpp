#include "wall.h"

#define WALL_STATE_OTHER 0
#define WALL_ADD_X_INIT  1
#define WALL_ADD_Z_INIT  2
#define WALL_ADD2_X_INIT 3
#define WALL_ADD2_Z_INIT 4

/******************************************************
 *                      Constructor                   *
 ******************************************************/
wall::wall(Map* map)
{
   actualMap = map;
   state = WALL_STATE_OTHER;
}

/******************************************************
 *                      Destructor                    *
 ******************************************************/
wall::~wall()
{
   actualMap = NULL;
}

/******************************************************
 *                        getWall                     *
 ******************************************************/
muro* wall::getWall()
{
   muro* aux;
   bool doneWalls2 = false;

   /* Search on Walls */
   aux = actualMap->muros;
   
   if( (!doneWalls2) && (aux == NULL) )
   {
      /* Not Found, So Search on 1/2 Walls */
      aux = actualMap->meiosFio;
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
      aux = aux->proximo;

      if( (!doneWalls2) && (aux == NULL) )
      {
         /* Not Found, So Search on 1/2 Walls */
         aux = actualMap->meiosFio;
         doneWalls2 = true;
      }
   }
   return(NULL);
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void wall::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool, GLuint actualTexture)
{
   actualTool = tool;
   texture = actualTexture;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;
   actualWall = getWall();

   if( (tool == TOOL_WALL_ADD_X) || (tool == TOOL_WALL2_ADD_X))
   {
   }
   else if((tool == TOOL_WALL_ADD_Z) || (tool == TOOL_WALL2_ADD_Z))
   {
   }
   else if(tool == TOOL_WALL_TEXTURE)
   {
      doTexture();
   }
   else if( (tool == TOOL_WALL_LESS_VER_TEXTURE) || 
            (tool == TOOL_WALL_MORE_VER_TEXTURE) ||
            (tool == TOOL_WALL_LESS_HOR_TEXTURE) ||
            (tool == TOOL_WALL_MORE_HOR_TEXTURE))
   {
      doModifyVerHorTexture();
   }
}

/******************************************************
 *                      drawTemporary                 *
 ******************************************************/
void wall::drawTemporary()
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
void wall::doModifyVerHorTexture()
{
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
void wall::doTexture()
{
   if( (actualWall) && ((mB & SDL_BUTTON(1))))
   {
      actualWall->textura = texture;
   }
}


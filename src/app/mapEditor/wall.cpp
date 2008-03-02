#include "wall.h"

#define WALL_STATE_OTHER     0
#define WALL_STATE_ADD_INIT  1

#define MARK_SIZE 10

/******************************************************
 *                      Constructor                   *
 ******************************************************/
wallController::wallController(Map* acMap)
{
   dirs dir;
   actualMap = acMap;
   state = WALL_STATE_OTHER;
   actualWall = actualMap->getFirstWall();
   limitSquare = false;

   /* Load Mark Texture */
   SDL_Surface* img = IMG_Load(dir.getRealFile("cursors/Walk.png").c_str());
   if(!img)
   {
      printf("Can't open cursors/Walk.png !\n");
      return;
   }
   glGenTextures(1,&markTexture);
   setTextureRGBA(img, markTexture);
   SDL_FreeSurface(img);
}

/******************************************************
 *                      Destructor                    *
 ******************************************************/
wallController::~wallController()
{
   glDeleteTextures(1,&markTexture);
   actualMap = NULL;
}

/******************************************************
 *                        getWall                     *
 ******************************************************/
wall* wallController::getWall()
{
   //FIXME do the search on curbs too!
   wall* aux;
   int wNum;

   /* Search on Walls */
   aux = actualMap->getFirstWall();
   
   for(wNum = 0; wNum < actualMap->getTotalWalls(); wNum++)
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
   }
   return(NULL);
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void wallController::verifyAction(GLfloat mouseX, GLfloat mouseY, 
                                  GLfloat mouseZ, Uint8 mButton, 
                                  Uint8* keys, int& tool, GLuint actualTexture)
{
   actualTool = tool;
   texture = actualTexture;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;

   wall* tmpWall = NULL;

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
      tmpWall = getWall();
      if(tmpWall)
      {
         actualWall = tmpWall;
         doTexture();
      }
   }
   else if( (tool == TOOL_WALL_LESS_Y_TEXTURE) || 
            (tool == TOOL_WALL_MORE_Y_TEXTURE) ||
            (tool == TOOL_WALL_LESS_X_TEXTURE) ||
            (tool == TOOL_WALL_MORE_X_TEXTURE) ||
            (tool == TOOL_WALL_LESS_Z_TEXTURE) || 
            (tool == TOOL_WALL_MORE_Z_TEXTURE) )
   {
      tmpWall = getWall();
      if(tmpWall)
      {
         actualWall = tmpWall;
         doModifyVerHorTexture();
      }
   }
   else if(tool == TOOL_WALL_NEXT)
   {
      if(actualWall)
      {
         actualWall = actualWall->next;
      }
      state = WALL_STATE_OTHER;
      tool = TOOL_NONE;
   }
   else if(tool == TOOL_WALL_PREVIOUS)
   {
      if(actualWall)
      {
         actualWall = actualWall->previous;
      }
      state = WALL_STATE_OTHER;
      tool = TOOL_NONE;
   }
   else if(tool == TOOL_WALL_DESTROY)
   {
      if(actualWall)
      {
         actualMap->removeWall(actualWall);
         actualWall = actualMap->getFirstWall();
      }
      state = WALL_STATE_OTHER;
      tool = TOOL_NONE;
   }
}

/******************************************************
 *                      drawTemporary                 *
 ******************************************************/
void wallController::drawTemporary(GLdouble modelView[16], 
                                   GLfloat camX, GLfloat camY, GLfloat camZ)
{
   GLfloat scale = 1.0;
   GLfloat dist = 0;

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

   if(actualWall)
   {
      /* Draw the indicator of actualWall */
      GLfloat px = (actualWall->x2+actualWall->x1) / 2.0;
      GLfloat pz = (actualWall->z2+actualWall->z1) / 2.0;

      /* Calculate scale factor */
      dist = sqrt( (camX-px)*(camX-px) +
                   (camY-WALL_HEIGHT)*(camY-WALL_HEIGHT) +
                   (camZ-pz)*(camZ-pz) );
      scale = dist / 500.0;

      glColor4f(1,1,1,1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glPushMatrix();
         glTranslatef(px, WALL_HEIGHT, pz);
         glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, markTexture );
      glBegin(GL_QUADS);
         glTexCoord2f(0,0);
         glVertex3f(-MARK_SIZE*modelView[0]*scale,0.0f,
                    -MARK_SIZE*modelView[8]*scale);
         glTexCoord2f(1,0);
         glVertex3f(MARK_SIZE*modelView[0]*scale,0.0f,
                    MARK_SIZE*modelView[8]*scale);
         glTexCoord2f(1,1);
         glVertex3f(MARK_SIZE*modelView[0]*scale + scale*16*modelView[1], 
                    scale*16*modelView[5] + scale*MARK_SIZE*modelView[4], 
                    scale*MARK_SIZE*modelView[8] + scale*16*modelView[9]);
         glTexCoord2f(0,1);
         glVertex3f(-MARK_SIZE*modelView[0]*scale + scale*16*modelView[1], 
                    scale*16*modelView[5] - MARK_SIZE*modelView[4]*scale,
                    -MARK_SIZE*modelView[8]*scale + 16*modelView[9]*scale);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
      glPopMatrix();
   }

   glEnable(GL_LIGHTING);
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
         case TOOL_WALL_LESS_Y_TEXTURE:
            actualWall->dY--;
         break;
         case TOOL_WALL_MORE_Y_TEXTURE:
            actualWall->dY++;
         break;
         case TOOL_WALL_LESS_X_TEXTURE:
            actualWall->dX--;
         break;
         case TOOL_WALL_MORE_X_TEXTURE:
            actualWall->dX++;
         break;
         case TOOL_WALL_LESS_Z_TEXTURE:
            actualWall->dZ--;
         break;
         case TOOL_WALL_MORE_Z_TEXTURE:
            actualWall->dZ++;
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
      if(full)
      {
         actualWall = actualMap->addWall(0,0,0,0);
      }
      else
      {
         //FIXME Curbs add!
         cerr << "Error: Removed the curbs! FIXME!" << endl;
      }
      actualWall->dX = 16; actualWall->dY = 16; actualWall->dZ = 16;
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
      
   }
}

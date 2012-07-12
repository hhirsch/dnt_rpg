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

#include "wall.h"
#include "../../engine/util.h"
#include <iostream>
using namespace std;
using namespace dntMapEditor;

#define WALL_STATE_OTHER     0
#define WALL_STATE_ADD_INIT  1

#define MARK_SIZE 10

/******************************************************
 *                      Constructor                   *
 ******************************************************/
WallController::WallController(Map* acMap, Farso::GuiInterface* g)
{
   dirs dir;
   actualMap = acMap;
   state = WALL_STATE_OTHER;
   actualWall = actualMap->getFirstWall();
   limitSquare = false;

   gui = g;
   detailWindow = NULL;
   detailWall = NULL;
   faceWindow = NULL;
   curFace = NULL;

   /* Load Mark Texture */
   SDL_Surface* img = IMG_Load(dir.getRealFile("cursors/Walk.png").c_str());
   if(!img)
   {
      cerr << "Couldn't open 'cursors/Walk.png'!" << endl;
      return;
   }
   glGenTextures(1,&markTexture);
   Farso::setTextureRGBA(img, markTexture);
   SDL_FreeSurface(img);

   /* Load face mark texture */
   img = IMG_Load(dir.getRealFile("texturas/general/travelmark.png").c_str());
   if(!img)
   {
      cerr << "Couldn't open 'texturas/general/travelmark.png'!" << endl;
      return;
   }
   glGenTextures(1,&faceMarkTexture);
   Farso::setTextureRGBA(img, faceMarkTexture);
   SDL_FreeSurface(img);
}

/******************************************************
 *                      Destructor                    *
 ******************************************************/
WallController::~WallController()
{
   glDeleteTextures(1,&markTexture);
   glDeleteTextures(1,&faceMarkTexture);
   actualMap = NULL;

   if(detailWindow)
   {
      gui->closeWindow(detailWindow);
   }
   if(faceWindow)
   {
      gui->closeWindow(faceWindow);
   }
}

/******************************************************
 *                        getWall                     *
 ******************************************************/
wall* WallController::getWall(int radius)
{
   wall* aux;
   int wNum;

   /* Search on Walls */
   aux = actualMap->getFirstWall();
   
   for(wNum = 0; wNum < actualMap->getTotalWalls(); wNum++)
   {
      if( ( ((mX >= aux->x1) && (mX <= aux->x2))  ||
            ((mX-radius >= aux->x1) && (mX-radius <= aux->x2)) ||
            ((mX+radius >= aux->x1) && (mX+radius <= aux->x2)) ) &&
          ( ((mZ >= aux->z1) && (mZ <= aux->z2))  ||
            ((mZ-radius >= aux->z1) && (mZ-radius <= aux->z2)) ||
            ((mZ+radius >= aux->z1) && (mZ+radius <= aux->z2)) ) )
      {
         return(aux);
      }
      aux = (wall*)aux->getNext();
   }
   return(NULL);
}

/******************************************************
 *                      verifyAction                  *
 ******************************************************/
void WallController::verifyAction(GLfloat mouseX, GLfloat mouseY, 
                                  GLfloat mouseZ, GLfloat floorX,
                                  GLfloat floorZ, Uint8 mButton, 
                                  Uint8* keys, int& tool, 
                                  GLuint curTexture, string curTextureName)
{
   actualTool = tool;
   texture = curTexture;
   textureName = curTextureName;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   fX = floorX;
   fZ = floorZ;
   mB = mButton;

   /* Limit Wall to the square limits */
   if(keys[SDLK_b])
   {
      limitSquare = true;
   }

   /* Add Wall Tool */
   if(tool == TOOL_WALL_ADD)
   {
      doWall();
   }
   /* Edit Wall Toold */
   else if(tool == TOOL_WALL_EDIT)
   {
      /* TODO */
   }

   else if(tool == TOOL_WALL_EDIT_FACE)
   {
      actualWall = getWall();
      if(actualWall)
      {
         if(mButton & SDL_BUTTON(1))
         {
            openFaceWindow();
         }
      }
   }

   /* Add texture to the wall */
   else if(tool == TOOL_WALL_TEXTURE)
   {
      actualWall = getWall();
      if(actualWall)
      {
         doTexture();
      }
   }

   /* Modify texture repeat-rate  */
   else if( (tool == TOOL_WALL_LESS_Y_TEXTURE) || 
            (tool == TOOL_WALL_MORE_Y_TEXTURE) ||
            (tool == TOOL_WALL_LESS_X_TEXTURE) ||
            (tool == TOOL_WALL_MORE_X_TEXTURE) ||
            (tool == TOOL_WALL_LESS_Z_TEXTURE) || 
            (tool == TOOL_WALL_MORE_Z_TEXTURE) )
   {
      actualWall = getWall();
      if(actualWall)
      {
         doModifyVerHorTexture();
      }
   }

   /* Destroy Current Wall */
   else if(tool == TOOL_WALL_DESTROY)
   {
      actualWall = getWall();
      if(actualWall)
      {
         doDestroy();
      }
   }

   /* Cut Current Wall */
   else if(tool == TOOL_WALL_CUT)
   {
      actualWall = getWall();
      if(actualWall)
      {
         doCut();
      }
   }
}

/******************************************************
 *                      drawTemporary                 *
 ******************************************************/
void WallController::drawTemporary(GLdouble modelView[16], 
                                   GLfloat camX, GLfloat camY, GLfloat camZ)
{
   GLfloat scale = 1.0;
   GLfloat dist = 0;

   glDisable(GL_LIGHTING);
   if(state == WALL_STATE_OTHER)
   {
      glColor3f(0.1, 0.8, 0.8);
      glBegin(GL_QUADS);
      glVertex3f(fX-1, 0.5, fZ-1);
      glVertex3f(fX-1, 0.5, fZ+1);
      glVertex3f(fX+1, 0.5, fZ+1);
      glVertex3f(fX+1, 0.5, fZ-1);
      glEnd();
   }

   if(actualWall)
   {
      glColor4f(1,1,1,1);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

      /* Draw the indicator of actual face */
      GLfloat x1=0.0f,x2=0.0f,
              y=WALL_HEIGHT/2.0f+10,
              z1=0.0f,z2=0.0f;
      if(getSideTexture(&x1, &z1, &x2, &z2))
      {
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, faceMarkTexture );

         glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(x1, y-8, z1);
            glTexCoord2f(1,0);
            glVertex3f(x2, y-8, z2);
            glTexCoord2f(1,1);
            glVertex3f(x2, y+8, z2);
            glTexCoord2f(0,1);
            glVertex3f(x1, y+8, z1);
         glEnd();
         glDisable(GL_TEXTURE_2D);
      }

      /* Draw the indicator of actualWall */
      GLfloat px = (actualWall->x2+actualWall->x1) / 2.0;
      GLfloat pz = (actualWall->z2+actualWall->z1) / 2.0;

      /* Calculate scale factor */
      dist = sqrt( (camX-px)*(camX-px) +
                   (camY-WALL_HEIGHT)*(camY-WALL_HEIGHT) +
                   (camZ-pz)*(camZ-pz) );
      scale = dist / 500.0;

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
void WallController::doModifyVerHorTexture()
{
   GLuint dX=0, dY=0, dZ=0;

   state = WALL_STATE_OTHER;
   if( (actualWall) && (mB & SDL_BUTTON(1)) )
   {
      wallTexture* tex = getSideTexture();

      if(tex)
      {
         tex->getDelta(dX, dY, dZ);
         switch(actualTool)
         {
            case TOOL_WALL_LESS_Y_TEXTURE:
               tex->setDelta(dX, dY-1, dZ);
            break;
            case TOOL_WALL_MORE_Y_TEXTURE:
               tex->setDelta(dX, dY+1, dZ);
            break;
            case TOOL_WALL_LESS_X_TEXTURE:
               tex->setDelta(dX-1, dY, dZ);
            break;
            case TOOL_WALL_MORE_X_TEXTURE:
               tex->setDelta(dX+1, dY, dZ);
            break;
            case TOOL_WALL_LESS_Z_TEXTURE:
               tex->setDelta(dX, dY, dZ-1);
            break;
            case TOOL_WALL_MORE_Z_TEXTURE:
               tex->setDelta(dX, dY, dZ+1);
            break;
         }
      }
   }
}

/******************************************************
 *                    getSideTexture()                *
 ******************************************************/
wallTexture* WallController::getSideTexture(float* x1, float* z1, 
      float* x2, float* z2)
{
   float middleX = ((actualWall->x2)-(actualWall->x1)) / 2.0f;
   float middleZ = ((actualWall->z2)-(actualWall->z1)) / 2.0f;

   if((mZ > actualWall->z1) && (mZ < actualWall->z2))
   {
      if(mX >= actualWall->x2)
      {
         /* Right Texture */
         if( (x1) && (x2) && (z1) && (z2) )
         {
            *x1 = actualWall->x2 + 2;
            *x2 = actualWall->x2 + 2;
            *z1 = actualWall->z1 + middleZ - 8;
            *z2 = actualWall->z1 + middleZ + 8;
         }
         return(&actualWall->rightTexture);
      }
      else if(mX <= actualWall->x1)
      {
         /* Left Texture */
         if( (x1) && (x2) && (z1) && (z2) )
         {
            *x1 = actualWall->x1 - 2;
            *x2 = actualWall->x1 - 2;
            *z1 = actualWall->z1 + middleZ-8;
            *z2 = actualWall->z1 + middleZ+8;
         }
         return(&actualWall->leftTexture);
      }
   }
   else
   {
      if(mZ <= actualWall->z1)
      {
         /* Front Texture */
         if( (x1) && (x2) && (z1) && (z2) )
         {
            *x1 = actualWall->x1 + middleX-8;
            *x2 = actualWall->x1 + middleX+8;
            *z1 = actualWall->z1-2;
            *z2 = actualWall->z1-2;
         }
         return(&actualWall->frontTexture);
      }
      else if(mZ >= actualWall->z2)
      {
         /* Back Texture */
         if( (x1) && (x2) && (z1) && (z2) )
         {
            *x1 = actualWall->x1 + middleX-8;
            *x2 = actualWall->x1 + middleX+8;
            *z1 = actualWall->z2+2;
            *z2 = actualWall->z2+2;
         }
         return(&actualWall->backTexture);
      }
   }

   /* No texture Defined */
   return(NULL);
}

/******************************************************
 *                        doTexture()                 *
 ******************************************************/
void WallController::doTexture()
{
   state =  WALL_STATE_OTHER;

   if( (actualWall) && ((mB & SDL_BUTTON(1))))
   {
      wallTexture* tex = getSideTexture();
      if(tex != NULL)
      {
         tex->setTextureId(texture);
         tex->setTextureName(textureName);
      }
   }
}

/******************************************************
 *                         doCut                      *
 ******************************************************/
bool WallController::doCut()
{
   state =  WALL_STATE_OTHER;

   if( (actualWall) && ((mB & SDL_BUTTON(1))))
   {
      bool wallZ = (actualWall->x2 - actualWall->x1 == 10);

      /* Insert a new wall in the map (it'll have the other half) */
      wall* newWall = actualMap->addWall(0,0,0,0);
      newWall->frontTexture = actualWall->frontTexture;
      newWall->backTexture = actualWall->backTexture;
      newWall->leftTexture = actualWall->leftTexture;
      newWall->rightTexture = actualWall->rightTexture;

      /* Cut the wall (halt is the current, half is the new) */
      if(wallZ)
      {
         /* Cut Z element */
         newWall->x1 = actualWall->x1;
         newWall->x2 = actualWall->x2;
         newWall->z1 = mZ;
         newWall->z2 = actualWall->z2;
         actualWall->z2 = mZ;
      }
      else
      {
         /* Cut X element */
         newWall->x1 = mX;
         newWall->x2 = actualWall->x2;
         newWall->z1 = actualWall->z1;
         newWall->z2 = actualWall->z2;
         actualWall->x2 = mX;
      }

      /* Wait mouse release */
      while(mB & SDL_BUTTON(1))
      {
         SDL_PumpEvents();
         int mx,my;
         mB = SDL_GetMouseState(&mx,&my);
      }

      return(true);
   }

   return(false);
}

/******************************************************
 *                       doDestroy                    *
 ******************************************************/
bool WallController::doDestroy()
{
   state =  WALL_STATE_OTHER;

   if( (actualWall) && ((mB & SDL_BUTTON(1))))
   {
      actualMap->removeWall(actualWall);
      actualWall = actualMap->getFirstWall();
      /* Wait mouse release */
      while(mB & SDL_BUTTON(1))
      {
         SDL_PumpEvents();
         int mx,my;
         mB = SDL_GetMouseState(&mx,&my);
      }
      return(true);
   }

   return(false);
}

/******************************************************
 *                        doWall()                    *
 ******************************************************/
void WallController::doWall()
{
   wall* nearWall = getWall(10);

   if( (state == WALL_STATE_OTHER) && (mB & SDL_BUTTON(1)))
   {
      state = WALL_STATE_ADD_INIT;
      limitSquare = false;
      actualWall = actualMap->addWall(0,0,0,0);

      /* Set texture to the current one */
      actualWall->frontTexture.setTextureId(texture);
      actualWall->frontTexture.setTextureName(textureName);
      actualWall->backTexture.setTextureId(texture);
      actualWall->backTexture.setTextureName(textureName);
      actualWall->leftTexture.setTextureId(texture);
      actualWall->leftTexture.setTextureName(textureName);
      actualWall->rightTexture.setTextureId(texture);
      actualWall->rightTexture.setTextureName(textureName);
      
      cerr << fX << " " << fZ << endl;
      /* Set initial position */
      if(!nearWall)
      {
         actualWall->x1 = fX;
         actualWall->x2 = fX;
         actualWall->z1 = fZ;
         actualWall->z2 = fZ;
      }
      else
      {
         actualWall->x1 = fX;
         actualWall->z1 = fZ;
         actualWall->x2 = fX;
         actualWall->z2 = fZ;

         /* Put it side by side with the wall */
         if( fabsf(nearWall->x1 - fX) <= 10)
         {
            actualWall->x1 = nearWall->x1;
         }
         else if( fabsf(nearWall->x2 - fX) <= 10)
         {
            actualWall->x1 = nearWall->x2;
         }
         if( fabsf(nearWall->z1 - fZ) <= 10)
         {
            actualWall->z1 = nearWall->z1;
         }
         else if( fabsf(nearWall->z2 - fZ) <= 10)
         {
            actualWall->z1 = nearWall->z2;
         }
      }

   }
   else if((state == WALL_STATE_ADD_INIT) && (mB & SDL_BUTTON(1)))
   {
      if(fabsf(fX - actualWall->x1) >= fabs(fZ - actualWall->z1) )
      {
         /* TODO: limit square! */

         /* Do on X */
         if(!nearWall)
         {
            actualWall->x2 = fX;
         }
         else
         {
            /* Put it side by side with the wall */
            if( fabsf(nearWall->x1 - fX) <= 10)
            {
               actualWall->x2 = nearWall->x1;
            }
            else if( fabsf(nearWall->x2 - fX) <= 10)
            {
               actualWall->x2 = nearWall->x2;
            }
         }

         /* Limit Square */
         if(limitSquare)
         {
            actualWall->x1 = ((int)floor((actualWall->x1) / 
                     actualMap->squareSize()))*actualMap->squareSize();
            actualWall->x2 = ((int)floor((actualWall->x2) / 
                     actualMap->squareSize()))*actualMap->squareSize();
            actualWall->z1 = ((int)floor((actualWall->z1 / 
                        actualMap->squareSize())))*actualMap->squareSize();
         }

         /* And set the Z fixed */
         if(fZ >= actualWall->z1)
         {
            actualWall->z2 = actualWall->z1+10;
         }
         else
         {
            actualWall->z2 = actualWall->z1-10;
         }
      }
      else
      {
         /* Do on Z */
         if(!nearWall)
         {
            actualWall->z2 = fZ;
         }
         else
         {
            /* Put it at the nearWall */
            if( fabsf(nearWall->z1 - fZ) <= 10)
            {
               actualWall->z2 = nearWall->z1;
            }
            else if( fabsf(nearWall->z2 - fZ) <= 10)
            {
               actualWall->z2 = nearWall->z2;
            }
         }

         /* Limit Square */
         if(limitSquare)
         {
            actualWall->z1 = ((int)floor((actualWall->z1) / 
                     actualMap->squareSize()))*actualMap->squareSize();
            actualWall->z2 = ((int)floor((actualWall->z2) / 
                     actualMap->squareSize()))*actualMap->squareSize();
            actualWall->x1 = ((int)floor((actualWall->x1 / 
                        actualMap->squareSize())))*actualMap->squareSize();
         }

         /* And set the X fixed */
         if(fX >= actualWall->x1)
         {
            actualWall->x2 = actualWall->x1+10;
         }
         else
         {
            actualWall->x2 = actualWall->x1-10;
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

/******************************************************
 *                       eventGot()                   *
 ******************************************************/
bool WallController::eventGot(int eventInfo, Farso::GuiObject* obj)
{
   bool got = false;
   if(eventInfo == Farso::EVENT_ON_PRESS_BUTTON)
   {
      /* X */
      if(obj == (Farso::GuiObject*)decXRep)
      {
         repX -= 1;
         if(repX <= 0)
         {
            repX = 1;
         }
         got = true;
      }
      else if(obj == (Farso::GuiObject*)incXRep)
      { 
         repX += 1;
         got = true;
      }
      /* Y */
      else if(obj == (Farso::GuiObject*)decYRep)
      {
         repY -= 1;
         if(repY <= 0)
         {
            repY = 1;
         }
         got = true;
      }
      else if(obj == (Farso::GuiObject*)incYRep)
      {
         repY += 1;
         got = true;
      }

      /* Z */
      else if(obj == (Farso::GuiObject*)decZRep)
      {
         repZ -= 1;
         if(repZ <= 0)
         {
            repZ = 1;
         }
         got = true;
      }
      else if(obj == (Farso::GuiObject*)incZRep)
      {
         repZ += 1;
         got = true;
      }
   }
   else if(eventInfo == Farso::EVENT_WROTE_TEXT_BAR)
   {
      if(obj == (Farso::GuiObject*)curXRep)
      {
         sscanf(curXRep->getText().c_str(), "%d", &repX);
         if(repX <= 0)
         {
            repX = 1;
         }
         got = true;
      }
      else if(obj == (Farso::GuiObject*)curYRep)
      {
         sscanf(curYRep->getText().c_str(), "%d", &repY);
         if(repY <= 0)
         {
            repY = 1;
         }
         got = true;
      }
      else if(obj == (Farso::GuiObject*)curZRep)
      {
         sscanf(curZRep->getText().c_str(), "%d", &repZ);
         if(repZ <= 0)
         {
            repZ = 1;
         }
         got = true;
      }
   }

   if( (got) && (curFace))
   {
      curFace->setDelta(repX, repY, repZ);
      setFaceValues();
   }

   return(got);
}

/******************************************************
 *                     openFaceWindow()               *
 ******************************************************/
void WallController::openFaceWindow()
{
   int posY;
   Farso::Font fnt;
   dirs dir;
   string fontArial = dir.getRealFile(DNT_FONT_ARIAL);

   wallTexture* wt=NULL;
   if(actualWall)
   {
      wt = getSideTexture();
      if(wt)
      {
         curFace = wt;

         /* Open the window, if already not opened */
         if(!faceWindow)
         {
            /* Create window and widgets */
            faceWindow = gui->insertWindow(400,200,600,290,"Wall Face");

            posY = 25;
            decXRep = faceWindow->getObjectsList()->insertButton(11,posY,
                  21,posY+17, fnt.createUnicode(0x25C4),0);
            decXRep->defineFont(fontArial, 9);
            curXRep = faceWindow->getObjectsList()->insertTextBar(23,posY,
                  86,posY+17,"",0);
            incXRep = faceWindow->getObjectsList()->insertButton(88,posY,
                  98,posY+17, fnt.createUnicode(0x25BA),0);
            incXRep->defineFont(fontArial, 9);
            faceWindow->getObjectsList()->insertTextBox(100,posY,
                  186,posY+17,0,"Texture X Repeat");
            posY += 20;

            decYRep = faceWindow->getObjectsList()->insertButton(11,posY,
                  21,posY+17, fnt.createUnicode(0x25C4),0);
            decYRep->defineFont(fontArial, 9);
            curYRep = faceWindow->getObjectsList()->insertTextBar(23,posY,
                  86,posY+17,"",0);
            incYRep = faceWindow->getObjectsList()->insertButton(88,posY,
                  98,posY+17, fnt.createUnicode(0x25BA),0);
            incYRep->defineFont(fontArial, 9);
            faceWindow->getObjectsList()->insertTextBox(100,posY,
                  186,posY+17,0,"Texture Y Repeat");
            posY += 20;

            decZRep = faceWindow->getObjectsList()->insertButton(11,posY,
                  21,posY+17, fnt.createUnicode(0x25C4),0);
            decZRep->defineFont(fontArial, 9);
            curZRep = faceWindow->getObjectsList()->insertTextBar(23,posY,
                  86,posY+17,"",0);
            incZRep = faceWindow->getObjectsList()->insertButton(88,posY,
                  98,posY+17, fnt.createUnicode(0x25BA),0);
            incZRep->defineFont(fontArial, 9);
            faceWindow->getObjectsList()->insertTextBox(100,posY,
                  186,posY+17,0,"Texture Z Repeat");
            posY += 20;

            /* Finally, open */
            faceWindow->setExternPointer(&faceWindow);
            gui->openWindow(faceWindow);
         }
         curFace->getDelta(repX, repY, repZ);
         setFaceValues();
      }
   }
}

/******************************************************
 *                     setFaceValues()                *
 ******************************************************/
void WallController::setFaceValues()
{
   if(faceWindow)
   {
      /* Set the face values on gui */
      char val[16];

      sprintf(val,"%d", repX);
      curXRep->setText(val);
      sprintf(val,"%d", repY);
      curYRep->setText(val);
      sprintf(val,"%d", repZ);
      curZRep->setText(val);
   }
}


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

#include "portal.h"
#include "../../gui/farso.h"
#include "../../gui/messages.h"

#include <iostream>
using namespace std;
using namespace dntMapEditor;

#define PORTAL_STATE_OTHER     0
#define PORTAL_STATE_ADD_INIT  1

/******************************************************
 *                      Constructor                   *
 ******************************************************/
Portal::Portal(Map* acMap, Farso::GuiInterface* g)
{
   gui = g;
   actualMap = acMap;
   state = PORTAL_STATE_OTHER;
   fileDoor = "";
   portalList = new(AreaList);
   /* Start the Portal List */
   Square* s;
   int x, z;
   for(x=0; x<actualMap->getSizeX(); x++)
   {
      for(z=0; z<actualMap->getSizeZ();z++)
      {
         s = actualMap->relativeSquare(x,z);
         if(s->mapConection.active)
         {
            portalList->addArea(s->mapConection.x1, s->mapConection.z1,
                                s->mapConection.x2, s->mapConection.z2,
                                s->mapConection.mapName);
         }
      }
   }
   actualDoor = NULL;
   doorWall = NULL;
   curLockDoor = NULL;
   lockWindow = NULL;
   doorMode = 0;
}

/******************************************************
 *                       Destructor                   *
 ******************************************************/
Portal::~Portal()
{
   actualMap = NULL;
   delete(portalList);
}

/******************************************************
 *                        inner                       *
 ******************************************************/
bool Portal::inner(GLfloat ax, GLfloat az, GLfloat bx1, GLfloat bz1, 
                                          GLfloat bx2, GLfloat bz2)
{
   return( (ax >= bx1 ) && (ax <= bx2) && (az >= bz1) && (az <=bz2) );
}

/******************************************************
 *                     defineDoor                     *
 ******************************************************/
void Portal::defineDoor(object* newDoor, string fileName)
{
   if(newDoor != NULL)
   {
      actualDoor = newDoor;
      actualDoor->setThingType(THING_TYPE_DOOR);
      fileDoor = fileName;
   }
}

/******************************************************
 *                       getDoor                      *
 ******************************************************/
object* Portal::getDoor()
{
   return(actualDoor);
}

/******************************************************
 *                       getDoor                      *
 ******************************************************/
string Portal::getDoorFileName()
{
   return(fileDoor);
}

/******************************************************
 *                    deleteDoor                      *
 ******************************************************/
void Portal::deleteDoor()
{
   if(actualDoor)
   {
      delete(actualDoor);
      fileDoor = "";
      actualDoor = NULL;
   }
}

/******************************************************
 *                       VerifyAction                 *
 ******************************************************/
void Portal::verifyAction(GLfloat mouseX, GLfloat mouseY, 
                          GLfloat mouseZ, Uint8 mButton, int tool,
                          GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   actualTool = tool;
   mX = mouseX;
   mY = mouseY;
   mZ = mouseZ;
   mB = mButton;

   if(tool == TOOL_PORTAL_ADD)
   {
      doAddPortal();
   }
   else if (tool == TOOL_PORTAL_TAG)
   {
      doTagPortal(proj,modl,viewPort);
   }
   else if(tool == TOOL_PORTAL_DOOR_LOCK)
   {
      doDoorLockUnlock(mouseX, mouseY, mouseZ, mButton);
   }
   else if( (tool == TOOL_PORTAL_DOOR) && (actualDoor))
   {
      /* Take nearest wall */
      wall* m = actualMap->getFirstWall();
      int wNum,  wTotal;
      wTotal = actualMap->getTotalWalls();
      doorWall = m;

      if(!doorWall)
      {
         return;
      }
      
      for( wNum = 0; wNum < wTotal; wNum++ )
      {
           if( inner(mX,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ-2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX,mZ+2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX-2,mZ+2,m->x1,m->z1,m->x2,m->z2) ||
               inner(mX+2,mZ+2,m->x1,m->z1,m->x2,m->z2))
           {
              doorWall = m;
           }

           m = (wall*)m->getNext();
      }

      /* Set door X or Z on wall */
      if( (doorWall->x2 - doorWall->x1) == 10)
      {
          if(doorMode)
          {
              doorX = doorWall->x2;
          }
          else
          {
              doorX = doorWall->x1;
          }
          doorOrientation = 90;  
          if(mZ > doorWall->z2)
          {
              doorZ = doorWall->z2; 
          }
          else if(mZ < doorWall->z1)
          {
              doorZ = doorWall->z1;
          } 
          else
          {
              doorZ = mZ; 
          }
      }
      else
      {
          if(doorMode)
          {
              doorZ = doorWall->z2;
          }
          else
          {
              doorZ = doorWall->z1;
          }
          doorOrientation = 0;
          if(mX > doorWall->x2)
          {
              doorX = doorWall->x2; 
          }
          else if(mX < doorWall->x1)
          {
              doorX = doorWall->x1;
          } 
          else
          {
              doorX = mX; 
          }
      }
   
      if(mButton & SDL_BUTTON(1))
      {
         /* Insert the door */
         GLfloat mx1,mx2,mz1,mz2;
         mx1 = doorWall->x1;
         mx2 = doorWall->x2;
         mz1 = doorWall->z1;
         mz2 = doorWall->z2;
         wall* novoMuro;

         /* "Break" the wall to door come in */
         novoMuro = actualMap->addWall(0,0,0,0);
         boundingBox bounds = actualDoor->scNode->getBoundingBox();
         if( doorOrientation == 0 )
         {
            doorWall->x2 = doorX;
            novoMuro->x1 = doorX + (bounds.max.x - bounds.min.x);
            novoMuro->x2 = mx2;
            novoMuro->z1 = mz1;
            novoMuro->z2 = mz2;
         }
         else
         {
            doorWall->z2 = doorZ - (bounds.max.z - bounds.min.z);
            novoMuro->z1 = doorZ;
            novoMuro->x2 = mx2;
            novoMuro->x1 = mx1;
            novoMuro->z2 = mz2;
         }
         novoMuro->frontTexture = doorWall->frontTexture;
         novoMuro->backTexture = doorWall->backTexture;
         novoMuro->leftTexture = doorWall->leftTexture;
         novoMuro->rightTexture = doorWall->rightTexture;
         
         /* Add the new door to the map */
         door* novaPorta = new(door);
         novaPorta->obj = actualDoor;
         novaPorta->obj->scNode->set(
               doorX,0.0f,doorZ, 0.0f, doorOrientation, 0.0f);

         
         actualMap->insertDoor(novaPorta);
         cerr << "Added Door: " << doorX << " " << doorZ << " " 
              << doorOrientation << endl;
         while(mButton & SDL_BUTTON(1))
         {
            /* Wait for Mouse Button Release */
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }

         /* Unset the pointer */
         actualDoor = NULL;
      }
      
      if(mButton & SDL_BUTTON(3))
      {
         doorMode = !doorMode;
         while(mButton & SDL_BUTTON(3))
         {
            /* Wait for Mouse Button Release */
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
}

/******************************************************
 *                       drawTemporary                *
 ******************************************************/
void Portal::drawTemporary()
{
   glPushMatrix();
   int delta = -2;
   if( (actualTool == TOOL_PORTAL_DOOR) && (actualDoor))
   {
      /* Set a delta to we ca see the door "outter" the wall */
      if(doorMode)
      {
         delta = 2;
      }

      /* SetPosition */
      if(doorOrientation)
      {
         actualDoor->scNode->set(doorX+delta,0.0f,doorZ,
               0.0f,doorOrientation,0.0f);
      }
      else
      {
         actualDoor->scNode->set(doorX,0.0f,doorZ+delta,
               0.0f,doorOrientation,0.0f);
      }

      /* Render a mouse position */
      glBegin(GL_QUADS);
         glVertex3f(mX-2,1,mZ-2);
         glVertex3f(mX-2,1,mZ+2);
         glVertex3f(mX+2,1,mZ+2);
         glVertex3f(mX+2,1,mZ-2);
      glEnd();
   }
   else if((actualTool == TOOL_PORTAL_DOOR_LOCK) && (curLockDoor))
   {
      curLockDoor->obj->scNode->getBoundingBox().render();
   }
   glPopMatrix();

   glDisable(GL_LIGHTING);
   /* Draw In course Map Portal Add */
   if(state == PORTAL_STATE_ADD_INIT)
   {
      glColor3f(0.8,0.1,0.8);
      glBegin(GL_QUADS);
        glVertex3f(initmX, 0.5, initmZ);
        glVertex3f(initmX, 0.5, mZ);
        glVertex3f(mX, 0.5, mZ);
        glVertex3f(mX, 0.5, initmZ);
      glEnd();
   }

   /* Draw All Map Portals Destinations */
   portalList->drawAll();
   
   glColor3f(1.0,1.0,1.0);
   glEnable(GL_LIGHTING);
}

/******************************************************
 *                       addPortal                    *
 ******************************************************/
void Portal::addPortal(int qx, int qz, string where)
{
   Square* s = actualMap->relativeSquare(qx,qz);
   if(s)
   {
      /* Set Portal square */
      if(initmX > mX)
      {
         GLfloat tmp = initmX;
         initmX = mX;
         mX = tmp;
      }    
      if(initmZ > mZ)
      {
         GLfloat tmp = initmZ;
         initmZ = mZ;
         mZ = tmp;
      } 
      s->mapConection.x1 = initmX;
      s->mapConection.x2 = mX;
      s->mapConection.z1 = initmZ;
      s->mapConection.z2 = mZ;
      s->mapConection.mapName = where; 
      s->mapConection.active = true;
      portalList->addArea(initmX, initmZ, mX, mZ, where);

      int minqx, minqz, maxqx, maxqz;
      minqx = (int)(initmX) / actualMap->squareSize();
      minqz = (int)(initmZ) / actualMap->squareSize();
      maxqx = (int)(mX) / actualMap->squareSize();
      maxqz = (int)(mZ) / actualMap->squareSize(); 
      int X1, Z1;
      Square* q;
      for(X1 = minqx; X1<=maxqx; X1++)
      {
         for(Z1 = minqz; Z1 <=maxqz; Z1++) 
         {
            q = actualMap->relativeSquare(X1,Z1);
            if((q) && (q != s))
            {
               q->mapConection.x1 = initmX;
               q->mapConection.x2 = mX;
               q->mapConection.z1 = initmZ;
               q->mapConection.z2 = mZ;
               q->mapConection.mapName = where; 
               q->mapConection.active = true;
            }
         }
      }
   }
}

/******************************************************
 *                      doAddPortal                   *
 ******************************************************/
void Portal::doAddPortal()
{
   int qx = (int) (mX / actualMap->squareSize());
   int qz = (int) (mZ / actualMap->squareSize());

   if( (state == PORTAL_STATE_OTHER) && (mB & SDL_BUTTON(1)) )
   {
      /* Init Portal Select Area */
      state = PORTAL_STATE_ADD_INIT;
      initmX = mX;
      initmZ = mZ;
   }
   else if( (state == PORTAL_STATE_ADD_INIT) && !(mB & SDL_BUTTON(1)) )
   {
      /* Add Portal */
      state = PORTAL_STATE_OTHER;
      addPortal(qx,qz, "mapFileName");
   }
}

/******************************************************
 *                       doTagPortal                  *
 ******************************************************/
void Portal::doTagPortal(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   Area* ar = portalList->getArea(mX, mZ);

   if( (ar != NULL) && (mB & SDL_BUTTON(1)) )
   {
      initmX = ar->x1;
      mX = ar->x2;
      initmZ = ar->y1;
      mZ = ar->y2;
      
      int qx = (int) (mX / actualMap->squareSize());
      int qz = (int) (mZ / actualMap->squareSize());
      
      addPortal(qx, qz, getStringFromUser("Input Destiny", ar->whereToGo,
                proj, modl, viewPort));
   }
}

/******************************************************
 *                 doDoorLockUnlock                   *
 ******************************************************/
void Portal::doDoorLockUnlock(GLfloat mouseX, GLfloat mouseY,
      GLfloat mouseZ, Uint8 mButton)
{
   boundingBox colBox;
   boundingBox mouseBox;
   
   /* Create a bounding box for the mouse position */
   mouseBox.setMin(mouseX-4, mouseY-4.0, mouseZ-4);
   mouseBox.setMax(mouseX+4, mouseY+4.0, mouseZ+4);

   if(mButton & SDL_BUTTON(1))
   {
      /* Doors Verification */
      door* porta = actualMap->getFirstDoor();
      int d;
      for(d=0; (d < actualMap->getTotalDoors()); d++)
      {
         colBox = porta->obj->scNode->getBoundingBox();
         if(mouseBox.intercepts(colBox))
         {
            /* Select the door */
            curLockDoor = porta;
            /* Update or open the window with this door values */
            openLockWindow();
            return;
         }
         porta = (door*)porta->getNext();
      }
   }
}

/******************************************************
 *                  openLockWindow                    *
 ******************************************************/
void Portal::openLockWindow()
{
   /* Only open window if it's not opened */
   if(!lockWindow)
   {
      /* Create window */
      lockWindow = gui->insertWindow(290,212,530,292,"Door Lock/Unlock");

      /* Radio Box for lock */
      lockCxSel = lockWindow->getObjectsList()->insertCxSel(6, 20, false);
      lockTxt = lockWindow->getObjectsList()->insertTextBox(18,17,129,31,0,
            "Door Unlocked");

      /* Dialog File To Use */
      lockWindow->getObjectsList()->insertTextBox(6,38,67,52,0,"Dialog File:");
      lockDialogBar = lockWindow->getObjectsList()->insertTextBar(68,38,234,52,
          "dialogs/objects/doors.dlg", 0);

      /* Lock Levels */
      lockWindow->getObjectsList()->insertTextBox(6,54,76,68,0,
            "PickLock Lvl:");
      lockIAmNotAFool = lockWindow->getObjectsList()->insertTextBar(
            77,54,107,68,"20", 0);
      lockWindow->getObjectsList()->insertTextBox(133,54,203,68,0,
            "Burglary Lvl:");
      lockFortitude = lockWindow->getObjectsList()->insertTextBar(
            204,54,234,68,"20", 0);


      /* Finally, open */
      lockWindow->setExternPointer(&lockWindow);
      gui->openWindow(lockWindow);
   }

   /* And set values */
   setLockWindowValues();
}

/******************************************************
 *              setLockWindowValues                   *
 ******************************************************/
void Portal::setLockWindowValues()
{
   char buf[32];
   if( (lockWindow) && (curLockDoor) )
   {
      /* Set Values */
      lockCxSel->setSelection(curLockDoor->obj->getState()==DOOR_STATE_LOCKED);
      lockDialogBar->setText(curLockDoor->obj->getConversationFile());
      lockDialogBar->setAvailable(lockCxSel->isSelected());
      sprintf(buf, "%d", curLockDoor->obj->curBonusAndSaves.fortitude);
      lockFortitude->setText(buf);
      lockFortitude->setAvailable(lockCxSel->isSelected());
      sprintf(buf, "%d", curLockDoor->obj->curBonusAndSaves.fortitude);
      sprintf(buf, "%d", curLockDoor->obj->curBonusAndSaves.iAmNotAFool);
      lockIAmNotAFool->setText(buf);
      lockIAmNotAFool->setAvailable(lockCxSel->isSelected());

      /* Set lock/unlock text */
      if(lockCxSel->isSelected())
      {
         lockTxt->setText("Door Locked");
      }
      else
      {
         lockTxt->setText("Door Unlocked");
      }

      /* Force Window redraw */
      lockWindow->draw(0,0);
   }
}


/******************************************************
 *                     eventGot                       *
 ******************************************************/
bool Portal::eventGot(int eventInfo, Farso::GuiObject* obj)
{
   if(eventInfo == Farso::EVENT_MODIFIED_CX_SEL)
   {
      if(obj == lockCxSel)
      {
         if(lockCxSel->isSelected())
         {
            /* Set as locked */
            curLockDoor->obj->setState(DOOR_STATE_LOCKED);
            /* If needed, set default dialog file */
            if(curLockDoor->obj->getConversationFile().empty())
            {
               curLockDoor->obj->setConversationFile(
                     "dialogs/objects/doors.dlg");
            }
         }
         else
         {
            /* Set As Unlocked */
            curLockDoor->obj->setState(DOOR_STATE_UNLOCKED);
            /* Clear Dialog File */
            curLockDoor->obj->setConversationFile("");
         }
         setLockWindowValues();
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_WROTE_TEXT_BAR)
   {
      if(obj == lockDialogBar)
      {
         curLockDoor->obj->setConversationFile(lockDialogBar->getText());
         setLockWindowValues();
         return(true);
      }
      else if(obj == lockFortitude)
      {
         sscanf(lockFortitude->getText().c_str(), "%d", 
               &curLockDoor->obj->curBonusAndSaves.fortitude);
         setLockWindowValues();
         return(true);
      }
      else if(obj == lockIAmNotAFool)
      {
         sscanf(lockIAmNotAFool->getText().c_str(), "%d", 
               &curLockDoor->obj->curBonusAndSaves.iAmNotAFool);
         setLockWindowValues();
         return(true);
      }
   }

   return(false);
}


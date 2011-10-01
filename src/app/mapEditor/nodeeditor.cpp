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

#include "nodeeditor.h"


/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
nodeEditor::nodeEditor(guiInterface* g)
{
   gui = g;
   nodeWindow = NULL;
   curMap = NULL;

   clear();
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
nodeEditor::~nodeEditor()
{
}

/***********************************************************************
 *                               clear                                 *
 ***********************************************************************/
void nodeEditor::clear()
{
   if(nodeWindow)
   {
      gui->closeWindow(nodeWindow);
   }
   nodeTab = NULL;
   deltaValue = 1.0f;
   curNode = NULL;
   curNodeParent = NULL;
   curMap = NULL;
   npcs = NULL;
}

/***********************************************************************
 *                              setMap                                 *
 ***********************************************************************/
void nodeEditor::setMap(Map* m, characterList* npcList)
{
   clear();
   curMap = m;
   npcs = npcList;
}

/***********************************************************************
 *                             verifyAction                            *
 ***********************************************************************/
void nodeEditor::verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
            GLfloat mouseZ, Uint8 mButton, int mouseXw, int mouseYw,
            GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   scene dntScene;
   sceneNode* scNode;

   if(mButton & SDL_BUTTON(1))
   {
      /* Create a bounding box for the mouse position */
      boundingBox mouseBox;
      mouseBox.setMin(mouseX-4, mouseY-4.0, mouseZ-4);
      mouseBox.setMax(mouseX+4, mouseY+4.0, mouseZ+4);

      scNode = dntScene.getSceneNode(mouseBox);
      if(scNode)
      {
         selectNode(scNode);
      }
   }
}

/***********************************************************************
 *                            deleteCurNode                            *
 ***********************************************************************/
void nodeEditor::deleteCurNode()
{
   if(curNode)
   {
      if(nodeParentType == NODE_PARENT_OBJECT)
      {
         object* o = (object*)curNodeParent;
         delete(o);
      }
      else if(nodeParentType == NODE_PARENT_CHARACTER)
      {
         npcs->removeCharacter((character*)curNodeParent);
      }
   }

   curNode = NULL;
}

/***********************************************************************
 *                                flush                                *
 ***********************************************************************/
void nodeEditor::flush()
{
   if( (curNode) && (curMap) )
   {
      if(nodeParentType == NODE_PARENT_OBJECT)
      {
         /* Must put back the object to the map, without creating another
          * sceneNode for it */
         curMap->insertObject(curNode->getPosX(), curNode->getPosY(), 
                              curNode->getPosZ(), curNode->getAngleX(),
                              curNode->getAngleY(), curNode->getAngleZ(),
                              (object*)curNodeParent, true, false);
      }
      curNodeParent = NULL;
      curNode = NULL;
   }

   if(nodeWindow)
   {
      gui->closeWindow(nodeWindow);
   }
}

/***********************************************************************
 *                              selectNode                             *
 ***********************************************************************/
void nodeEditor::selectNode(sceneNode* scNode)
{
   /* Flush changes not applied, if needed */
   flush();

   /* Set new curNode */
   curNode = scNode;

   /* Verify its type */
   curNodeParent = curMap->getDoor(curNode);
   if(curNodeParent)
   {
      //nodeParentType = NODE_PARENT_DOOR;
      
      /* FIXME: not editing doors for now... */
      curNode = NULL;
      return;
   }
   curNodeParent = curMap->getObject(curNode);
   if(curNodeParent)
   {
      nodeParentType = NODE_PARENT_OBJECT;
      /* Must remove from map to edit it (otherwise, the squares related
       * to it will be mistaken!) */
      curMap->removeObject((object*)curNodeParent);
      openWindow();
      return;
   }
   curNodeParent = npcs->getCharacter(curNode);
   if(curNodeParent)
   {
      nodeParentType = NODE_PARENT_CHARACTER;
      openWindow();
      return;
   }
   
   /* Couldn't determine type, no edit! */
   //cerr << "Couldn't determine sceneNode type! Unselecting... " << endl;
   curNode = NULL;
}

/***********************************************************************
 *                               eventGot                              *
 ***********************************************************************/
bool nodeEditor::eventGot(int eventInfo, guiObject* obj)
{
   float delta=deltaValue;

   if( (!nodeWindow) || (!curNode))
   {
      /* If no window, no events here! */
      return(false);
   }

   /* Define Delta Value */

   if(eventInfo == FARSO_EVENT_ON_PRESS_TAB_BUTTON)
   {
      /* Angles */
      if(obj == clearRot)
      {
         curNode->setAngle(0.0f, 0.0f, 0.0f);
         return(true);
      }
      else if(obj == rotX[0])
      {
         curNode->setAngle(curNode->getAngleX()-delta, 
                           curNode->getAngleY(),
                           curNode->getAngleZ());
         return(true);
      }
      else if(obj == rotX[1])
      {
         curNode->setAngle(curNode->getAngleX()+delta, 
                           curNode->getAngleY(),
                           curNode->getAngleZ());
         return(true);
      }
      else if(obj == rotY[0])
      {
         curNode->setAngle(curNode->getAngleX(), 
                           curNode->getAngleY()-delta,
                           curNode->getAngleZ());
         return(true);
      }
      else if(obj == rotY[1])
      {
         curNode->setAngle(curNode->getAngleX(), 
                           curNode->getAngleY()+delta,
                           curNode->getAngleZ());
         return(true);
      }
      else if(obj == rotZ[0])
      {
         curNode->setAngle(curNode->getAngleX(), 
                           curNode->getAngleY(),
                           curNode->getAngleZ()-delta);
         return(true);
      }
      else if(obj == rotZ[1])
      {
         curNode->setAngle(curNode->getAngleX(), 
                           curNode->getAngleY(),
                           curNode->getAngleZ()+delta);
         return(true);
      }
      /* Position */
      else if(obj == moveX[0])
      {
         if(!gridMode->isSelected())
         {
            curNode->setPosition(curNode->getPosX()-delta, 
                                 curNode->getPosY(),
                                 curNode->getPosZ());
            return(true);
         }
      }
      else if(obj == moveX[1])
      {
         if(!gridMode->isSelected())
         {
            curNode->setPosition(curNode->getPosX()+delta, 
                                 curNode->getPosY(),
                                 curNode->getPosZ());
            return(true);
         }
      }
      else if(obj == moveY[0])
      {
         curNode->setPosition(curNode->getPosX(), 
                              curNode->getPosY()+delta,
                              curNode->getPosZ());
         return(true);
      }
      else if(obj == moveY[1])
      {
         curNode->setPosition(curNode->getPosX(), 
                              curNode->getPosY()-delta,
                              curNode->getPosZ());
         return(true);
      }
      else if(obj == moveZ[0])
      {
         if(!gridMode->isSelected())
         {
            curNode->setPosition(curNode->getPosX(), 
                                 curNode->getPosY(),
                                 curNode->getPosZ()-delta);
            return(true);
         }
      }
      else if(obj == moveZ[1])
      {
         if(!gridMode->isSelected())
         {
            curNode->setPosition(curNode->getPosX(), 
                                 curNode->getPosY(),
                                 curNode->getPosZ()+delta);
            return(true);
         }
      }
   }
   else if(eventInfo == FARSO_EVENT_PRESSED_TAB_BUTTON)
   {
      if(obj == moveX[0])
      {
         if(gridMode->isSelected())
         {
            int pm10 = round(curNode->getPosX());
            pm10 = (pm10 - (pm10 % 10)) - delta*10;
            curNode->setPosition(pm10, 
                                 curNode->getPosY(),
                                 curNode->getPosZ());
            return(true);
         }
      }
      else if(obj == moveX[1])
      {
         if(gridMode->isSelected())
         {
            int pm10 = round(curNode->getPosX());
            pm10 = (pm10 - (pm10 % 10)) + delta*10;
            curNode->setPosition(pm10, 
                                 curNode->getPosY(),
                                 curNode->getPosZ());
            return(true);
         }
      }
      else if(obj == moveZ[0])
      {
         if(gridMode->isSelected())
         {
            int pm10 = round(curNode->getPosZ());
            pm10 = (pm10 - (pm10 % 10)) - delta*10;
            curNode->setPosition(curNode->getPosX(), 
                                 curNode->getPosY(),
                                 pm10);
            return(true);
         }
      }
      else if(obj == moveZ[1])
      {
         if(gridMode->isSelected())
         {
            int pm10 = round(curNode->getPosZ());
            pm10 = (pm10 - (pm10 % 10)) + delta*10;
            curNode->setPosition(curNode->getPosX(), 
                                 curNode->getPosY(),
                                 pm10);
            return(true);
         }
      
      }
      else if(obj == deleteNode)
      {
         deleteCurNode();
         gui->closeWindow(nodeWindow);
      }
   }
   else if(eventInfo == FARSO_EVENT_WROTE_TEXT_BAR)
   {
      if(obj == deltaText)
      {
         if(!deltaText->getText().empty())
         {
            sscanf(deltaText->getText().c_str(),"%f",&deltaValue);
         }
         else
         {
            deltaValue = 0.0f;
         }
      }
   }

   return(false);
}

/***********************************************************************
 *                              openWindow                             *
 ***********************************************************************/
void nodeEditor::openWindow()
{
   dirs dir;
   if(!nodeWindow)
   {
      /* Create the window */
      nodeWindow = gui->insertWindow(290,212,490,368,"SceneNode");
      nodeTab = nodeWindow->getObjectsList()->insertTabButton(7,17,0,0,
            dir.getRealFile("mapEditor/nodeeditor.png").c_str());

      /* Insert the tabbuttons */
      rotX[0] = nodeTab->insertButton(114,0,143,29);
      rotX[0]->setMouseHint("Dec X Rot");
      rotX[1] = nodeTab->insertButton(155,0,182,29);
      rotX[1]->setMouseHint("Inc X Rot");
      rotY[0] = nodeTab->insertButton(114,37,143,66);
      rotY[0]->setMouseHint("Dec Y Rot");
      rotY[1] = nodeTab->insertButton(155,37,182,66);
      rotY[1]->setMouseHint("Inc Y Rot");
      rotZ[0] = nodeTab->insertButton(114,70,143,100);
      rotZ[0]->setMouseHint("Dec Z Rot");
      rotZ[1] = nodeTab->insertButton(155,70,182,100);
      rotZ[1]->setMouseHint("Inc Z Rot");

      moveX[0] = nodeTab->insertButton(0,41,18,57);
      moveX[0]->setMouseHint("Dec X Pos");
      moveX[1] = nodeTab->insertButton(37,41,54,57);
      moveX[0]->setMouseHint("Inc X Pos");

      moveZ[0] = nodeTab->insertButton(19,22,34,38);
      moveZ[0]->setMouseHint("Dec Z Pos");
      moveZ[1] = nodeTab->insertButton(19,59,34,76);
      moveZ[1]->setMouseHint("Inc Z Pos");

      moveY[0] = nodeTab->insertButton(62,59,79,98);
      moveY[0]->setMouseHint("Up");
      moveY[1] = nodeTab->insertButton(86,59,103,98);
      moveY[1]->setMouseHint("Down");
    
      deleteNode = nodeTab->insertButton(37,65,59,98);
      deleteNode->setMouseHint("Delete");

      clearRot = nodeTab->insertButton(59,8,114,47);
      clearRot->setMouseHint("Clear Rotations");

      /* Cx Sel for grid mode on move X/Z */
      gridMode = nodeWindow->getObjectsList()->insertCxSel(12,134,false);
      nodeWindow->getObjectsList()->insertTextBox(24,130,79,144,0,
            "Grid Mode");

      /* Delta Value edit */
      nodeWindow->getObjectsList()->insertTextBox(80,130,120,144,0,"Delta:");
      deltaText = nodeWindow->getObjectsList()->insertTextBar(122,131,162,145,
            "1.0",0);

      /* Finally, open */
      nodeWindow->setExternPointer(&nodeWindow);
      gui->openWindow(nodeWindow);
   }
}


/***********************************************************************
 *                             drawTemporary                           *
 ***********************************************************************/
void nodeEditor::drawTemporary()
{
   boundingBox b;
   float delta=2;
   int med;

   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);
   if(curNode)
   {
      b = curNode->getBoundingBox();
      glColor4f(0.0f, 0.5f, 0.8f, 1.0f);

      /* Calculate delta */
      med = ( (b.x2-b.x1) + (b.y2-b.y1) + (b.z2-b.z1) ) / 3;
      if(med > 10)
      {
         delta = 2.0f;
      }
      else if(med > 5)
      {
         delta = 1.0f;
      }
      else
      {
         delta = 0.5f;
      }

      /* Render the contorn grid */
      glBegin(GL_QUADS);

        glVertex3f(b.x1-delta, b.y1+1, b.z1);
        glVertex3f(b.x1-delta, b.y1+1, b.z2);
        glVertex3f(b.x1+delta, b.y1+1, b.z2);
        glVertex3f(b.x1+delta, b.y1+1, b.z1);
      
        glVertex3f(b.x2-delta, b.y1+1, b.z1);
        glVertex3f(b.x2-delta, b.y1+1, b.z2);
        glVertex3f(b.x2+delta, b.y1+1, b.z2);
        glVertex3f(b.x2+delta, b.y1+1, b.z1);

        glVertex3f(b.x1, b.y1+1, b.z1-delta);
        glVertex3f(b.x2, b.y1+1, b.z1-delta);
        glVertex3f(b.x2, b.y1+1, b.z1+delta);
        glVertex3f(b.x1, b.y1+1, b.z1+delta);

        glVertex3f(b.x1, b.y1+1, b.z2-delta);
        glVertex3f(b.x2, b.y1+1, b.z2-delta);
        glVertex3f(b.x2, b.y1+1, b.z2+delta);
        glVertex3f(b.x1, b.y1+1, b.z2+delta);

      glEnd();
   }


   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glEnable(GL_FOG);
   glEnable(GL_LIGHTING);
}


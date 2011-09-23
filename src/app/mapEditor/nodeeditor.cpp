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
   nodeTab = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
nodeEditor::~nodeEditor()
{
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

/***********************************************************************
 *                              selectNode                             *
 ***********************************************************************/
void nodeEditor::selectNode(sceneNode* scNode)
{
   curNode = scNode;
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
      rotZ[1] = nodeTab->insertButton(155,70,143,100);
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
     
      clearRot = nodeTab->insertButton(59,8,114,47);
      clearRot->setMouseHint("Clear Rotations");

      /* Finally, open */
      nodeWindow->setExternPointer(&nodeWindow);
      gui->openWindow(nodeWindow);
   }
}



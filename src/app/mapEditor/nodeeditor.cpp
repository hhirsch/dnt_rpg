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
nodeEditor::nodeEditor()
{
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
      curNode = scNode;
   }
}



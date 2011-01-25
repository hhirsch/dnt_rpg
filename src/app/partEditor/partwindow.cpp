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

#include "partwindow.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partWindow::partWindow(guiInterface* interf)
{
   curWindow = NULL;
   part = NULL;
   gui = interf;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
partWindow::~partWindow()
{
   closeWindow();
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool partWindow::isOpen()
{
   return(curWindow != NULL);
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void partWindow::openWindow()
{
   int posX=0, 
       posY = 64;
   dntFont fnt;

   if(isOpen())
   {
      /* Clsoe to reopen */
      posX = curWindow->getX1();
      posY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(posX, posY, posX+193, posY+212,
                                "Particle"); 
   curWindow->setExternPointer(&curWindow);

   /* maxParticles */
   curWindow->getObjectsList()->insertTextBox(10,20,80,37,0,"MaxParticles:");
   maxParticles = curWindow->getObjectsList()->insertTextBar(81, 20, 141, 37, 
         "", 0);

   /* maxLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,43,80,60,0,"MaxLifeTime:");
   maxLifeTime = curWindow->getObjectsList()->insertTextBar(81, 43, 141, 60, 
         "", 0);

   /* particleLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,66,80,83,0,
         "ParticleLifeTime:");
   maxLifeTime = curWindow->getObjectsList()->insertTextBar(81, 66, 141, 83, 
         "", 0);

   /* DrawMode things */
   curWindow->getObjectsList()->insertTextBox(10,89,80,106,0,"DrawMode:");
   previousDrawMode = curWindow->getObjectsList()->insertButton(81,99,91,106, 
         fnt.createUnicode(0x25C4),0);
   previousDrawMode->defineFont(DNT_FONT_ARIAL, 9);
   drawMode = curWindow->getObjectsList()->insertTextBox(92, 99, 172, 106, 
         1, "");
   nextDrawMode = curWindow->getObjectsList()->insertButton(173, 99, 183, 106, 
         fnt.createUnicode(0x25BA),0);
   nextDrawMode->defineFont(DNT_FONT_ARIAL, 9);

   /* renderMode things */
   curWindow->getObjectsList()->insertTextBox(10,112,80,129,0,"RenderMode:");
   previousRenderMode = curWindow->getObjectsList()->insertButton(81,112,
         91,129, fnt.createUnicode(0x25C4),0);
   previousRenderMode->defineFont(DNT_FONT_ARIAL, 9);
   renderMode = curWindow->getObjectsList()->insertTextBox(92,112,172,129, 
         1, "");
   nextRenderMode = curWindow->getObjectsList()->insertButton(173,112,183,129, 
         fnt.createUnicode(0x25BA),0);
   nextRenderMode->defineFont(DNT_FONT_ARIAL, 9);

   /* pointSize */
   curWindow->getObjectsList()->insertTextBox(10,66,80,83,0,"PointSize:");
   pointSize = curWindow->getObjectsList()->insertTextBar(81, 66, 141, 83, 
         "", 0);


   /* Finally, open the window */
   gui->openWindow(curWindow);
#if 0
      cxSel* windAffect;
      cxSel* floorCollision;

      /* Elements */
      element;
      menu* elementsMenu;
#endif
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void partWindow::closeWindow()
{
   if(isOpen())
   {
      gui->closeWindow(curWindow);
   }
}




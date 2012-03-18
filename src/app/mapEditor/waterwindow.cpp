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

#include "waterwindow.h"
#include "../../etc/dirs.h"

#include <iostream>
using namespace std;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
waterWindow::waterWindow(Farso::GuiInterface* actualGui)
{
   gui = actualGui;
   activeWater = NULL;
   activePlane = NULL;
   intWindow = NULL;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
waterWindow::~waterWindow()
{
}

/***********************************************************************
 *                              setWater                               *
 ***********************************************************************/
void waterWindow::setWater(particleSystem* water)
{
   activeWater = water;
   if(!intWindow)
   {
      openWindow();
   }
}

/***********************************************************************
 *                              setPlane                               *
 ***********************************************************************/
void waterWindow::setPlane(interPlane* pl)
{
   activePlane = pl;
   if(!intWindow)
   {
      openWindow();
   }
}

/***********************************************************************
 *                              setPlane                               *
 ***********************************************************************/
void waterWindow::setPartSystem(partController* pS) 
{
   pSystem = pS;
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void waterWindow::openWindow()
{
   dirs dir;
   Farso::Font fnt;
   intWindow = gui->insertWindow(403,599-247,653,599-37,"Particle's Planes");

   /* Waterfall Navigators */
   intWindow->getObjectsList()->insertTextBox(15,17,77,36,0,"Waterfall");
   waterTab = intWindow->getObjectsList()->insertTabButton(6,30,0,0,
                                 dir.getRealFile("mapEditor/nav2.png").c_str());
   waterMoveTab = intWindow->getObjectsList()->insertTabButton(67,30,0,0,
                           dir.getRealFile("mapEditor/water_move.png").c_str());
   buttonPreviousWater = waterTab->insertButton(0,0,19,19);
   buttonDestroyWater = waterTab->insertButton(20,0,39,19);
   buttonNextWater = waterTab->insertButton(40,0,59,19);
   buttonMoveWaterUp = waterMoveTab->insertButton(0,0,19,9);
   buttonMoveWaterDown = waterMoveTab->insertButton(0,10,19,19);
   intWindow->getObjectsList()->insertTextBox(4,16,90,52,1,"");

   /* Plane Navigators */
   intWindow->getObjectsList()->insertTextBox(112,17,152,36,0,"Planes");
   planesTab = intWindow->getObjectsList()->insertTabButton(94,30,0,0,
                            dir.getRealFile("mapEditor/plane_nav.png").c_str());
   buttonAddPlane = planesTab->insertButton(0,0,19,19);
   buttonPreviousPlane = planesTab->insertButton(20,0,39,19);
   buttonDestroyPlane = planesTab->insertButton(40,0,59,19);
   buttonNextPlane = planesTab->insertButton(60,0,79,19);
   intWindow->getObjectsList()->insertTextBox(91,16,177,52,1,"");

   /* Plane Inclination */
   intWindow->getObjectsList()->insertTextBox(181,17,245,36,0,"Plane Inc");
   incTypeTab = intWindow->getObjectsList()->insertTabButton(181,30,0,0,
                    dir.getRealFile("mapEditor/plane_inclination.png").c_str());
   buttonNoInclination = incTypeTab->insertButton(0,0,19,19);
   buttonInclinationX = incTypeTab->insertButton(20,0,39,19);
   buttonInclinationZ = incTypeTab->insertButton(40,0,59,19);
   intWindow->getObjectsList()->insertTextBox(178,16,246,52,1,"");

   /* Plane Move X_Z */
   intWindow->getObjectsList()->insertTextBox(5,55,45,68,0,"Plane  Move");
   movXZTab = intWindow->getObjectsList()->insertTabButton(5,78,0,0,
                           dir.getRealFile("mapEditor/plane_move.png").c_str());
   buttonMoveZMore = movXZTab->insertButton(14,0,26,14);
   buttonMoveZLess = movXZTab->insertButton(14,24,26,39);
   buttonMoveXLess = movXZTab->insertButton(2,12,15,26);
   buttonMoveXMore = movXZTab->insertButton(25,12,38,26);
   intWindow->getObjectsList()->insertTextBox(3,53,48,120,1,"");

   /* Plane Move Y */
   intWindow->getObjectsList()->insertTextBox(10,123,65,136,0,"Plane y");
   movYTab = intWindow->getObjectsList()->insertTabButton(6,136,0,0,
                         dir.getRealFile("mapEditor/plane_move_y.png").c_str());
   buttonMoveYMore = movYTab->insertButton(5,1,15,15);
   buttonMoveYLess = movYTab->insertButton(5,24,15,38);
   buttonMoveY1More = movYTab->insertButton(20,1,30,15);
   buttonMoveY1Less = movYTab->insertButton(20,24,30,38);
   buttonMoveY2More = movYTab->insertButton(47,1,57,15);
   buttonMoveY2Less = movYTab->insertButton(47,24,57,38);
   intWindow->getObjectsList()->insertTextBox(3,121,69,180,1,"");

   /* Plane Scale */
   intWindow->getObjectsList()->insertTextBox(75,123,130,136,0,"P Scale");
   scaleTab = intWindow->getObjectsList()->insertTabButton(73,136,0,0,
                          dir.getRealFile("mapEditor/plane_scale.png").c_str());
   buttonScaleXMore = scaleTab->insertButton(0,0,19,19);
   buttonScaleXLess = scaleTab->insertButton(0,20,19,39);
   buttonScaleYMore = scaleTab->insertButton(20,0,39,19);
   buttonScaleYLess = scaleTab->insertButton(20,20,39,39);
   buttonScaleZMore = scaleTab->insertButton(40,0,59,19);
   buttonScaleZLess = scaleTab->insertButton(40,20,59,39);
   intWindow->getObjectsList()->insertTextBox(70,121,138,180,1,"");

   /* Waterflow Direction */
   intWindow->getObjectsList()->insertTextBox(110,55,245,68,0,
                                              "Plane Waterflow");
   intWindow->getObjectsList()->insertTextBox(50,73,100,88,0,"deltaX:");
   dxWaterflowLess = intWindow->getObjectsList()->insertButton(101,71,111,88,
                                                   fnt.createUnicode(0x25C4),0);
   dxWaterflowText = intWindow->getObjectsList()->insertTextBox(112,71,162,88,
                                                                 1,"1.0");
   dxWaterflowMore = intWindow->getObjectsList()->insertButton(163,71,173,88,
                                                   fnt.createUnicode(0x25BA),0);
   intWindow->getObjectsList()->insertTextBox(50,93,100,108,0,"deltaZ:");
   dzWaterflowLess = intWindow->getObjectsList()->insertButton(101,91,111,108,
                                                   fnt.createUnicode(0x25C4),0);
   dzWaterflowText = intWindow->getObjectsList()->insertTextBox(112,91,162,108,
                                                                 1,"1.0");
   dzWaterflowMore = intWindow->getObjectsList()->insertButton(163,91,173,108,
                                                   fnt.createUnicode(0x25BA),0);
   intWindow->getObjectsList()->insertTextBox(49,53,246,120,1,"");

   /* Unused Space */
   intWindow->getObjectsList()->insertTextBox(139,121,246,180,1,"");

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   gui->openWindow(intWindow);
}

/***********************************************************************
 *                              eventGot                               *
 ***********************************************************************/
bool waterWindow::eventGot(int type, Farso::GuiObject* object)
{
   GLfloat wX = 0, wY = 0, wZ = 0;

   if(!intWindow)
   {
      return(false);
   }

   if(activeWater)
   {
      activeWater->getPosition(wX, wY, wZ);
   }

   if(type == Farso::EVENT_ON_PRESS_TAB_BUTTON)
   {
      /* Waterfall Up/Down */
      if(object == (Farso::GuiObject*)buttonMoveWaterUp)
      {
         if(activeWater)
         {
            wY += 0.5;
            activeWater->definePosition(wX, wY, wZ);
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveWaterDown)
      {
         if(activeWater)
         {
            wY -= 0.5;
            activeWater->definePosition(wX, wY, wZ);
         }
         return(true);
      }

      /* Plane Inclination Type */
      else if(object == (Farso::GuiObject*)buttonNoInclination)
      {
         if(activePlane)
         {
            activePlane->inclination = PLANE_NO_INCLINATION;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonInclinationX)
      {
         if(activePlane)
         {
            activePlane->inclination = PLANE_INCLINATION_X;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonInclinationZ)
      {
         if(activePlane)
         {
            activePlane->inclination = PLANE_INCLINATION_Z;
         }
         return(true);
      }

      /* Plane Move X/Z */
      else if(object == (Farso::GuiObject*)buttonMoveZMore)
      {
         if(activePlane)
         {
            activePlane->z1 += 0.1;
            activePlane->z2 += 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveZLess)
      {
         if(activePlane)
         {
            activePlane->z1 -= 0.1;
            activePlane->z2 -= 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveXMore)
      {
         if(activePlane)
         {
            activePlane->x1 += 0.1;
            activePlane->x2 += 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveXLess)
      {
         if(activePlane)
         {
            activePlane->x1 -= 0.1;
            activePlane->x2 -= 0.1;
         }
         return(true);
      }

      /* Move Plane Y */
      else if(object == (Farso::GuiObject*)buttonMoveYMore)
      {
         if(activePlane)
         {
            activePlane->y1 += 0.1;
            activePlane->y2 += 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveYLess)
      {
         if(activePlane)
         {
            activePlane->y1 -= 0.1;
            activePlane->y2 -= 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveY1More)
      {
         if(activePlane)
         {
            activePlane->y1 += 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveY1Less)
      {
         if(activePlane)
         {
            activePlane->y1 -= 0.1;
         }
         return(true);
      }

      else if(object == (Farso::GuiObject*)buttonMoveY2More)
      {
         if(activePlane)
         {
            activePlane->y2 += 0.1;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonMoveY2Less)
      {
         if(activePlane)
         {
            activePlane->y2 -= 0.1;
         }
         return(true);
      }

      /* Scale Plane */
      else if(object == (Farso::GuiObject*)buttonScaleXMore)
      {
         if(activePlane)
         {
            activePlane->x1 -= 0.05;
            activePlane->x2 += 0.05;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonScaleXLess)
      {
         if(activePlane)
         {
            activePlane->x1 += 0.05;
            activePlane->x2 -= 0.05;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonScaleYMore)
      {
         if(activePlane)
         {
            activePlane->y1 -= 0.05;
            activePlane->y2 += 0.05;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonScaleYLess)
      {
         if(activePlane)
         {
            activePlane->y1 += 0.05;
            activePlane->y2 -= 0.05;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonScaleZMore)
      {
         if(activePlane)
         {
            activePlane->z1 -= 0.05;
            activePlane->z2 += 0.05;
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonScaleZLess)
      {
         if(activePlane)
         {
            activePlane->z1 += 0.05;
            activePlane->z2 -= 0.05;
         }
         return(true);
      }

   }

   else if(type == Farso::EVENT_PRESSED_TAB_BUTTON)
   {
      /* Waterfall Controller */
      if(object == (Farso::GuiObject*)buttonNextWater)
      {
         if(activeWater)
         {
            activeWater = (particleSystem*)activeWater->getNext();
            activePlane = activeWater->getLastPlane();
            defineValues();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonPreviousWater)
      {
         if(activeWater)
         {
            activeWater = (particleSystem*)activeWater->getPrevious();
            activePlane = activeWater->getLastPlane();
            defineValues();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonDestroyWater)
      {
         if((activeWater) && (pSystem))
         {
            particleSystem* tmpWater = NULL;
            if(activeWater != (particleSystem*)activeWater->getPrevious())
            {
               tmpWater = (particleSystem*)activeWater->getPrevious();
               activePlane = tmpWater->getLastPlane();
            }
            else
            {
               activePlane = NULL;
            }
            pSystem->removeParticle(activeWater);
            activeWater = tmpWater;
            defineValues();
         }
         return(true);
      }
      /* Plane Controller */
      else if(object == (Farso::GuiObject*)buttonAddPlane)
      {
         if(activeWater)
         {  
            activePlane = activeWater->addPlane(wX-2, wY-1, wZ-2, 
                                                wX+2, wY-1, wZ+2,
                                                1, 0, PLANE_NO_INCLINATION);
            defineValues();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonNextPlane)
      {
         if(activePlane)
         {
            activePlane = (interPlane*)activePlane->getNext();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonPreviousPlane)
      {
         if(activePlane)
         {
            activePlane = (interPlane*)activePlane->getPrevious();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)buttonDestroyPlane)
      {
         if((activeWater) && (activePlane))
         {
            interPlane* tmpPlane = NULL;
            if(activePlane != activePlane->getPrevious())
            {
               tmpPlane = (interPlane*)activePlane->getPrevious();
            }
            activeWater->removePlane(activePlane);
            activePlane = tmpPlane;
            defineValues();
         }
         return(true);
      }
   }
   else if(type == Farso::EVENT_ON_PRESS_BUTTON)
   {
      if(object == (Farso::GuiObject*)dxWaterflowMore)
      {
         activePlane->dX += 0.1;
         defineValues();
         return(true);
      }
      else if(object == (Farso::GuiObject*)dxWaterflowLess)
      {
         activePlane->dX -= 0.1;
         defineValues();
         return(true);
      }
      else if(object == (Farso::GuiObject*)dzWaterflowMore)
      {
         activePlane->dZ += 0.1;
         defineValues();
         return(true);
      }
      else if(object == (Farso::GuiObject*)dzWaterflowLess)
      {
         activePlane->dZ -= 0.1;
         defineValues();
         return(true);
      }
   }
   return(false);
}

/***********************************************************************
 *                           defineValues                              *
 ***********************************************************************/
void waterWindow::defineValues()
{
   char tmp[8];
   if(activePlane)
   {
      sprintf(tmp,"%.2f",activePlane->dX);
      dxWaterflowText->setText(tmp);
      sprintf(tmp,"%.2f",activePlane->dZ);
      dzWaterflowText->setText(tmp);
      intWindow->draw(0,0);
   }
}

/***********************************************************************
 *                           drawTemporary                             *
 ***********************************************************************/
void waterWindow::drawTemporary()
{
   if(activePlane != NULL)
   {
      glDisable(GL_LIGHTING);
      glColor3f(1.0,0.0,0.0);
      glBegin(GL_QUADS);
      if(activePlane->inclination == PLANE_NO_INCLINATION)
      {
         glVertex3f(activePlane->x1, activePlane->y1, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y1, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y1, activePlane->z2);
         glVertex3f(activePlane->x1, activePlane->y1, activePlane->z2);
      }
      else if(activePlane->inclination == PLANE_INCLINATION_X)
      {
         glVertex3f(activePlane->x1, activePlane->y1, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y2, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y2, activePlane->z2);
         glVertex3f(activePlane->x1, activePlane->y1, activePlane->z2);
      }
      else if(activePlane->inclination == PLANE_INCLINATION_Z)
      {
         glVertex3f(activePlane->x1, activePlane->y1, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y1, activePlane->z1);
         glVertex3f(activePlane->x2, activePlane->y2, activePlane->z2);
         glVertex3f(activePlane->x1, activePlane->y2, activePlane->z2);
      }

      glEnd();
      glEnable(GL_LIGHTING);
   }
}


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

#include "grasswindow.h"
#include "../../etc/dirs.h"

/**********************************************************
 *                      constructor                       *
 **********************************************************/
grassWindow::grassWindow(Farso::GuiInterface* actualGui)
{
#if 0
   actualWindow = NULL;
   actualGrass = NULL;
   pSystem = NULL;
   gui = actualGui;
   lastTime = 0;
#endif
}

/**********************************************************
 *                       destructor                       *
 **********************************************************/
grassWindow::~grassWindow()
{
}

#if 0
/**********************************************************
 *                        setGrass                        *
 **********************************************************/
void grassWindow::setGrass(grass* gr)
{
   actualGrass = gr;
   if(actualWindow == NULL)
   {
      openWindow();
   }
   defineValues();
}

/**********************************************************
 *                      openWindow                        *
 **********************************************************/
void grassWindow::openWindow()
{  
   dirs dir;
   dntFont fnt;
   actualWindow = gui->insertWindow(184,599-345,403,599-248,"Grass");
   
   actualWindow->getObjectsList()->insertTextBox(7,24,87,41,0,"Emptiness:");
   buttonPovoationLess = actualWindow->getObjectsList()->insertButton(88,23,
         98,40, fnt.createUnicode(0x25C4),0);
   textPovoation = actualWindow->getObjectsList()->insertTextBox(99,23,150,40,
                                                                 1,"20.0");
   buttonPovoationMore = actualWindow->getObjectsList()->insertButton(151,23,
         161,40, fnt.createUnicode(0x25BA),0);
   actualWindow->getObjectsList()->insertTextBox(7,44,87,61,0,"ScaleFactor:");
   buttonSizeLess = actualWindow->getObjectsList()->insertButton(88,43,98,60,
         fnt.createUnicode(0x25C4),0);
   textSize = actualWindow->getObjectsList()->insertTextBox(99,43,150,60,
         1,"1.0");
   buttonSizeMore = actualWindow->getObjectsList()->insertButton(151,43,161,60,
         fnt.createUnicode(0x25BA),0);

   navigationTab = actualWindow->getObjectsList()->insertTabButton(65,70,0,0,
                                 dir.getRealFile("mapEditor/nav2.png").c_str());
   buttonPrevious = navigationTab->insertButton(0,0,19,19);
   buttonDestroy = navigationTab->insertButton(20,0,39,19);
   buttonNext = navigationTab->insertButton(40,0,59,19);

   /* Open Window */
   actualWindow->setExternPointer(&actualWindow);
   actualWindow->setAttributes(false,true,false,false);
   gui->openWindow(actualWindow);
}

/**********************************************************
 *                     defineValues                       *
 **********************************************************/
void grassWindow::defineValues()
{
   char buf[64];
   GLfloat pX1=0, pX2=0, pZ1=0, pZ2=0;
   float povValue;

   if( (actualWindow) && (actualGrass) )
   {
      /* Write povoation */
      actualGrass->getPosition(pX1,pZ1,pX2,pZ2);
      povValue = ((pZ2-pZ1)*(pX2-pX1) / (float)actualGrass->getMaxParticles());
      sprintf(buf,"%.2f",povValue);
      textPovoation->setText(buf);

      /* Write Size */
      sprintf(buf,"%.2f",actualGrass->getScaleFactor());
      textSize->setText(buf);
      actualWindow->draw(0,0);
   }
}

/**********************************************************
 *                      drawTemporary                     *
 **********************************************************/
void grassWindow::drawTemporary()
{
   if(actualGrass)
   {
      GLfloat x1, z1, x2, z2;
      actualGrass->getPosition(x1,z1,x2,z2);
      glDisable(GL_LIGHTING);
         glColor3f(0.1,0.4,0.8);
         glBegin(GL_QUADS);
            glVertex3f(x1, 1, z1);
            glVertex3f(x2, 1, z1);
            glVertex3f(x2, 1, z2);
            glVertex3f(x1, 1, z2);
         glEnd();
      glEnable(GL_LIGHTING);
   }
}

/**********************************************************
 *                    setPartSystem                       *
 **********************************************************/
void grassWindow::setPartSystem(partController* pS)
{
   pSystem = pS;
}

/**********************************************************
 *                       eventGot                         *
 **********************************************************/
bool grassWindow::eventGot(int type, guiObject* object)
{
   float size;
   float pov;
   bool treated = false;
   int total;
   int actualTime = SDL_GetTicks();
   GLfloat pX1=0, pX2=0, pZ1=0, pZ2=0;

   /* If no window open, for sure the event is not from here */
   if(!actualWindow)
   {
      return(false);
   }

   if( (type == FARSO_EVENT_ON_PRESS_BUTTON) && 
       (actualTime - lastTime > 100) )
   {  
      lastTime = actualTime;
      if(object == (guiObject*)buttonSizeMore)
      {
         /* Inc the size of actual grass */
         if(actualGrass)
         {
            size = actualGrass->getScaleFactor();
            if(size <= 9.9)
            {
               actualGrass->setScaleFactor(size+0.1);
            }
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonSizeLess)
      {
         /* Dec the size of actual grass */
         if(actualGrass)
         {
            size = actualGrass->getScaleFactor();
            if(size >= 0.2)
            {
               actualGrass->setScaleFactor(size-0.1);
            }
         }
         treated = true;
      }
      if(object == (guiObject*)buttonPovoationMore)
      {
         /* Inc the povoation of actual grass */
         if(actualGrass)
         {
            actualGrass->getPosition(pX1,pZ1,pX2,pZ2);
            pov = ((pZ2-pZ1)*(pX2-pX1)/(float)actualGrass->getMaxParticles());
            if(pov < 7000)
            {
               pov += 1;
               total = (int)floor((pZ2-pZ1)*(pX2-pX1) / pov);
               actualGrass->setTotal(total);
            }
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonPovoationLess)
      {
         /* Dec the povoation of actual grass */
         if(actualGrass)
         {
            actualGrass->getPosition(pX1,pZ1,pX2,pZ2);
            pov = ((pZ2-pZ1)*(pX2-pX1)/(float)actualGrass->getMaxParticles());
            if(pov > 200)
            {
               pov -= 1;
               total = (int)floor((pZ2-pZ1)*(pX2-pX1) / pov);
               actualGrass->setTotal(total);
            }
         }
         treated = true;
      }

      /* Actualize Window, if needed */
      if(treated)
      {
         defineValues();
      }
   }
   else if(type == FARSO_EVENT_PRESSED_TAB_BUTTON)
   {
      if(object == (guiObject*)buttonPrevious)
      {
         if(actualGrass)
         {
            actualGrass = (grass*)actualGrass->getPrevious();
            defineValues();
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonNext)
      {
         if(actualGrass)
         {
            actualGrass = (grass*)actualGrass->getNext();
            defineValues();
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonDestroy)
      {
         if( (pSystem) && (actualGrass) )
         {
            grass* tmpGrass = NULL;
            if(actualGrass != (grass*)actualGrass->getPrevious())
            {
               tmpGrass = (grass*)actualGrass->getPrevious();
            }
            pSystem->removeParticle(actualGrass);
            actualGrass = tmpGrass;
            defineValues();
         }
         treated = true;
      }
   }

   return(treated);
}

#endif

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

#include "partoriginwindow.h"
#include "../../gui/farsoopts.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partOriginWindow::partOriginWindow(guiInterface* interf)
{
   curWindow = NULL;
   part = NULL;
   gui = interf;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
partOriginWindow::~partOriginWindow()
{
   closeWindow();
}

/***********************************************************************
 *                              setParticle                            *
 ***********************************************************************/
void partOriginWindow::setParticle(partAux* p)
{
   if(!p)
   {
      return;
   }

   if( (p != part) || (!isOpen()) )
   {
      part= p;
      openWindow();
   }
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool partOriginWindow::isOpen()
{
   return(curWindow != NULL);
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void partOriginWindow::openWindow()
{
   farsoOptions opt;
   int curPosX = 346;
   int curPosY = 64;
   int i;
   dntFont fnt;

   /* Close it if already opened */
   if(isOpen())
   {
      curPosX = curWindow->getX1();
      curPosY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(curPosX, curPosY, curPosX+191, curPosY+116,
                                "Origin"); 

   /* Type things */
   previousType = curWindow->getObjectsList()->insertButton(10, 20, 20, 37, 
         fnt.createUnicode(0x25C4),0);
   previousType->defineFont(opt.getDefaultFont(), 9);
   type = curWindow->getObjectsList()->insertTextBox(21, 20, 130, 37, 1, "");
   type->setFont(opt.getDefaultFont(), 9, DNT_FONT_ALIGN_CENTER);
   nextType = curWindow->getObjectsList()->insertButton(131, 20, 141, 37, 
         fnt.createUnicode(0x25BA),0);
   nextType->defineFont(opt.getDefaultFont(), 9);

   /* Lower Value */
   text[0] = curWindow->getObjectsList()->insertTextBox(10, 43, 80, 60, 0, 
         "Pos1:");
   for(i=0; i<3; i++)
   {
      pos1[i] = curWindow->getObjectsList()->insertTextBar(81+33*i, 43, 
            111+33*i, 60, "", 0);
   }
   /* Upper Value */
   text[1] = curWindow->getObjectsList()->insertTextBox(10, 66, 80, 83, 0, 
         "Pos2:");
   for(i=0; i<3; i++)
   {
      pos2[i] = curWindow->getObjectsList()->insertTextBar(81+33*i, 66, 
            111+33*i, 83, "", 0);
   }

   /* Radius */
   text[2] = curWindow->getObjectsList()->insertTextBox(10, 89, 80, 106, 0, 
         "Radius:");
   radius = curWindow->getObjectsList()->insertTextBar(81,89,131,106, "", 0);

   /* Finally open the window */
   gui->openWindow(curWindow);
   curWindow->setExternPointer(&curWindow);

   /* Set it */
   setTexts();
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void partOriginWindow::closeWindow()
{
   if(isOpen())
   {
      gui->closeWindow(curWindow);
   }
}

/***********************************************************************
 *                            setTypeText                              *
 ***********************************************************************/
void partOriginWindow::setTypeThings()
{
   dntPartOrigin* o = part->getOrigin();
   switch(o->type)
   {
      case DNT_PART_ORIGIN_POINT:
      {
         type->setText("Point");
         pos2[0]->setAvailable(false);
         pos2[1]->setAvailable(false);
         pos2[2]->setAvailable(false);
         radius->setAvailable(false);
         text[0]->setText("Position:");
      }
      break;
      case DNT_PART_ORIGIN_QUAD:
      {
         type->setText("Quad");
         pos2[0]->setAvailable(true);
         pos2[1]->setAvailable(true);
         pos2[2]->setAvailable(true);
         radius->setAvailable(false);
         text[0]->setText("Pos1:");
      }
      break;
      case DNT_PART_ORIGIN_SPHERE:
      {
         type->setText("Sphere");
         pos2[0]->setAvailable(false);
         pos2[1]->setAvailable(false);
         pos2[2]->setAvailable(false);
         radius->setAvailable(true);
         text[0]->setText("Center:");
      }
      break;
   }
   curWindow->draw();
}

/***********************************************************************
 *                             setTexts                                *
 ***********************************************************************/
void partOriginWindow::setTexts()
{
   char buf[128];
   int i;
   dntPartOrigin* o = part->getOrigin();
   setTypeThings();

   for(i = 0; i < 3; i++)
   {
      /* Lower */
      sprintf(buf, "%f", o->p[i]);
      pos1[i]->setText(buf);
      /* Upper */
      sprintf(buf, "%f", (o->p[i]+o->sum[i]));
      pos2[i]->setText(buf);
   }

   /* Radius */
   sprintf(buf, "%f", o->radius);
   radius->setText(buf);
}

/***********************************************************************
 *                            resetElement                             *
 ***********************************************************************/
void partOriginWindow::resetElement()
{
   float x1=0.0f,y1=0.0f,z1=0.0f,
         x2=0.0f,y2=0.0f,z2=0.0f;
   float r=0.0f;
   dntPartOrigin* o = part->getOrigin();

   /* Get lower */
   sscanf(pos1[0]->getText().c_str(), "%f", &x1);
   sscanf(pos1[1]->getText().c_str(), "%f", &y1);
   sscanf(pos1[2]->getText().c_str(), "%f", &z1);

   /* Get upper */
   sscanf(pos2[0]->getText().c_str(), "%f", &x2);
   sscanf(pos2[1]->getText().c_str(), "%f", &y2);
   sscanf(pos2[2]->getText().c_str(), "%f", &z2);

   /* Get radius */
   sscanf(radius->getText().c_str(), "%f", &r);

   /* Set, based on type */
   switch(o->type)
   {
      case DNT_PART_ORIGIN_POINT:
      {
         o->setPoint(x1, y1, z1);
      }
      break;
      case DNT_PART_ORIGIN_QUAD:
      {
         o->setQuad(x1, y1, z1, x2, y2, z2);
      }
      break;
      case DNT_PART_ORIGIN_SPHERE:
      {
         o->setSphere(x1, y1, z1, r);
      }
      break;
   }
}

/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
bool partOriginWindow::treat(guiObject* object, int eventInfo)
{
   dntPartOrigin* o;

   if(!isOpen())
   {
      /* No need to verify events on a closed window */
      return(false);
   }

   /* get the origin */
   o = part->getOrigin();

   /* Button press */
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      if(object == (guiObject*)nextType)
      {
         if(o->type < DNT_PART_ORIGIN_SPHERE)
         {
            o->type++;
            resetElement();
            setTypeThings();
            return(true);
         }
      }
      else if(object == (guiObject*)previousType)
      {
         if(o->type > 0)
         {
            o->type--;
            resetElement();
            setTypeThings();
            return(true);
         }
      }
   }
   else if(eventInfo == FARSO_EVENT_WROTE_TEXT_BAR)
   {
      if( (object == (guiObject*)pos1[0]) ||
          (object == (guiObject*)pos1[1]) ||
          (object == (guiObject*)pos1[2]) ||
          (object == (guiObject*)pos2[0]) ||
          (object == (guiObject*)pos2[1]) ||
          (object == (guiObject*)pos2[2]) ||
          (object == (guiObject*)radius) )
      {
         resetElement();
         return(true);
      }
   }

   return(false);
}


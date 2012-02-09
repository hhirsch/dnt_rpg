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

#include "rolbar.h"
#include "dntfont.h"
#include "guilist.h"
#include "interface.h"

#include <math.h>
#include <iostream>
using namespace std;

#define ROLBAR_UPDATE_RATE 50

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
rolBar::rolBar(int xa, int ya, int xb, int yb, string txt, guiList* list,
               SDL_Surface* surface):guiObject(surface)
{
   lastUpdated = SDL_GetTicks();
   if(!list)
   {
      cerr << "Unknow Objects List when inserting rolBar!" << endl;
      return;
   }
   intList = list;

   type = FARSO_OBJECT_ROL_BAR;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   maxHeight = (yb-ya-2);

   guiList* l = (guiList*)list;

   /* Edges */
   lastMouseY = -1;
   deltaY = 0.0f;
   position = l->insertButton(xb-10, ya+2, xb-2, yb-26, "", 0);
   sizeY = position->getY2() - position->getY1();
   contorn = l->insertTextBox(xb-12, ya, xb, yb-24, 1, "");
   
   /* Buttons */
   dntFont font;
   up = l->insertButton(xb-12,yb-23,xb,yb-12, font.createUnicode(0x25B2),0);
   up->defineFont(DNT_FONT_ARIAL, 8);
   down = l->insertButton(xb-12,yb-11,xb,yb, font.createUnicode(0x25BC),0);
   down->defineFont(DNT_FONT_ARIAL, 8);

   /* Text */
   scrollText = l->insertTextBox(xa,ya,xb-13,yb,1,""); 
   scrollText->setFont(DNT_FONT_ARIAL, 10, 0);
   scrollText->setColor(246, 190, 190);

   actualPressed = NULL;

   scrollText->setText(txt);

}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
rolBar::~rolBar()
{
}

/*********************************************************************
 *                             isOwner                               *
 *********************************************************************/
bool rolBar::isOwner(guiObject* obj)
{
   return( (obj == (guiObject*) up) ||
           (obj == (guiObject*) down) ||
           (obj == (guiObject*) scrollText) ||
           (obj == (guiObject*) contorn) ||
           (obj == (guiObject*) position) );
}

/*********************************************************************
 *                              eventGot                             *
 *********************************************************************/
bool rolBar::eventGot(int type, guiObject* object, int mouseY)
{
   /* Verify position button */
   if( (type == FARSO_EVENT_ON_PRESS_BUTTON) && 
       (object == (guiObject*)position) )
   {
      if(lastMouseY == -1)
      {
         /* set initial-procedure mouseY */
         lastMouseY = mouseY;
         deltaY = mouseY-position->getY1();
      }
      else if(mouseY != lastMouseY)
      {
         /* Update last mouse change */
         lastMouseY = mouseY;

         /* Set initial text line */
         int dif = (position->getY2()-position->getY1());
         int newInit = (int)floor( ( (mouseY-deltaY - y1 - 2) / 
                                     (float)(y2-28-y1) ) *
                                   scrollText->getTotalLines());
         int newEnd = (int)floor( ( (mouseY-deltaY+dif - y1 - 2) /
                  (float)(y2-28-y1) ) *
               scrollText->getTotalLines())-1;
         if(newInit < 0)
         {
            newInit = 0;
         }
         else if(newEnd > scrollText->getTotalLines()-1) //FIXME!
         {
            newInit = scrollText->getTotalLines()-1-(newEnd-newInit);
         }
         scrollText->setFirstLine(newInit);
         
         /* mark to update */
         setChanged();
         return(true);
      }
   }
   else
   {
      lastMouseY = -1;
   }

   /* Verify press up or down buttons */
   if((SDL_GetTicks() - lastUpdated) >= ROLBAR_UPDATE_RATE)
   {
      lastUpdated = SDL_GetTicks();
      if(type == FARSO_EVENT_ON_PRESS_BUTTON)
      {
         if(object == (guiObject*)up)
         {
            if(scrollText->getFirstLine() > 0)
            {
               scrollText->setFirstLine(scrollText->getFirstLine() - 1);
            }
            actualPressed = up;
            setChanged();
            return(true);
         }
         else if(object == (guiObject*)down)
         {
            if(scrollText->lastDrawableLine() < scrollText->getTotalLines()-1)
            {
               scrollText->setFirstLine(scrollText->getFirstLine() + 1);
               actualPressed = down;
               setChanged();
            }
            return(true);
         }
         else
         {
            actualPressed = NULL;
         }
      }
   }
   return(false);
}

/*********************************************************************
 *                               draw                                *
 *********************************************************************/
void rolBar::draw(int i)
{
   /* No draw when hidden */
   if(isVisible())
   {
      scrollText->draw(i);
   }
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void rolBar::draw()
{
   /* Redraw the TextBox and Bar */
   redraw();

   /* Redraw Buttons */
   up->draw();
   down->draw();
}

/*********************************************************************
 *                               redraw                              *
 *********************************************************************/
void rolBar::redraw()
{
   int actualInit = scrollText->getFirstLine();

   contorn->draw();
   int end = scrollText->draw2();

   if(scrollText->getTotalLines() != 0)
   {
      //deltaY = (float)scrollText->getTotalLines() /  (y2-28.0f-y1);
      position->setCoordinate(position->getX1(), 
                              (int) ((y1+2) + ((float)actualInit/
                                     (float)scrollText->getTotalLines())*
                                    (y2-28-y1)),
                              position->getX2(),
                              (int) ((y1+2) + ((float)(end+1)/
                                     (float)scrollText->getTotalLines())*
                                    (y2-28-y1)));
   }
   else
   {
      //deltaY = 0;
      position->setCoordinate(position->getX1(), y1+2, 
                              position->getX2(), y2-26);
   }
   sizeY = position->getY2() - position->getY1();
   position->draw();
   setChanged();
}

/*********************************************************************
 *                              setText                              *
 *********************************************************************/
void rolBar::setText(string txt)
{
   scrollText->setText(txt);
   scrollText->setFirstLine(0);
   redraw();
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt)
{
   scrollText->addText(txt);
   scrollText->setLastLine(scrollText->getTotalLines()-1);
   redraw();
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt, string font, int size,
                     int align, int style)
{
   scrollText->addText(txt, font, size, align, style);
   scrollText->setLastLine(scrollText->getTotalLines()-1);
   redraw();
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt, string font, int size,
                     int align, int style, int R, int G, int B)
{
   scrollText->addText(txt, font, size, align, style, R, G, B);
   scrollText->setLastLine(scrollText->getTotalLines()-1);
   redraw();
}

/*********************************************************************
 *                           setFirstLine                            *
 *********************************************************************/
void rolBar::setFirstLine(int line)
{
   scrollText->setFirstLine(line);
   redraw();
}

/*********************************************************************
 *                           getFirstLine                            *
 *********************************************************************/
int rolBar::getFirstLine()
{
   return(scrollText->getFirstLine());
}

/*********************************************************************
 *                            getLastLine                            *
 *********************************************************************/
string rolBar::getLastLine()
{
   return(scrollText->getTextLine(scrollText->getTotalLines()-1));
}


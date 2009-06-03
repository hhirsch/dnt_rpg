/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "rolbar.h"
#include "dntfont.h"
#include "guilist.h"
#include "interface.h"

#include <iostream>
using namespace std;

#define ROLBAR_UPDATE_RATE 50

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
rolBar::rolBar(int xa, int ya, int xb, int yb, string txt, void* list,
               SDL_Surface* surface)
{
   wSurface = surface;
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
   position = l->insertTextBox(xb-10, ya+2, xb-2, yb-26, 1, "");
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
bool rolBar::eventGot(int type, guiObject* object)
{
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
 *                                draw                               *
 *********************************************************************/
void rolBar::draw(SDL_Surface* screen)
{
   /* Redraw the TextBox and Bar */
   redraw();

   /* Redraw Buttons */
   up->draw(screen);
   down->draw(screen);
}

/*********************************************************************
 *                               redraw                              *
 *********************************************************************/
void rolBar::redraw()
{
   int actualInit = scrollText->getFirstLine();

   contorn->draw();
   int end = scrollText->draw();

   if(scrollText->getTotalLines() != 0)
   {
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
      position->setCoordinate(position->getX1(), y1+2, 
                              position->getX2(), y2-26);
   }
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


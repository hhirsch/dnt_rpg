/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "rolbar.h"
#include "dntfont.h"
#include "guilist.h"
#include "interface.h"

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
      printf("Unknow Objects List when inserting rolBar!\n");
      return;
   }
   intList = list;

   type = GUI_ROL_BAR;
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
   scrollText = l->insertTextBox(xa,ya,xb-13,yb,2,""); 
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
 *                              eventGot                             *
 *********************************************************************/
bool rolBar::eventGot(int type, guiObject* object)
{
   if((SDL_GetTicks() - lastUpdated) >= ROLBAR_UPDATE_RATE)
   {
      lastUpdated = SDL_GetTicks();
      if(type == ON_PRESS_BUTTON)
      {
         if(object == (guiObject*)up)
         {
            if(scrollText->getFirstLine() > 0)
            {
               scrollText->setFirstLine(scrollText->getFirstLine() - 1);
            }
            actualPressed = up;
            return(true);
         }
         else if(object == (guiObject*)down)
         {
            scrollText->setFirstLine(scrollText->getFirstLine() + 1);
            if(scrollText->lastDrawableLine() >= scrollText->getTotalLines())
            {
               scrollText->setFirstLine(scrollText->getFirstLine() - 1);
            }
            actualPressed = down;
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
 *                               redraw                              *
 *********************************************************************/
void rolBar::redraw()
{
   int actualInit = scrollText->getFirstLine();

   contorn->draw(wSurface);
   int end = scrollText->draw(wSurface);

   position->setCoordinate(position->getX1(), 
                              (int) ((y1+2) + ((float)actualInit/
                                     (float)scrollText->getTotalLines())*
                                    (y2-28-y1)),
                              position->getX2(),
                              (int) ((y1+2) + ((float)(end+1)/
                                     (float)scrollText->getTotalLines())*
                                    (y2-28-y1)));
   position->draw(wSurface);
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
   scrollText->setFirstLine(scrollText->getTotalLines());
   redraw();
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt, string font, int size,
                     int align, int style)
{
   scrollText->addText(txt, font, size, align, style);
   scrollText->setFirstLine(scrollText->getTotalLines());
   redraw();

}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt, string font, int size,
                     int align, int style, int R, int G, int B)
{
   scrollText->addText(txt, font, size, align, style, R, G, B);
   scrollText->setFirstLine(scrollText->getTotalLines()-1);
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


/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "rolbar.h"
#include "dntfont.h"
#include "guilist.h"
#include "interface.h"

#define UPDATE_RATE 50

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
rolBar::rolBar(int xa, int ya, int xb, int yb, string txt, void* list,
               SDL_Surface* surface)
{
   dntFont fnt;
   wSurface = surface;
   lastUpdated = SDL_GetTicks();
   if(!list)
   {
      printf("Unknow Objects List when inserting rolBar!\n");
      return;
   }
   intList = list;

   fnt.defineFont(DNT_FONT_ARIAL, 10);
   type = GUI_ROL_BAR;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   maxLines = ((yb-ya-2) / fnt.getHeight());

   guiList* l = (guiList*)list;

   /* Edges */
   position = l->insertTextBox(xb-10, ya+2, xb-2, yb-26, 1, "");
   contorn = l->insertTextBox(xb-12, ya, xb, yb-24, 1, "");
   
   /* Buttons */
   up = l->insertButton(xb-12,yb-23,xb,yb-12, fnt.createUnicode(0x25B2),0);
   up->defineFont(DNT_FONT_ARIAL, 8);
   down = l->insertButton(xb-12,yb-11,xb,yb, fnt.createUnicode(0x25BC),0);
   down->defineFont(DNT_FONT_ARIAL, 8);

   /* Text */
   scrollText = l->insertTextBox(xa,ya,xb-13,yb,2,""); 
   scrollText->setFont(DNT_FONT_ARIAL, 10, 0);
   scrollText->setColor(246, 190, 190);

   actualPressed = NULL;

   setText(txt);

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
   dntFont fnt;
   if((SDL_GetTicks() - lastUpdated) >= UPDATE_RATE)
   {
      lastUpdated = SDL_GetTicks();
      if(type == ON_PRESS_BUTTON)
      {
         if(object == (guiObject*)up)
         {
            if(actualInit > 0)
            {
               actualInit -= 1;
               actualEnd = actualInit + maxLines -1;
            }
            
            scrollText->setText(fnt.copyLines(fullText, actualInit,actualEnd,
                                              x1+2,x2-13));
            actualPressed = up;
            return(true);
         }
         else if(object == (guiObject*)down)
         {
            if(actualEnd < totalLines)
            {
               actualEnd += 1;
               actualInit = (actualEnd - maxLines) +1;
               if(actualInit < 0)
               {
                  actualInit = 0;
               }
            }

            scrollText->setText(fnt.copyLines(fullText, actualInit,actualEnd,
                                              x1+2,x2-13));
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
   contorn->draw(wSurface);
   if(maxLines <= totalLines)
   {
      position->setCoordinate(position->getX1(), 
                              (int) ((y1+2) + ((float)actualInit/
                                               (float)totalLines)*
                                    (y2-28-y1)),
                              position->getX2(),
                              (int) ((y1+2) + ((float)(actualEnd)/
                                               (float)totalLines)*
                                    (y2-28-y1)));
   }
   position->draw(wSurface);   
   scrollText->draw(wSurface);

   /* Mantain the draw of button pressed */
   if(actualPressed)
   {
      actualPressed->draw(1, wSurface);
   }
}

/*********************************************************************
 *                              setText                              *
 *********************************************************************/
void rolBar::setText(string txt)
{
   dntFont fnt;
   fullText = txt;
   totalLines = fnt.getTotalLines(fullText,x1+2,x2-13);
   actualInit = 0;
   actualEnd = maxLines - 1;
   if(maxLines <= totalLines)
   {
      position->setCoordinate(position->getX1(), 
                              (int) ((y1+2) + ((float)actualInit/
                                               (float)totalLines)*
                                    (y2-28-y1)),
                              position->getX2(),
                              (int) ((y1+2) + ((float)(actualEnd)/
                                               (float)totalLines)*
                                    (y2-28-y1)));
   }
   else
   {
      position->setCoordinate(position->getX1(), y1+2,
                              position->getX2(), y2-26);
   }
   scrollText->setText(fnt.copyLines(fullText,actualInit,actualEnd,x1+2,x2-13));
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt)
{
   dntFont fnt;
   /* Add Text */
   fullText += txt;
   /* Set the new scrollText */
   setText(fullText);
   /* Put the rolling bar to the end */
   actualEnd = totalLines+1;
   actualInit = (actualEnd - maxLines) +1;
   if(actualInit < 0)
   {
      actualInit = 0;
   }
   scrollText->setText(fnt.copyLines(fullText,actualInit,actualEnd,x1+2,x2-13));
   redraw();
}


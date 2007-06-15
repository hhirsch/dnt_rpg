/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "rolbar.h"
#include "fonte.h"
#include "lista.h"
#include "interface.h"

#define ACTUALIZE_RATE 50

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
rolBar::rolBar(int xa, int ya, int xb, int yb, string txt, void* list,
               SDL_Surface* surface)
{
   wSurface = surface;
   lastActualized = SDL_GetTicks();
   if(!list)
   {
      printf("Unknow Objects List when inserting rolBar!\n");
      return;
   }
   intList = list;

   defineFont(FMINI, ALIGN_LEFT, 1);
   type = GUI_ROL_BAR;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   maxLines = ((yb-ya) / 11)-1;
   charPerLine = (xb-xa) / font_incCP();
   
   Tlista* l = (Tlista*)list;

   /* Contorno */
   position = l->insertTextBox(xb-10, ya+2, xb-2, yb-26, 1, "");
   contorn = l->insertTextBox(xb-12, ya, xb, yb-24, 1, "");
   
   /* Botoes */
   up = l->insertButton(xb-12,yb-23,xb,yb-12, "\36",0);
   down = l->insertButton(xb-12,yb-11,xb,yb, "\37",0);

   /* Texto */
   scrollText = l->insertTextBox(xa,ya,xb-13,yb,2,""); 
   scrollText->setFont(FMINI, 1, ALIGN_LEFT);
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
   if((SDL_GetTicks() - lastActualized) >= ACTUALIZE_RATE)
   {
      lastActualized = SDL_GetTicks();
      if(type == ON_PRESS_BUTTON)
      {
         if(object == (guiObject*)up)
         {
            if(actualInit > 0)
            {
               actualInit -= 1;
               actualEnd = actualInit + maxLines -1;
            }
            
            scrollText->setText(copyLines(fullText, actualInit,actualEnd));
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

            scrollText->setText(copyLines(fullText, actualInit,actualEnd));
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
   fullText = txt;
   totalLines = getTotalLines(fullText);
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
   scrollText->setText(copyLines(fullText, actualInit,actualEnd));
}

/*********************************************************************
 *                              addText                              *
 *********************************************************************/
void rolBar::addText(string txt)
{
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
   scrollText->setText(copyLines(fullText, actualInit,actualEnd));
   redraw();
}


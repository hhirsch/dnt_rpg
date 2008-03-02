/* DccNiTghmare is in Public Domain. Do whatever you want with this code. */

#include "tabbutton.h"
#include "draw.h"
#include "mouse.h"

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
tabButton::tabButton(int x,int y,const char* arquivo):picture(x,y,0,0,arquivo)
{
   numButtons = 0;
   pressed = false;
   type = GUI_TAB_BUTTON;
   current = - 1;
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
tabButton::tabButton(int x, int y, int w, int h):picture(x,y,0,0, NULL)
{
   type = GUI_TAB_BUTTON;
   numButtons = 0;
   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h;
   pressed = false;
   current = -1;
}

/***********************************************************
 *                       insertButton                      *
 ***********************************************************/
oneTabButton* tabButton::insertButton(int x1, int y1, int x2, int y2)
{
   if(numButtons < MAX_TABBUTTONS)
   {
      Buttons[numButtons].x1 = x1;
      Buttons[numButtons].y1 = y1;
      Buttons[numButtons].x2 = x2;
      Buttons[numButtons].y2 = y2;
      numButtons++;
      return(&Buttons[numButtons-1]);
   }
   return(NULL);
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void tabButton::draw(SDL_Surface *screen)
{ 
   int i;
   picture* fig;
   fig = this;
   fig->draw(screen);
   for(i=0;i<numButtons;i++)
   {
      if(i == current)
      {
          color_Set(cor.colorCont[0].R,cor.colorCont[0].G,cor.colorCont[0].B);
          rectangle_2Colors(screen,x1+Buttons[i].x1,y1+Buttons[i].y1,
                            x1+Buttons[i].x2,y1+Buttons[i].y2,
                            cor.colorCont[1].R,cor.colorCont[1].G,
                            cor.colorCont[1].B);
      }
      else if(fig->get() == NULL)
      {
         color_Set(cor.colorWindow.R, cor.colorWindow.G, cor.colorWindow.B);
         rectangle_Draw(screen, x1+Buttons[i].x1,y1+Buttons[i].y1,
                        x1+Buttons[i].x2,y1+Buttons[i].y2);
      }
   }
   setChanged();
}

/***********************************************************
 *                      verifyPosition                     *
 ***********************************************************/
guiObject* tabButton::verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                                   int Xjan, int Yjan, SDL_Surface *screen,
                                   int &actionType)
{
   int i;
   bool atButton = false;
   actionType = TABBUTTON_NONE;
   for(i=0;i<numButtons;i++)
   {
      if(isMouseAt(x1+Buttons[i].x1+Xjan,y1+Buttons[i].y1+Yjan,
                   x1+Buttons[i].x2+Xjan,y1+Buttons[i].y2+Yjan,
                   mouseX, mouseY))
      {

         if(i != current)
         {
            current = i;
            atButton = true;
            draw(screen);
         }

         if(Mbuttons & SDL_BUTTON(1))
         {
            /* Only return when released the mouse button! */
            actionType = TABBUTTON_ON_PRESS;
            pressed = true;  
            return((guiObject*) &Buttons[i]);
         }
         else
         {
            if(pressed)
            {
               actionType = TABBUTTON_PRESSED;
               pressed = false;
               return((guiObject*) &Buttons[i]);
            }
            else
            {
               /* Still in focus, but no action done */
               return((guiObject*) &Buttons[i]);
            }
         }
      }
   }

   if(!atButton)
   {
      current = -1;
      draw(screen);
   }

   return(NULL);
}


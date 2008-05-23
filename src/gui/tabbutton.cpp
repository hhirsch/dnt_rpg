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
   objectBelow = NULL;
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
   objectBelow = NULL;
}

/***********************************************************
 *                     setObjectBelow                      *
 ***********************************************************/
void tabButton::setObjectBelow(guiObject* obj)
{
   objectBelow = obj;
}

/***********************************************************
 *                       setCurrent                        *
 ***********************************************************/
void tabButton::setCurrent(int i)
{
   current = i;
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
   /* Clear Below */
   color_Set(cor.colorWindow.R, cor.colorWindow.G, 
             cor.colorWindow.B, cor.colorWindow.A);
   rectangle_Fill(screen, x1,y1, x2, y2);

   /* Draw below Object, if exists */
   if(objectBelow)
   {
      objectBelow->draw(screen);
   }

   /* Draw Picture (if one) */
   if(fig)
   {
      SDL_Rect Ret;
      Ret.x = x1;
      Ret.y = y1;
      Ret.w = fig->w;
      Ret.h = fig->h;
      SDL_BlitSurface(fig,NULL,screen,&Ret);
   }

   /* Draw Button Contorns */
   if(current >=0)
   {
      color_Set(cor.colorCont[0].R, cor.colorCont[0].G,
                cor.colorCont[0].B, cor.colorCont[0].A);
      rectangle_2Colors(screen,x1+Buttons[current].x1,
                               y1+Buttons[current].y1,
                               x1+Buttons[current].x2,
                               y1+Buttons[current].y2,
                        cor.colorCont[1].R, cor.colorCont[1].G,
                        cor.colorCont[1].B, cor.colorCont[1].A);
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

   if((!atButton) && (current != -1))
   {
      current = -1;
      draw(screen);
   }

   return(NULL);
}


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

#include "tabbutton.h"
#include "textbox.h"
#include "rolbar.h"
#include "draw.h"
#include "mouse.h"
using namespace std;
using namespace Farso;

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
TabButton::TabButton(int x,int y,const char* arquivo, SDL_Surface* surface)
          :Picture(x,y,0,0, arquivo, surface)
{
   int i;

   numButtons = 0;
   pressed = false;
   rightPressed = false;
   type = Farso::OBJECT_TAB_BUTTON;
   style = STYLE_NORMAL;
   current = - 1;

   for(i=0; i < TABBUTTON_BELLOW; i++)
   {
      objectsBelow[i] = NULL;
   }
   curBelow = -1;
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
TabButton::TabButton(int x, int y, int w, int h, SDL_Surface* surface)
          :Picture(x,y,0,0, NULL, surface)
{
   int i;

   type = Farso::OBJECT_TAB_BUTTON;
   numButtons = 0;
   x1 = x;
   y1 = y;
   x2 = x+w;
   y2 = y+h;
   pressed = false;
   rightPressed = false;
   current = -1;
   for(i=0; i < TABBUTTON_BELLOW; i++)
   {
      objectsBelow[i] = NULL;
   }
   curBelow = -1;
   style = STYLE_NORMAL;
}

/***********************************************************
 *                     setObjectBelow                      *
 ***********************************************************/
void TabButton::setObjectBelow(GuiObject* obj)
{
   if(curBelow+1 < TABBUTTON_BELLOW)
   {
      curBelow++;
      objectsBelow[curBelow] = obj;
      return;
   }

   cerr << "WARNING: tabButon: max objects bellow reached!" << endl;
}

/***********************************************************
 *                       setCurrent                        *
 ***********************************************************/
void TabButton::setCurrent(int i)
{
   current = i;
}

/***********************************************************
 *                        setStyle                         *
 ***********************************************************/
void TabButton::setStyle(int st)
{
   style = st;
}

/***********************************************************
 *                       insertButton                      *
 ***********************************************************/
OneTabButton* TabButton::insertButton(int x1, int y1, int x2, int y2)
{
   if(numButtons < MAX_TABBUTTONS)
   {
      buttons[numButtons].x1 = x1;
      buttons[numButtons].y1 = y1;
      buttons[numButtons].x2 = x2;
      buttons[numButtons].y2 = y2;
      numButtons++;
      return(&buttons[numButtons-1]);
   }
   return(NULL);
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void TabButton::draw()
{ 
   int i;
   
   /* No draw when hidden */
   if(!isVisible())
   {
      return;
   }

   /* Clear Below */
   color_Set(cor.colorWindow.R, cor.colorWindow.G, 
             cor.colorWindow.B, cor.colorWindow.A);
   rectangle_Fill(wSurface, x1,y1, x2, y2);

   /* Draw below Object, if exists */
   for(i=curBelow; i >= 0; i--)
   {
      objectsBelow[i]->draw();
   }

   /* Draw Picture (if one) */
   if(fig)
   {
      SDL_Rect Ret;
      Ret.x = x1;
      Ret.y = y1;
      Ret.w = fig->w;
      Ret.h = fig->h;
      SDL_BlitSurface(fig,NULL,wSurface,&Ret);
   }

   /* Draw Button Contorns */
   if(current >=0)
   {
      if(style == STYLE_NORMAL)
      {
        color_Set(cor.colorCont[0].R, cor.colorCont[0].G,
                cor.colorCont[0].B, cor.colorCont[0].A);
        rectangle_2Colors(wSurface,x1+buttons[current].x1,
                                 y1+buttons[current].y1,
                                 x1+buttons[current].x2,
                                 y1+buttons[current].y2,
                          cor.colorCont[1].R, cor.colorCont[1].G,
                          cor.colorCont[1].B, cor.colorCont[1].A);
      }
      else if(style == STYLE_HIGH)
      {
         color_Set(cor.colorHigh.R,cor.colorHigh.G,
               cor.colorHigh.B, cor.colorHigh.A);
         rectangle_Draw(wSurface,x1+buttons[current].x1,
                                 y1+buttons[current].y1,
                                 x1+buttons[current].x2,
                                 y1+buttons[current].y2);
         rectangle_Draw(wSurface,x1+buttons[current].x1+1,
                                 y1+buttons[current].y1+1,
                                 x1+buttons[current].x2-1,
                                 y1+buttons[current].y2-1);
      }
      else if(style == STYLE_LIST_TEXT)
      {
         color_Set(cor.colorHigh.R,cor.colorHigh.G,
               cor.colorHigh.B, cor.colorHigh.A);
         rectangle_Fill(wSurface, x1+buttons[current].x1,
               y1+buttons[current].y1,
               x1+buttons[current].x2,
               y1+buttons[current].y2);

         if( (objectsBelow[0] != NULL) && 
               (objectsBelow[0]->type == Farso::OBJECT_ROL_BAR) )
         {
            RolBar* rb = (RolBar*)objectsBelow[0];
            rb->draw(current+rb->getFirstLine());
         }
      }
   }
   setChanged();
}

/***********************************************************
 *                      verifyPosition                     *
 ***********************************************************/
GuiObject* TabButton::verifyPosition(int mouseX, int mouseY, Uint8 Mbuttons, 
                                   int Xjan, int Yjan, int &actionType)
{
   int i;
   bool atButton = false;
   actionType = TABBUTTON_NONE;
   for(i=0;i<numButtons;i++)
   {
      if( (buttons[i].isAvailable()) &&
          (isMouseAt(x1+buttons[i].x1+Xjan,y1+buttons[i].y1+Yjan,
                     x1+buttons[i].x2+Xjan,y1+buttons[i].y2+Yjan,
                     mouseX, mouseY)) )
      {

         if(i != current)
         {
            current = i;
            atButton = true;
            draw();
         }

         if(Mbuttons & SDL_BUTTON(1))
         {
            /* Only return when released the mouse button! */
            actionType = TABBUTTON_ON_PRESS;
            pressed = true;  
            return((GuiObject*) &buttons[i]);
         }
         else if(Mbuttons & SDL_BUTTON(3))
         {
            /* Only return when released the mouse button! */
            actionType = TABBUTTON_ON_RIGHT_PRESS;
            rightPressed = true;  
            return((GuiObject*) &buttons[i]);
         }
         else
         {
            if(pressed)
            {
               actionType = TABBUTTON_PRESSED;
               pressed = false;
               return((GuiObject*) &buttons[i]);
            }
            else if(rightPressed)
            {
               actionType = TABBUTTON_RIGHT_PRESSED;
               rightPressed = false;
               return((GuiObject*) &buttons[i]);
            }
            else
            {
               /* Still in focus, but no action done */
               return((GuiObject*) &buttons[i]);
            }
         }
      }
   }

   if((!atButton) && (current != -1))
   {
      current = -1;
      draw();
   }

   return(NULL);
}


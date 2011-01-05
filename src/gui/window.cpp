/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "window.h"
#include "menu.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
windowList::windowList()
{
   activeWindow = NULL;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
windowList::~windowList()
{
   clearList();
} 

/*********************************************************************
 *                           freeElement                             *
 *********************************************************************/
void windowList::freeElement(dntListElement* obj)
{
   /* Delete the window */
   window* w = (window*)obj;
   delete(w);
}
 
/*********************************************************************
 *                           insertWindow                            *
 *********************************************************************/
window* windowList::insertWindow(int xa,int ya,int xb,int yb,string text,
      bool empty)
{
   window* novo;

   /* Create and insert the window on the list */
   novo = new window(xa,ya,xb,yb,text,this, empty);
   insert(novo);

   return(novo);
} 

/*********************************************************************
 *                           removeWindow                            *
 *********************************************************************/
void windowList::removeWindow(window *jan)
{
   /* Verify ActiveWindow Pointer */
   if(jan == activeWindow)
   {
      activeWindow = NULL;
      if((total > 1))
      {
         window* j = (window*)jan->getPrevious();
         j->activate();
      }
   }

   remove(jan);
}

/*********************************************************************
 *                             Constructor                           *
 *********************************************************************/
window::window(int xa, int ya, int xb, int yb, string title, void* list,
      bool empty):guiObject(NULL)
{
   dntFont fnt;
   dirs dir;

   /* Set Variables */
   intList = list;
   x1 = xa;
   x2 = xb;
   y1 = ya;
   y2 = yb;
   minY = 15;
   minX = 50;
   text = title;
   canMaximize = !empty;
   canScale = !empty;
   canClose = !empty;
   canMove = !empty;
   modal = false;
   hasSelfDraw = !empty;
   visible = true;
   externPointer = NULL;

   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
   #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
   #else
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
   #endif

   alpha = 0.75;
   /* Create Surface And generate texture */
   glGenTextures(1,&texture);
   surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                  smallestPowerOfTwo(xb-xa),
                                  smallestPowerOfTwo(yb-ya),32,
                                  rmask,gmask,bmask,amask);
   color_Set(255, 255, 255, hasSelfDraw?255:0);
   rectangle_Fill(surface, 0,0, smallestPowerOfTwo(xb-xa)-1,
                  smallestPowerOfTwo(yb-ya)-1);

   propX = (float)(xb-xa) / (float)smallestPowerOfTwo(xb-xa);
   propY = (float)(yb-ya) / (float)smallestPowerOfTwo(yb-ya);

   /* Create Objects List */
   objects = new guiList(surface);


   /* Create title bar things */
   if(hasSelfDraw)
   {
      /* Create Menu Button */
      menuButton = objects->insertButton(3,3,13,12,"-",0);
      menuButton->men = new menu(0,0,surface);
      menu* men = (menu*) menuButton->men;
      men->insertItem(gettext("Maximize"), 
            dir.getRealFile("icons/maximize.png") ,0);
      men->insertItem("-",0);
      men->insertItem(gettext("Close"), dir.getRealFile("icons/close.png"), 1);

      /* Create Close Button */
      closeButton = objects->insertButton(14,3,24,12,
            fnt.createUnicode(0x25CF),0);
      closeButton->defineFont(DNT_FONT_ARIAL, 10);

      /* Create Minimize Maximize Button */
      minMaxButton = objects->insertButton(25,3,35,12,
            fnt.createUnicode(0x25B2),0);
      minMaxButton->defineFont(DNT_FONT_ARIAL, 8);
   }
   else
   {
      /* No title bar */
      menuButton = NULL;
      closeButton = NULL;
      minMaxButton = NULL;
   }

   /* Set the object type as WINDOW! */
   type = FARSO_OBJECT_WINDOW;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
window::~window()
{
   if(externPointer != NULL)
   {
      *externPointer = NULL;
   }
   glDeleteTextures(1,&texture);
   SDL_FreeSurface(surface);
   delete(objects);
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void window::draw()
{
   draw(-1,-1, true);
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void window::draw(int mouseX, int mouseY, bool drawBar)
{
   int dx = x2 - x1;
   int dy = y2 - y1;

   if(hasSelfDraw)
   {
      color_Set(Colors.colorWindow.R, Colors.colorWindow.G,
            Colors.colorWindow.B, Colors.colorWindow.A);
      rectangle_Fill(surface, 3,3,dx-3,dy-3);
      color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
            Colors.colorCont[0].B, Colors.colorCont[0].A);
      rectangle_Draw(surface,0,0,dx-1,dy-1);
      color_Set(Colors.colorButton.R, Colors.colorButton.G, 
            Colors.colorButton.B, Colors.colorButton.A);
      rectangle_Draw(surface,1,1,dx-2,dy-2);
      color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
            Colors.colorCont[2].B, Colors.colorCont[2].A);
      line_Draw(surface,2,13,dx-4,13);
      color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
            Colors.colorCont[0].B, Colors.colorCont[0].A);
      rectangle_2Colors(surface,2,2,dx-3,dy-3,Colors.colorCont[2].R,
            Colors.colorCont[2].G,Colors.colorCont[2].B,
            Colors.colorCont[2].A);


      /* Draw the bar */
      windowList* lst = (windowList*)intList;

      if(drawBar)
      {
         if(this == lst->getActiveWindow())
         {
            drawActiveBar();
         }
         else
         {
            drawInactiveBar();
         }
      }
   }

   /* Objects Draw */
   objects->draw();
   
   setChanged();
}

/*********************************************************************
 *                         drawInactiveBar                           *
 *********************************************************************/
void window::drawInactiveBar()
{
   dntFont fnt;
   int dx = x2-x1;

   /* Redraw All window, removing the mouse from it */
   draw(-1,-1, false);

   if(hasSelfDraw)
   {
      /* Redraw the Inactive Bar */
      color_Set(Colors.colorWindow.R, Colors.colorWindow.G,
            Colors.colorWindow.B, Colors.colorWindow.A);
      rectangle_Fill(surface,36,3,dx-3,12);
      color_Set(Colors.colorBar.R+100, Colors.colorBar.G,
            Colors.colorBar.B, Colors.colorBar.A);
      fnt.defineFont(DNT_FONT_ARIAL,10);
      fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
      fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
      fnt.write(surface,39,1,text);
   }
   setChanged();
}

/*********************************************************************
 *                           drawActiveBar                           *
 *********************************************************************/
void window::drawActiveBar()
{
   dntFont fnt;
   int dx = x2-x1;

   if(hasSelfDraw)
   {
      color_Set(Colors.colorBar.R, Colors.colorBar.G,
            Colors.colorBar.B, Colors.colorBar.A);
      rectangle_Fill(surface,36,3,dx-3,12);
      color_Set(Colors.colorText.R, Colors.colorText.G,
            Colors.colorText.B, Colors.colorText.A);
      fnt.defineFont(DNT_FONT_ARIAL,10);
      fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
      fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
      fnt.write(surface,39,1,text);
      setChanged();
   }
}

/*********************************************************************
 *                               flush                               *
 *********************************************************************/
void window::flush()
{
   setTextureRGBA(surface, texture);
}

/*********************************************************************
 *                              render                               *
 *********************************************************************/
void window::render(float depth)
{
   /* Update the Texture if needed */
   if(changed())
   {
      flush();
   }

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture);

   glColor4f(1.0, 1.0, 1.0, /*alpha*/ 1.0);

   glPushMatrix();
      glBegin(GL_QUADS);
         glTexCoord2f(0.0, 0.0);
         glVertex3f(x1, SCREEN_Y-y1, depth);
         glTexCoord2f(0.0, propY);
         glVertex3f(x1, SCREEN_Y-y2, depth);
         glTexCoord2f(propX, propY);
         glVertex3f(x2, SCREEN_Y-y2, depth);
         glTexCoord2f(propX, 0.0);
         glVertex3f(x2, SCREEN_Y-y1, depth);
      glEnd();
   glPopMatrix();

   glColor4f(1.0,1.0,1.0,1.0);
   glDisable(GL_TEXTURE_2D);
}

/*********************************************************************
 *                             activate                              *
 *********************************************************************/
void window::activate()
{
  windowList *ljan = (windowList*) intList;
  if (ljan->getActiveWindow() != NULL)
  {
     if(ljan->getActiveWindow()->isModal())
     {
        /* The current active window is a modal one,
         * so it continue as the active window! */
        return;
     }

     /* Draw the Inactive TitleBar to the current active window */
     ljan->getActiveWindow()->drawInactiveBar();
  }

  /* Set this window as the new active one */
  ljan->setActiveWindow(this);

  /* And obviously, draw its active bar */
  drawActiveBar();
}

/*********************************************************************
 *                               open                                *
 *********************************************************************/
void window::open()
{
   activate();
   draw(0,0);
   setChanged();
}

/*********************************************************************
 *                          setAttributes                            *
 *********************************************************************/
void window::setAttributes(bool close, bool move, bool scale, bool maximize)
{
   menu* men = (menu*)menuButton->men;
   canClose = close;
   men->setItemAvailable(WINDOW_MENU_CLOSE, canClose);
   canMove = move;
   canScale = scale;
   canMaximize = maximize;
}

/*********************************************************************
 *                              isModal                              *
 *********************************************************************/
bool window::isModal()
{
   return(modal);
}

/*********************************************************************
 *                              setModal                             *
 *********************************************************************/
void window::setModal()
{
   modal = true;
}

/*********************************************************************
 *                                hide                               *
 *********************************************************************/
void window::hide()
{
   visible = false;
}

/*********************************************************************
 *                                 show                              *
 *********************************************************************/
void window::show()
{
   visible = true;
}

/*********************************************************************
 *                             isVisible                             *
 *********************************************************************/
bool window::isVisible()
{
   return(visible);
}

/*********************************************************************
 *                              changed                              *
 *********************************************************************/
bool window::changed()
{
   bool result = false;

   /* Verify the Window */
   result |= guiObject::changed();

   /* Verify some Object Change. Must verify all to avoid
    * not needed redraws at next frame.  */
   result |= objects->changed();

   /* Reset the Flag */
   return(result);
}

/*********************************************************************
 *                               doMove                              *
 *********************************************************************/
int window::doMove(SDL_Surface* backGround, int xinic, int yinic, int Mbotao)
{
   int dx = x2 - x1;        /* Width */
   int dy = y2 - y1;        /* Heigh */

   int x,y;         /* Mouse Coordinates */

   if(Mbotao & SDL_BUTTON(1))
   {
      /* Calculate the translation from window (0,0) coordinate */
      x = xinic - difx; 
      y = yinic - dify;

      /* Verify max bounds */
      if(x + dx > SCREEN_X-1)
      { 
         x -= (x + dx) - (SCREEN_X-1);
      }
      if(y + dy > SCREEN_Y-1) 
      {
         y -= (y+dy) - (SCREEN_Y-1);
      }

      /* Verify min bounds */
      if(x < 0)
      {
         x = 0;
      }
      if(y < 0)
      {
         y = 0;
      }

      /* Set the new values */
      x1 = x;
      x2 = x + dx;
      y1 = y;
      y2 = y+ dy;
   }
   else 
   {
      /* No more pressed the mouse button. Finished */
      return(0);
   }
   /* mouse button still pressed, continue to move */
   return(1);
}



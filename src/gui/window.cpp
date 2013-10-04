/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "window.h"
#include "menu.h"
#include "farsoopts.h"

#include <iostream>
using namespace std;
using namespace Farso;

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
WindowList::WindowList()
{
   activeWindow = NULL;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
WindowList::~WindowList()
{
   clearList();
}

/**************************************************************
 *                          clearList                         *
 **************************************************************/
void WindowList::clearList()
{
   std::list<Window*>::iterator it;
   Window* obj;

   for(it=list.begin(); it != list.end(); it++)
   {
      obj = *it;
      delete(obj);
   }
   list.clear();
}

/*********************************************************************
 *                           insertWindow                            *
 *********************************************************************/
Window* WindowList::insertWindow(int xa,int ya,int xb,int yb,string text,
      bool empty)
{
   Window* novo;

   /* Create and insert the Window on the list */
   novo = new Window(xa,ya,xb,yb,text,this, empty);
   list.push_back(novo);

   return(novo);
}

/*********************************************************************
 *                         getModalWindow                            *
 *********************************************************************/
Window* WindowList::getModalWindow()
{
   Window* modalW = NULL;

   std::list<Window*>::iterator it;

   /* Get the last (most recent) modal Window */
   for(it=list.begin(); it != list.end(); it++)
   {
      if((*it)->isModal())
      {
         modalW = (*it);
      }
   }

   return(modalW);
}

/*********************************************************************
 *                           removeWindow                            *
 *********************************************************************/
void WindowList::removeWindow(Window *jan)
{
   /* Verify ActiveWindow Pointer */
   if(jan == activeWindow)
   {
      activeWindow = NULL;
      list.remove(jan);
      if((list.size() > 0))
      {
         Window* j = list.back();
         j->activate();
      }
   }
   else
   {
      list.remove(jan);
   }
   delete(jan);
}

/*********************************************************************
 *                         setActiveWindow                           *
 *********************************************************************/
void WindowList::setActiveWindow(Window* jan)
{
   activeWindow = jan;
   /* Reinsert it at the end, to be last draw (aka on top)
    * after de-activated */
   list.remove(jan);
   list.push_back(jan);
}

/*********************************************************************
 *                             Constructor                           *
 *********************************************************************/
Window::Window(int xa, int ya, int xb, int yb, string title, WindowList* list,
      bool empty):GuiObject(NULL)
{
   Font fnt;
   Options opt;

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
   objects = new GuiList(x2-x1, y2-y1, surface, hasSelfDraw);


   /* Create title bar things */
   if(hasSelfDraw)
   {
      /* Create Menu Button */
      menuButton = objects->insertButton(3,3,13,12,"-",0);
      menuButton->men = new Menu(0,0,x2-x1, y2-y1, surface);
      Menu* men = (Menu*) menuButton->men;
      men->insertItem(opt.getMaximizeLabel(), opt.getMaximizeIcon(), 0);
      men->insertItem("-",0);
      men->insertItem(opt.getCloseLabel(), opt.getCloseIcon(), 1);

      /* Create Close Button */
      closeButton = objects->insertButton(14,3,24,12,
            fnt.createUnicode(0x25CF),0);
      closeButton->defineFont(opt.getDefaultFont(), 10);

      /* Create Minimize Maximize Button */
      minMaxButton = objects->insertButton(25,3,35,12,
            fnt.createUnicode(0x25B2),0);
      minMaxButton->defineFont(opt.getDefaultFont(), 8);
   }
   else
   {
      /* No title bar */
      menuButton = NULL;
      closeButton = NULL;
      minMaxButton = NULL;
   }

   /* Set the object type as WINDOW! */
   type = Farso::OBJECT_WINDOW;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
Window::~Window()
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
void Window::draw()
{
   draw(-1,-1, true);
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void Window::draw(int mouseX, int mouseY, bool drawBar)
{
   int dx = x2 - x1;
   int dy = y2 - y1;

   if(hasSelfDraw)
   {
      color_Set(colors.colorWindow.R, colors.colorWindow.G,
            colors.colorWindow.B, colors.colorWindow.A);
      rectangle_Fill(surface, 3,3,dx-3,dy-3);
      color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
            colors.colorCont[0].B, colors.colorCont[0].A);
      rectangle_Draw(surface,0,0,dx-1,dy-1);
      color_Set(colors.colorButton.R, colors.colorButton.G, 
            colors.colorButton.B, colors.colorButton.A);
      rectangle_Draw(surface,1,1,dx-2,dy-2);
      color_Set(colors.colorCont[2].R, colors.colorCont[2].G,
            colors.colorCont[2].B, colors.colorCont[2].A);
      line_Draw(surface,2,13,dx-4,13);
      color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
            colors.colorCont[0].B, colors.colorCont[0].A);
      rectangle_2Colors(surface,2,2,dx-3,dy-3,colors.colorCont[2].R,
            colors.colorCont[2].G,colors.colorCont[2].B,
            colors.colorCont[2].A);


      /* Draw the bar */
      WindowList* lst = (WindowList*)intList;

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
void Window::drawInactiveBar()
{
   Options opt;
   Font fnt;
   int dx = x2-x1;

   /* Redraw All Window, removing the mouse from it */
   draw(-1,-1, false);

   if(hasSelfDraw)
   {
      /* Redraw the Inactive Bar */
      color_Set(colors.colorWindow.R, colors.colorWindow.G,
            colors.colorWindow.B, colors.colorWindow.A);
      rectangle_Fill(surface,36,3,dx-3,12);
      color_Set(colors.colorBar.R+100, colors.colorBar.G,
            colors.colorBar.B, colors.colorBar.A);
      fnt.defineFont(opt.getDefaultFont(),10);
      fnt.defineFontAlign(Font::ALIGN_LEFT);
      fnt.defineFontStyle(Font::STYLE_NORMAL);
      fnt.write(surface,39,1,text);
   }
   setChanged();
}

/*********************************************************************
 *                           drawActiveBar                           *
 *********************************************************************/
void Window::drawActiveBar()
{
   Options opt;
   Font fnt;
   int dx = x2-x1;

   if(hasSelfDraw)
   {
      color_Set(colors.colorBar.R, colors.colorBar.G,
            colors.colorBar.B, colors.colorBar.A);
      rectangle_Fill(surface,36,3,dx-3,12);
      color_Set(colors.colorText.R, colors.colorText.G,
            colors.colorText.B, colors.colorText.A);
      fnt.defineFont(opt.getDefaultFont(),10);
      fnt.defineFontAlign(Font::ALIGN_LEFT);
      fnt.defineFontStyle(Font::STYLE_NORMAL);
      fnt.write(surface,39,1,text);
      setChanged();
   }
}

/*********************************************************************
 *                               flush                               *
 *********************************************************************/
void Window::flush()
{
   setTextureRGBA(surface, texture, false, GL_RGBA);
}

/*********************************************************************
 *                              render                               *
 *********************************************************************/
void Window::render(float depth)
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
void Window::activate()
{
  WindowList *ljan = (WindowList*) intList;
  if (ljan->getActiveWindow() != NULL)
  {
     if(ljan->getActiveWindow()->isModal())
     {
        /* The current active Window is a modal one,
         * so it continue as the active Window! */
        return;
     }

     /* Draw the Inactive TitleBar to the current active Window */
     ljan->getActiveWindow()->drawInactiveBar();
  }

  /* Set this Window as the new active one */
  ljan->setActiveWindow(this);

  /* And obviously, draw its active bar */
  drawActiveBar();
}

/*********************************************************************
 *                               open                                *
 *********************************************************************/
void Window::open()
{
   activate();
   draw(0,0);
   setChanged();
}

/*********************************************************************
 *                          setAttributes                            *
 *********************************************************************/
void Window::setAttributes(bool close, bool move, bool scale, bool maximize)
{
   if(hasSelfDraw)
   {
      Menu* men = (Menu*)menuButton->men;
      canClose = close;
      men->setItemAvailable(WINDOW_MENU_CLOSE, canClose);
      canMove = move;
      canScale = scale;
      canMaximize = maximize;
   }
}

/*********************************************************************
 *                              isModal                              *
 *********************************************************************/
bool Window::isModal()
{
   return(modal);
}

/*********************************************************************
 *                              setModal                             *
 *********************************************************************/
void Window::setModal()
{
   modal = true;
}

/*********************************************************************
 *                                hide                               *
 *********************************************************************/
void Window::hide()
{
   visible = false;
}

/*********************************************************************
 *                                 show                              *
 *********************************************************************/
void Window::show()
{
   visible = true;
}

/*********************************************************************
 *                             isVisible                             *
 *********************************************************************/
bool Window::isVisible()
{
   return(visible);
}

/*********************************************************************
 *                              changed                              *
 *********************************************************************/
bool Window::changed()
{
   bool result = false;

   /* Verify the Window */
   result |= GuiObject::changed();

   /* Verify some Object Change. Must verify all to avoid
    * not needed redraws at next frame.  */
   result |= objects->changed();

   /* Reset the Flag */
   return(result);
}

/*********************************************************************
 *                               doMove                              *
 *********************************************************************/
int Window::doMove(SDL_Surface* backGround, int xinic, int yinic, int Mbotao)
{
   int dx = x2 - x1;        /* Width */
   int dy = y2 - y1;        /* Heigh */

   int x,y;         /* Mouse Coordinates */

   if(Mbotao & SDL_BUTTON(1))
   {
      /* Calculate the translation from Window (0,0) coordinate */
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



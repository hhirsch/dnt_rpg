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

#include "tabbox.h"

#include "guilist.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                  tabObj                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
tabObj::tabObj(SDL_Surface* surface)
{
   list = new guiList(surface);
   title = "";
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
tabObj::~tabObj()
{
   delete(list);
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                  tabBox                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
tabBox::tabBox(int xa, int ya, int xb, int yb, SDL_Surface* screen)
       :guiObject(screen)
{
   /* Define guiObject type */
   type = FARSO_OBJECT_TAB_BOX;
   /* Define Position */
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;

   /* And nullify the list! */
   active = NULL;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
tabBox::~tabBox()
{
   clearList();
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void tabBox::freeElement(dntListElement* obj)
{
   tabObj* tb = (tabObj*)obj;
   delete(tb);
}

/***********************************************************************
 *                             getActiveList                           *
 ***********************************************************************/
guiList* tabBox::getActiveList()
{
   if(active)
   {
      return(active->list);
   }

   return(NULL);
}

/***********************************************************************
 *                            getActiveTitle                           *
 ***********************************************************************/
string tabBox::getActiveTitle()
{
   if(active)
   {
      return(active->title);
   }
   return("");
}

/***********************************************************************
 *                               getObject                             *
 ***********************************************************************/
tabObj* tabBox::getObject(int opt)
{
   int i;
   tabObj* obj = (tabObj*)first;

   /* Search for the element */
   if( (opt < total) && (opt >= 0))
   {
      for(i = 0; i < opt; i++)
      {
         obj = (tabObj*)obj->getNext();
      }
      return(obj);
   }

   return(NULL);
}

/***********************************************************************
 *                                getList                              *
 ***********************************************************************/
guiList* tabBox::getList(int opt)
{
   tabObj* obj = getObject(opt);

   if(obj != NULL)
   {
      return(obj->list);
   }

   return(NULL);
}

/***********************************************************************
 *                                getList                              *
 ***********************************************************************/
guiList* tabBox::getList(string title)
{
   int i;
   tabObj* obj = (tabObj*)first;

   /* Search for the element */
   for(i = 0; i < total; i++)
   {
      if(obj->title == title)
      {
         /* Found it! */
         return(obj->list);
      }
      obj = (tabObj*)obj->getNext();
   }

   return(NULL);
}

/***********************************************************************
 *                              insertOption                           *
 ***********************************************************************/
guiList* tabBox::insertOption(string title)
{
   tabObj* obj;

   /* Only insert if not found! */
   if(getList(title) == NULL)
   {
      obj = new tabObj(wSurface);
      obj->title = title;

      /* Insert it */
      insert(obj);

      /* Define the active one */
      active = obj;

      return(obj->list);
   }

   return(getList(title));
}

/***********************************************************************
 *                                 draw                                *
 ***********************************************************************/
void tabBox::draw()
{
   if(total == 0)
   {
     return; //nothing to draw
   }

   int i;
   int incX = (x2-x1) / total;
   int posX = 0;
   int endPos = 0;
   tabObj* obj;
   farso_colors colors;

   /* Set the font */
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);

   /* Draw Limitators */
   color_Set(colors.colorButton.R, colors.colorButton.G,
             colors.colorButton.B, colors.colorButton.A);
   rectangle_Fill(wSurface, x1, y1, x2, y2);
   color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
             colors.colorCont[0].B, colors.colorCont[0].A);
   rectangle_2Colors(wSurface, x1, y1+18, x2, y2,
                     colors.colorCont[1].R, colors.colorCont[1].G,
                     colors.colorCont[1].B, colors.colorCont[1].A);

   /* Draw All Titles */
   obj = (tabObj*)first;
   posX = x1;
   for(i = 0; i < total; i++)
   {
      /* Define Final position */
      if(i != total-1)
      {
         endPos = posX+incX-1;
      }
      else
      {
         endPos = x2;
      }

      if(obj == active)
      {
         /* Draw some contorn */
         color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
                   colors.colorCont[0].B, colors.colorCont[0].A);
         line_Draw(wSurface, posX, y1, posX, y1+18);
         line_Draw(wSurface, posX, y1, endPos, y1);
         color_Set(colors.colorCont[1].R, colors.colorCont[1].G,
                   colors.colorCont[1].B, colors.colorCont[1].A);
         line_Draw(wSurface, endPos, y1, endPos, y1+18);

         /* Without Line separator bellow */
         color_Set(colors.colorButton.R, colors.colorButton.G,
                   colors.colorButton.B, colors.colorButton.A);
         rectangle_Fill(wSurface, posX+1, y1+17, endPos-1, y1+19);
      }
      else
      {
         /* Draw some pressed contorn */
         color_Set(colors.colorCont[1].R, colors.colorCont[1].G,
                   colors.colorCont[1].B, colors.colorCont[1].A);
         rectangle_2Colors(wSurface, posX, y1, endPos, y1+17,
                           colors.colorCont[0].R, colors.colorCont[0].G,
                           colors.colorCont[0].B, colors.colorCont[0].A);
      }
      

      /* Draw the title text */
      color_Set(colors.colorText.R, colors.colorText.G,
                colors.colorText.B, colors.colorText.A);
      fnt.write(wSurface, posX+2, y1+2, obj->title, posX, y1, endPos, y1+17);

      /* Next! */
      posX += incX;
      obj = (tabObj*)obj->getNext();
   }

   /* Draw all active objects */
   if(active)
   {
      active->list->draw();
   }
}

/***********************************************************************
 *                              verifyChanges                          *
 ***********************************************************************/
bool tabBox::verifyChanges(int mouseX, int mouseY)
{
   if(total == 0)
   {
      return(false);//nothing to verify
   }
   int incX = (x2-x1) / total;
   int cur = 0;
   tabObj* obj = NULL;

   if(isMouseAt(x1, y1, x2, y1+18, mouseX, mouseY))
   {
      /* Clicked at the title bar! */
      cur = (mouseX-x1) / incX;
      obj = getObject(cur);
      if( (obj != NULL) && (obj != active))
      {
         active = obj;
         draw();
         setChanged();
         return(true);
      }
   }
   return(false);
}



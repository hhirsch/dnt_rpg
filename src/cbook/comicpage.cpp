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

#include "comicpage.h"
#include "../etc/dirs.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"
#include "../engine/util.h"

#include <iostream>

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif

using namespace std;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
comicPageList::comicPageList(): dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
comicPageList::~comicPageList()
{
   clearList();
}

/***********************************************************************
 *                            freeElement                              *
 ***********************************************************************/
void comicPageList::freeElement(dntListElement* obj)
{
   comicPage* page = (comicPage*)obj;
   delete(page);
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
comicPage::comicPage()
{
   texture = NULL;
   glGenTextures(1, &tex);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
comicPage::~comicPage()
{
   /* Free texture */
   glDeleteTextures(1,&tex);
   if(texture != NULL)
   {
      SDL_FreeSurface(texture);
   }
}

/***********************************************************************
 *                            flushTexture                             *
 ***********************************************************************/
void comicPage::flushTexture()
{
   if(texture != NULL)
   {
      setTexture(texture, tex);
   }
}

/***********************************************************************
 *                             getFirstBox                             *
 ***********************************************************************/
comicBox* comicPage::getFirstBox()
{
   return((comicBox*)boxes.getFirst());
}

/***********************************************************************
 *                            getTotalBoxes                            *
 ***********************************************************************/
int comicPage::getTotalBoxes()
{
   return(boxes.getTotal());
}

/***********************************************************************
 *                               getWidth                              *
 ***********************************************************************/
int comicPage::getWidth()
{
   if(texture)
   {
      return(texture->w);
   }
   return(0);
}

/***********************************************************************
 *                               getHeight                             *
 ***********************************************************************/
int comicPage::getHeight()
{
   if(texture)
   {
      return(texture->h);
   }
   return(0);
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void comicPage::render()
{
   int i;
   comicBox* box = (comicBox*)boxes.getFirst();

   /* Calculate scale ratio */
   GLfloat ratio = 1.0f;
   if(texture)
   { 
      ratio = (float)SCREEN_Y / (float)texture->h;
   }

   glColor4f(1.0f,1.0f,1.0f,1.0f);

   glPushMatrix();
   glDisable(GL_DEPTH_TEST);

   /* Center the Page on screen */
   if(texture)
   {
      glTranslatef(((SCREEN_X/2.0) - ratio*(texture->w/2.0)),0.0,0.0);

      /* Render Blank Page */
      GLfloat midX = ratio * texture->w;
      GLfloat midY = ratio * texture->h;
      glBegin(GL_QUADS);
         glVertex2f(0, 0);
         glVertex2f(midX, 0);
         glVertex2f(midX, midY);
         glVertex2f(0, midY);
      glEnd();

   }

   /* Render Page Boxes */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);
   for(i = 0; i < boxes.getTotal(); i++)
   {
      if(box->getStatus() != COMIC_BOX_STATUS_INACTIVE)
      {
         box->render();
      }
      box = (comicBox*)box->getNext();
   }
   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
}

/***********************************************************************
 *                              insertBox                              *
 ***********************************************************************/
void comicPage::insertBox(comicBox* box)
{
   boxes.insert(box);
}


/***********************************************************************
 *                             insertText                              *
 ***********************************************************************/
void comicPage::insertText(int x1, int y1, int x2, int y2, string text)
{
   insertText(x1, y1, x2, y2, text, 16, DNT_FONT_STYLE_NORMAL, 0, 0, 0);
}

/***********************************************************************
 *                             insertText                              *
 ***********************************************************************/
void comicPage::insertText(int x1, int y1, int x2, int y2, string text,
                           int fontSize, int fontStyle, 
                           int R, int G, int B)
{
   if(texture != NULL)
   {
      dntFont fnt;
      dirs dir;
      fnt.defineFont(dir.getRealFile(DNT_FONT_TIMES), fontSize);
      fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
      fnt.defineFontStyle(fontStyle);

      color_Set(R, G, B, 255);
      fnt.write(texture, x1+1,y1+2, text, x1,y1,x2,y2);

      fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   }
   else
   {
      cerr << "Warning: tried to define comic text '" << text 
           << "' without texture!" << endl;
   }
}


/***********************************************************************
 *                          defineTexture                              *
 ***********************************************************************/
bool comicPage::defineTexture(string textureFile)
{
   dirs dir;
   if(texture != NULL)
   {
      SDL_FreeSurface(texture);
   }

   texture = IMG_Load(dir.getRealFile(textureFile).c_str());

   if(!texture)
   {
      cerr << "Error on loading image: '" << textureFile << "'" << endl;
      return(false);
   }

   return(true);
}


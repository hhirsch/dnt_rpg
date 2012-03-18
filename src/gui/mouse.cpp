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

#include "mouse.h"
#include "farsoopts.h"
#include "draw.h"
#include "dntfont.h"
#include "color.h"

#include <iostream>
using namespace std;
using namespace Farso;

/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
SingleMouseCursor::SingleMouseCursor()
{
   hotSpot[0] = 0;
   hotSpot[1] = 0;
   size[0] = 0;
   size[1] = 0;
   prop[0] = 0;
   prop[1] = 0;
   scaleFactor = 0;
   fileName = "";

   /* Generate OpenGL Textures */
   glGenTextures(1, &texture);

}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
SingleMouseCursor::~SingleMouseCursor()
{
   /* Clear texture */
   glDeleteTextures(1, &texture);
}

/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
MouseCursor::MouseCursor()
{
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
MouseCursor::~MouseCursor()
{
}

/*****************************************************************
 *                             init                              *
 *****************************************************************/
void MouseCursor::init()
{
   Options opt;
   
   /* Set font to default */
   font = opt.getDefaultFont();
   
   /* Disable HW Mouse cursos */
   SDL_ShowCursor(SDL_DISABLE);
   
   /* Load Default Cursor */
   current = loadCursor(opt.getDefaultCursor(),0,0); 
  
   /* Text over */
   textOver = "";
   glGenTextures(1, &textOverTexture);
   textOverWidth=0;
   textOverHeight=0;
   textOverInit = 0;
   
   /* Create the place-holder cursor for SDL_Images* */
   SingleMouseCursor* mCursor = new SingleMouseCursor();
   mCursor->fileName = MOUSE_CURSOR_USER_IMAGE;
   cursors.push_back(mCursor);
}

/*****************************************************************
 *                              end                              *
 *****************************************************************/
void MouseCursor::finish()
{
   std::list<SingleMouseCursor*>::iterator it;
   SingleMouseCursor* cur;
   /* Delete all cursors */
   for(it = cursors.begin(); it != cursors.end(); it++)
   {
      cur = (*it);
      /* Delete Cursor */
      delete(cur);
   }
   cursors.clear();

   /* Finally, finish the textOver texture */
   glDeleteTextures(1, &textOverTexture);
}

/*****************************************************************
 *                         Load Cursor                           *
 *****************************************************************/
SingleMouseCursor* MouseCursor::loadCursor(string fileName, int hX, int hY)
{
   SingleMouseCursor* cur = new SingleMouseCursor();
   cur->hotSpot[0] = hX;
   cur->hotSpot[1] = hY;
   cur->fileName = fileName;

   /* Generate OpenGL Textures */
   glGenTextures(1, &cur->texture);

   SDL_Surface* img = IMG_Load(fileName.c_str());
   if(img)
   {
      setTexture(img, cur->texture, true);
      cur->size[0] = img->w;
      cur->size[1] = img->h;
      cur->prop[0] = (float)(img->w) / (float)smallestPowerOfTwo(img->w);
      cur->prop[1] = (float)(img->h) / (float)smallestPowerOfTwo(img->h);
      cur->scaleFactor = 32.0f / (float)(img->w);
      SDL_FreeSurface(img);
      cursors.push_back(cur);
      return(cur);
   }
   else
   {
      cerr << "Can't load mouse MouseCursor: " << fileName << endl;
      delete(cur);
   }

   return(NULL);
}

/*****************************************************************
 *                             search                            *
 *****************************************************************/
SingleMouseCursor* MouseCursor::search(std::string fileName)
{
   std::list<SingleMouseCursor*>::iterator it;

   /* Search through all cursors */
   for(it = cursors.begin(); it != cursors.end(); it++)
   {
      if((*it)->fileName == fileName)
      {
         /* Found! */
         return((*it));
      }
   }

   /* Not found */
   return(NULL);
}

/*****************************************************************
 *                               set                             *
 *****************************************************************/
void MouseCursor::set(std::string imageFile, int spotX, int spotY)
{
   SingleMouseCursor* cur = search(imageFile);
   if(!cur)
   {
      cur = loadCursor(imageFile, spotX, spotY);
      if(!cur)
      {
         /* Couldn't Load! */
         return;
      }
   }
   else
   {
      cur->hotSpot[0] = spotX;
      cur->hotSpot[1] = spotY;
   }
   
   current = cur;
}

/*****************************************************************
 *                               hide                            *
 *****************************************************************/
void MouseCursor::hide()
{
   current = NULL;
}

/*****************************************************************
 *                              set                              *
 *****************************************************************/
void MouseCursor::set(SDL_Surface* img)
{
   /* Get the place-holder cursor */
   SingleMouseCursor* cur = search(MOUSE_CURSOR_USER_IMAGE);

   if(cur)
   {
      /* Load the image to a texture */
      setTexture(img, cur->texture);
      cur->size[0] = img->w;
      cur->size[1] = img->h;
      cur->prop[0] = (float)(img->w) / (float)smallestPowerOfTwo(img->w);
      cur->prop[1] = (float)(img->h) / (float)smallestPowerOfTwo(img->h);
      cur->hotSpot[0] = 0;
      cur->hotSpot[1] = 0;
      cur->scaleFactor = 1.0f;
      current = cur;
   }
}

/*****************************************************************
 *                               get                             *
 *****************************************************************/
std::string MouseCursor::get()
{
   if(current)
   {
      return(current->fileName);
   }

   return(MOUSE_CURSOR_NONE);
}

/*****************************************************************
 *                      setTextOverFont                          *
 *****************************************************************/
void MouseCursor::setTextOverFont(std::string f)
{
   font = f;
}

/*****************************************************************
 *                        setTextOver                            *
 *****************************************************************/
void MouseCursor::setTextOver(string txt)
{
   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
 
   /* Rese the timer */
   textOverInit = SDL_GetTicks();

   /* Reset things if needed */
   if(txt != textOver)
   {
      textOver = txt;
      if(!textOver.empty())
      {
         /* Recreate the texture */
         Font fnt;
         Colors curColor;
         
         fnt.defineFont(font, 12);
         fnt.defineFontAlign(Font::ALIGN_CENTER);
         fnt.defineFontStyle(Font::STYLE_NORMAL);
         textOverHeight = fnt.getHeight()+4;
         textOverWidth = fnt.getStringWidth(textOver)+8;

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

         SDL_Surface* s = SDL_CreateRGBSurface(SDL_SWSURFACE,
                  smallestPowerOfTwo(textOverWidth),
                  smallestPowerOfTwo(textOverHeight),
                  32, rmask, gmask, bmask, amask);
         
         /* Do some details */
         color_Set(curColor.colorButton.R, curColor.colorButton.G, 
               curColor.colorButton.B, 255);
         rectangle_Fill(s,1,1,textOverWidth-2,textOverHeight-2);
         color_Set(curColor.colorCont[0].R, curColor.colorCont[0].G, 
               curColor.colorCont[0].B, 255);
         rectangle_Oval(s,0,0,textOverWidth-1, textOverHeight-1, 
               curColor.colorCont[0].R, curColor.colorCont[0].G,
               curColor.colorCont[0].B, 255);

         /* Write the text */
         color_Set(255,255,255,255);
         fnt.defineFontAlign(Font::ALIGN_CENTER);
         fnt.write(s, 2, 3,
               textOver.c_str(),
               0, 0, textOverWidth, textOverHeight);


         glGenTextures(1,&textOverTexture);
         setTextureRGBA(s, textOverTexture);

         SDL_FreeSurface(s);
      }
   }
}

/*****************************************************************
 *                             Draw                              *
 *****************************************************************/
void MouseCursor::draw(int mouseX, int mouseY, float angle,
                  float scaleX, float scaleY, float scaleZ)
{
   if(!current)
   {
      return;
   }

   glPushAttrib(GL_ENABLE_BIT);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glDisable(GL_DEPTH_TEST);

   /* Render the MouseCursor */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, current->texture);
   glPushMatrix();
      glTranslatef(mouseX+2-current->hotSpot[0], 
                   SCREEN_Y - (mouseY+2-current->hotSpot[1]), 
                   0.1);
      glRotatef(angle, 0, 0, 1);
      glScalef(scaleX, scaleY, scaleZ);
      glScalef(current->scaleFactor, current->scaleFactor, 
               current->scaleFactor);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f);
         glVertex2f(0.0f, 0.0f);
         glTexCoord2f(0.0f, current->prop[0]);
         glVertex2f(0.0f, -current->size[1]);
         glTexCoord2f(current->prop[0], current->prop[1]);
         glVertex2f(current->size[0], -current->size[1]);
         glTexCoord2f(current->prop[0], 0.0f);
         glVertex2f(current->size[0], 0.0f);
      glEnd();
   glPopMatrix();

   /* Render the text over MouseCursor, if defined */
   if(!textOver.empty())
   {
      int tx1, ty1;
      tx1 = mouseX - current->hotSpot[0];
      ty1 = mouseY - current->hotSpot[1]-textOverHeight-2;

      /* Verify off screen */
      if(ty1 < 0)
      {
         ty1 = 0;
      }
      else if(ty1+textOverHeight > SCREEN_Y-1)
      {
         ty1 = (SCREEN_Y-1) - textOverHeight;
      }
      if(tx1 < 0)
      {
         tx1 = 0;
      }
      else if(tx1+textOverWidth > SCREEN_X-1)
      {
         tx1 = (SCREEN_X-1) - textOverWidth;
      }

      glPushMatrix();
         textureToScreen(textOverTexture, tx1, ty1, textOverWidth+tx1, 
               ty1+textOverHeight, textOverWidth, textOverHeight);
      glPopMatrix();

      /* Verify max display time */
      if((SDL_GetTicks() - textOverInit) > MOUSE_CURSOR_MAX_TEXT_OVER_TIME)
      {
         /* Expired */
         setTextOver("");
      }
   }

   glPopAttrib();
}

/*****************************************************************
 *                         Static Members                        *
 *****************************************************************/
SingleMouseCursor* MouseCursor::current;
std::list<SingleMouseCursor*> MouseCursor::cursors;
std::string MouseCursor::textOver="";
GLuint MouseCursor::textOverTexture=0;
int MouseCursor::textOverWidth=0;
int MouseCursor::textOverHeight=0;
Uint32 MouseCursor::textOverInit=0;
std::string MouseCursor::font="";

/**********************************************************
 *                        isMouseIn                       *
 **********************************************************/
bool Farso::isMouseAt(int x1,int y1,int x2,int y2, int mouseX, int mouseY)
{
   return( ( (mouseX>=x1) && (mouseX<=x2) ) && 
           ( (mouseY>=y1) && (mouseY<=y2) ) );
}



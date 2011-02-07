/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "cursor.h"

#include "util.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
cursor::~cursor()
{
}

/*****************************************************************
 *                             init                              *
 *****************************************************************/
void cursor::init()
{
   dirs dir;
   /* Generate OpenGL Textures */
   glGenTextures(CURSOR_TOTAL, texture);

   /* Load Cursors */
   loadCursor(dir.getRealFile("cursors/Walk.png"), CURSOR_WALK);
   hotSpot[CURSOR_WALK][0] = 0;
   hotSpot[CURSOR_WALK][1] = 0;
   loadCursor(dir.getRealFile("cursors/Attack.png"), CURSOR_ATTACK);
   hotSpot[CURSOR_ATTACK][0] = 1;
   hotSpot[CURSOR_ATTACK][1] = 0;
   loadCursor(dir.getRealFile("cursors/Defend.png"), CURSOR_DEFEND);
   hotSpot[CURSOR_DEFEND][0] = 0;
   hotSpot[CURSOR_DEFEND][1] = 0;
   loadCursor(dir.getRealFile("cursors/MapTravel.png"), CURSOR_MAPTRAVEL);
   hotSpot[CURSOR_MAPTRAVEL][0] = 15;
   hotSpot[CURSOR_MAPTRAVEL][1] = 15;
   loadCursor(dir.getRealFile("cursors/talk.png"), CURSOR_TALK);
   hotSpot[CURSOR_TALK][0] = 15;
   hotSpot[CURSOR_TALK][1] = 15;
   loadCursor(dir.getRealFile("cursors/Get.png"), CURSOR_GET);
   hotSpot[CURSOR_GET][0] = 5;
   hotSpot[CURSOR_GET][1] = 2;
   loadCursor(dir.getRealFile("cursors/Inventory.png"), CURSOR_INVENTORY);
   hotSpot[CURSOR_INVENTORY][0] = 15;
   hotSpot[CURSOR_INVENTORY][1] = 15;
   loadCursor(dir.getRealFile("cursors/Door.png"), CURSOR_DOOR);
   hotSpot[CURSOR_DOOR][0] = 5;
   hotSpot[CURSOR_DOOR][1] = 0;
   loadCursor(dir.getRealFile("cursors/forbidden.png"), CURSOR_FORBIDDEN);
   hotSpot[CURSOR_FORBIDDEN][0] = 15;
   hotSpot[CURSOR_FORBIDDEN][1] = 15;
   loadCursor(dir.getRealFile("cursors/walk_cont.png"), CURSOR_WALK_CONT);
   hotSpot[CURSOR_WALK_CONT][0] = 0;
   hotSpot[CURSOR_WALK_CONT][1] = 0;
   loadCursor(dir.getRealFile("cursors/use.png"), CURSOR_USE);
   hotSpot[CURSOR_USE][0] = 15;
   hotSpot[CURSOR_USE][1] = 15;

   /* Text over */
   textOver = "";
   glGenTextures(1, &textOverTexture);
   textOverWidth=0;
   textOverHeight=0;
   textOverInit = 0;
   
   currentCursor = CURSOR_WALK;
}

/*****************************************************************
 *                              end                              *
 *****************************************************************/
void cursor::finish()
{
   /* Free Cursors Textures */
   glDeleteTextures(CURSOR_TOTAL,texture);
   glDeleteTextures(1,&textOverTexture);
}

/*****************************************************************
 *                         Load Cursor                           *
 *****************************************************************/
void cursor::loadCursor(string fileName, int index)
{
   SDL_Surface* img = IMG_Load(fileName.c_str());
   if(img)
   {
      setTexture(img, texture[index], true);
      sizeX[index] = img->w;
      sizeY[index] = img->h;
      propX[index] = (float)(img->w) / (float)smallestPowerOfTwo(img->w);
      propY[index] = (float)(img->h) / (float)smallestPowerOfTwo(img->h);
      scaleFactor[index] = 32.0f / (float)(img->w);
      SDL_FreeSurface(img);
   }
   else
   {
      cerr << "Can't load mouse cursor: " << fileName << endl;
   }
}

/*****************************************************************
 *                               set                             *
 *****************************************************************/
void cursor::set(int nCursor)
{
   currentCursor = nCursor;
}

/*****************************************************************
 *                              set                              *
 *****************************************************************/
void cursor::set(SDL_Surface* img)
{
   /* Load the image to a texture */
   setTexture(img, texture[CURSOR_USER_IMAGE]);
   sizeX[CURSOR_USER_IMAGE] = img->w;
   sizeY[CURSOR_USER_IMAGE] = img->h;
   propX[CURSOR_USER_IMAGE] = (float)(img->w) / 
                              (float)smallestPowerOfTwo(img->w);
   propY[CURSOR_USER_IMAGE] = (float)(img->h) / 
                              (float)smallestPowerOfTwo(img->h);
   hotSpot[CURSOR_USER_IMAGE][0] = 0;
   hotSpot[CURSOR_USER_IMAGE][1] = 0;
   scaleFactor[CURSOR_USER_IMAGE] = 1.0f;
   currentCursor = CURSOR_USER_IMAGE;
}

/*****************************************************************
 *                               get                             *
 *****************************************************************/
int cursor::get()
{
   return(currentCursor);
}

/*****************************************************************
 *                      setTextOver                              *
 *****************************************************************/
void cursor::setTextOver(string txt)
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
         dntFont fnt;
         farso_colors curColor;
         
         fnt.defineFont(DNT_FONT_PALLADIO, 12);
         fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
         fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
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
               curColor.colorButton.B, curColor.colorButton.A);
         rectangle_Fill(s,1,1,textOverWidth-2,textOverHeight-2);
         color_Set(curColor.colorCont[0].R, curColor.colorCont[0].G, 
               curColor.colorCont[0].B, curColor.colorCont[0].A);
         rectangle_Oval(s,0,0,textOverWidth-1, textOverHeight-1, 
               curColor.colorCont[0].R, curColor.colorCont[0].G,
               curColor.colorCont[0].B, curColor.colorCont[0].A);

         /* Write the text */
         color_Set(255,255,255,255);
         fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
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
void cursor::draw(int mouseX, int mouseY, float angle,
                  float scaleX, float scaleY, float scaleZ)
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glDisable(GL_DEPTH_TEST);

   /* Render the cursor */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture[currentCursor]);
   glPushMatrix();
      glTranslatef(mouseX+2-hotSpot[currentCursor][0], 
                   SCREEN_Y - (mouseY+2-hotSpot[currentCursor][0]), 
                   0.1);
      glRotatef(angle, 0, 0, 1);
      glScalef(scaleX, scaleY, scaleZ);
      glScalef(scaleFactor[currentCursor], scaleFactor[currentCursor], 
               scaleFactor[currentCursor]);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0f, 0.0f);
         glVertex2f(0.0f, 0.0f);
         glTexCoord2f(0.0f, propY[currentCursor]);
         glVertex2f(0.0f, -sizeY[currentCursor]);
         glTexCoord2f(propX[currentCursor], propY[currentCursor]);
         glVertex2f(sizeX[currentCursor], -sizeY[currentCursor]);
         glTexCoord2f(propX[currentCursor], 0.0f);
         glVertex2f(sizeX[currentCursor], 0.0f);
      glEnd();
   glPopMatrix();

   /* Render the text over cursor, if defined */
   if(!textOver.empty())
   {
      int tx1, ty1;
      tx1 = mouseX - hotSpot[currentCursor][0];
      ty1 = mouseY - hotSpot[currentCursor][1]-textOverHeight-2;

      glPushMatrix();
         textureToScreen(textOverTexture, tx1, ty1, textOverWidth+tx1, 
               ty1+textOverHeight, textOverWidth, textOverHeight);
      glPopMatrix();

      /* Verify max display time */
      if((SDL_GetTicks() - textOverInit) > DNT_CURSOR_MAX_TEXT_OVER_TIME)
      {
         /* Expired */
         setTextOver("");
      }
   }

   glEnable(GL_DEPTH_TEST);

   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
}

/*****************************************************************
 *                         Static Members                        *
 *****************************************************************/
GLuint cursor::texture[CURSOR_TOTAL]; /**< Cursors Textures */
float cursor::sizeX[CURSOR_TOTAL];    /**< Cursors Widths */
float cursor::sizeY[CURSOR_TOTAL];    /**< Cursors Heights */
float cursor::propX[CURSOR_TOTAL];    /**< X Proportion */
float cursor::propY[CURSOR_TOTAL];    /**< Y Proportion */
float cursor::hotSpot[CURSOR_TOTAL][2]; /**< HotSpot */
float cursor::scaleFactor[CURSOR_TOTAL]; 
int cursor::currentCursor;
string cursor::textOver;
GLuint cursor::textOverTexture;
int cursor::textOverWidth;
int cursor::textOverHeight;
Uint32 cursor::textOverInit;          /**< Time inited the display */

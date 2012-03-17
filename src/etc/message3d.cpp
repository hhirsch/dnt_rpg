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

#include "message3d.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"
#include "../engine/options.h"
#include "../engine/util.h"
#include "../etc/dirs.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                              message3d                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg)
{
   init(x, y, z, msg, 1.0f, 0.5f, 0.5f, DNT_FONT_ARIAL, 18);
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg,
                     GLfloat R, GLfloat G, GLfloat B)
{
   init(x,y,z,msg, R,G,B, DNT_FONT_ARIAL, 18);
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, std::string msg,
      GLfloat R, GLfloat G, GLfloat B, std::string font, int fontSize)
{
   init(x,y,z,msg, R,G,B, font, fontSize);
}

/***********************************************************
 *                           init                          *
 ***********************************************************/
void message3d::init(GLfloat x, GLfloat y, GLfloat z, string msg,
      GLfloat R, GLfloat G, GLfloat B, string font, int fontSize)
{
   dirs dir;
   dntFont fnt;

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

   previous = NULL;
   next = NULL;
   posX = x;
   posY = y;
   posZ = z;
   live = 0;
   message = msg;
   isStatic = false;

   /* Define the font and sizes */
   fnt.defineFont(dir.getRealFile(font), fontSize);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   size = fnt.getStringWidth(message) + 8;
   halfSize = (size / 2.0f);
   color_Set(0,0,0,255);
   SDL_Surface* s = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                         smallestPowerOfTwo(size),
                                         32,
                                         32, rmask, gmask, bmask, amask);
   SDL_Surface* bs = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                          smallestPowerOfTwo(size),
                                          32,
                                          32, rmask, gmask, bmask, amask);
   
   fnt.defineFontOutline(2);
   color_Set(0, 0, 0, 255);
   fnt.write(bs, 0, 0, message.c_str(), true);

   fnt.defineFontOutline(0);
   color_Set((int)floor(R*255),(int)floor(G*255),(int)floor(B*255),255);
   fnt.write(s, 2, 2, message.c_str(), true);
   
   SDL_BlitSurface(s, NULL, bs, NULL);
   
   
   glGenTextures(1,&messageTexture);
   setTextureRGBA(bs, messageTexture);

   SDL_FreeSurface(s);
   SDL_FreeSurface(bs);
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
message3d::~message3d()
{
   glDeleteTextures(1,&messageTexture);
}

/***********************************************************
 *                       setStatic                         *
 ***********************************************************/
void message3d::setStatic()
{
   isStatic = true;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            message3dList                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3dList::message3dList():dntList(DNT_LIST_TYPE_ADD_AT_BEGIN)
{
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
message3dList::~message3dList()
{
   clearList();
}

/***********************************************************
 *                        FreeElement                      *
 ***********************************************************/
void message3dList::freeElement(dntListElement* obj)
{
   message3d* m = (message3d*)obj;
   delete(m);
}


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                           messageController                           //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
messageController::messageController()
{
}

/***********************************************************
 *                        Destructor                       *
 ***********************************************************/
messageController::~messageController()
{
}

/***********************************************************
 *                            init                         *
 ***********************************************************/
void messageController::init()
{
   list = new message3dList();
}

/***********************************************************
 *                          finish                         *
 ***********************************************************/
void messageController::finish()
{
   if(list)
   {
      delete(list);
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
      bool yStatic)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg);
      list->insert(m);

      if(yStatic)
      {
         m->setStatic();
      }
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
      GLfloat R, GLfloat G, GLfloat B, bool yStatic)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg, R, G, B);
      list->insert(m);

      if(yStatic)
      {
         m->setStatic();
      }
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
      GLfloat R, GLfloat G, GLfloat B, std::string font, int fontSize, 
      bool yStatic)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg, R, G, B, font, fontSize);
      list->insert(m);

      if(yStatic)
      {
         m->setStatic();
      }
   }
}

/***********************************************************
 *                           draw                          *
 ***********************************************************/
void messageController::draw(GLdouble modelView[16], GLdouble projection[16], 
                             GLint viewPort[4])
{
   int i;
   int tot = list->getTotal();
   message3d* msg = (message3d*)list->getFirst();
   options option;

   GLdouble winX=0, winY=0, winZ=0;

   GLfloat factor = 1.0f;

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   for(i = 0; i < tot; i++)
   {
      gluProject(msg->posX, msg->posY, msg->posZ, modelView, projection, 
            viewPort, &winX, &winY, &winZ);
      
      /* Calculate Texture factor */
      factor = (float)(msg->size) / (float)(smallestPowerOfTwo(msg->size));

      /* Draw */
      glColor3f(1.0f, 1.0f, 1.0f);
      glBindTexture(GL_TEXTURE_2D, msg->messageTexture);
      glPushMatrix();
      glBegin(GL_QUADS);
         glTexCoord2f(0,1);
         glVertex2f(winX - msg->halfSize, winY - 16);
         glTexCoord2f(factor,1);
         glVertex2f(winX + msg->halfSize, winY - 16);
         glTexCoord2f(factor,0);
         glVertex2f(winX + msg->halfSize, winY + 16);
         glTexCoord2f(0,0);
         glVertex2f(winX - msg->halfSize, winY + 16);
      glEnd();
      glPopMatrix();

      msg->live++;
      if(!msg->isStatic)
      {
         msg->posY += 0.5f;
      }
      msg = (message3d*)msg->getNext();

      /* Verify if the message elapsed */
      if(((message3d*)msg->getPrevious())->live > MESSAGE3D_MAX_TIME)
      {
         list->remove(msg->getPrevious());
      }
   }

   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LIGHTING);
   glEnable(GL_FOG);
}

/***********************************************************
 *                   static members                        *
 ***********************************************************/
message3dList* messageController::list = NULL;


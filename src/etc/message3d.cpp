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

#include "message3d.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"
#include "../engine/options.h"

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
   init(x, y, z, msg, 1.0f, 0.5f, 0.5f);
}

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3d::message3d(GLfloat x, GLfloat y, GLfloat z, string msg,
                     GLfloat R, GLfloat G, GLfloat B)
{
   init(x,y,z,msg, R,G,B);
}

/***********************************************************
 *                           init                          *
 ***********************************************************/
void message3d::init(GLfloat x, GLfloat y, GLfloat z, string msg,
                     GLfloat R, GLfloat G, GLfloat B)
{
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

   /* Define the font and sizes */
   fnt.defineFont(DNT_FONT_ARIAL, 18);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   size = fnt.getStringWidth(message) + 8;
   halfSize = (size / 2.0f);
   color_Set(0,0,0,255);
   SDL_Surface* s = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                         smallestPowerOfTwo(size),
                                         32,
                                         32, rmask, gmask, bmask, amask);
   color_Set((int)floor(R*255),(int)floor(G*255),(int)floor(B*255),255);
   fnt.write(s, 0, 0, message.c_str(), true);


   glGenTextures(1,&messageTexture);
   setTextureRGBA(s, messageTexture);

   SDL_FreeSurface(s);
}

/***********************************************************
 *                       Destructor                        *
 ***********************************************************/
message3d::~message3d()
{
   glDeleteTextures(1,&messageTexture);
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                            message3dList                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/***********************************************************
 *                       Constructor                       *
 ***********************************************************/
message3dList::message3dList():dntList(DNT_LIST_TYPE_ADD_AT_END)
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
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg);
      list->insert(m);
   }
}

/***********************************************************
 *                           add                           *
 ***********************************************************/
void messageController::addMessage(GLfloat x, GLfloat y, GLfloat z, string msg,
                                   GLfloat R, GLfloat G, GLfloat B)
{
   if(!msg.empty())
   {
      message3d* m = new message3d(x,y,z,msg, R, G, B);
      list->insert(m);
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
      if(! (gluProject(msg->posX, msg->posY, msg->posZ,
                       modelView, projection, viewPort,
                       &winX, &winY, &winZ) ) )
      {
         /* Just for debug!  */
         printf("no!\n");
      }

      
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
      msg->posY += 0.5f;
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


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

#include "comicbox.h"
#include "../engine/util.h"
using namespace std;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBoxList::comicBoxList(): dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
comicBoxList::~comicBoxList()
{
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void comicBoxList::freeElement(dntListElement* obj)
{
   comicBox* box = (comicBox*)obj;
   delete(box);
}

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBox::comicBox(string t)
{
   int i;

   /* Set some default values */
   scaleFactorX = 1.0;
   scaleFactorY = 1.0;
   status = COMIC_BOX_STATUS_INACTIVE;
   effectType = COMIC_BOX_EFFECT_NONE;
   timeout = COMIC_BOX_DEFAULT_TIMEOUT;
 
   title = t;

   for(i = 0; i < 4; i++)
   {
      vertex[i][0] = 0.0;
      vertex[i][1] = 0.0;
   }

   pos[0] = 0;
   pos[1] = 0;

   scale[0] = 1.0;
   scale[1] = 1.0;

   angle[0] = 0.0;
   angle[1] = 0.0;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
comicBox::~comicBox()
{
}

/***********************************************************************
 *                                setVertex                            *
 ***********************************************************************/
void comicBox::setVertex(int vertNumber, GLfloat x, GLfloat y,
                         int imgX, int imgY)
{
   float ratio = (float)Farso::SCREEN_Y / (float)imgY;
   
   if( (vertNumber >= 0) && (vertNumber <= 3) )
   {
      texCoord[vertNumber][0] = x / (float)Farso::smallestPowerOfTwo(imgX);
      texCoord[vertNumber][1] = y / (float)Farso::smallestPowerOfTwo(imgY);
      vertex[vertNumber][0] = x * ratio;
      vertex[vertNumber][1] = Farso::SCREEN_Y - (y * ratio);
   }
}

/***********************************************************************
 *                               getTitle                              *
 ***********************************************************************/
string comicBox::getTitle()
{
   return(title);
}

/***********************************************************************
 *                                setEffect                            *
 ***********************************************************************/
void comicBox::setEffect(int type)
{
   effectType = type;
}

/***********************************************************************
 *                             setTimeout                              *
 ***********************************************************************/
void comicBox::setTimeout(int value)
{
   timeout = value;
}

/***********************************************************************
 *                               getStatus                             *
 ***********************************************************************/
int comicBox::getStatus()
{
   return(status);
}

/***********************************************************************
 *                                 skip                                *
 ***********************************************************************/
void comicBox::skip()
{ 
   pos[0] = 0;
   pos[1] = 0;

   scale[0] = 1.0;
   scale[1] = 1.0;

   angle[0] = 0.0;
   angle[1] = 0.0;
   angle[2] = 0.0;

   status = COMIC_BOX_STATUS_DONE;
}

/***********************************************************************
 *                                activate                             *
 ***********************************************************************/
void comicBox::activate()
{
   int i;

   /* Set the current status to running */
   status = COMIC_BOX_STATUS_RUNNING;
   timer = SDL_GetTicks();

   /* Calculate Center Positions */
   center[0] = vertex[0][0] + ((vertex[3][0] - vertex[0][0]) / 2.0);
   center[1] = vertex[0][1] + ((vertex[2][1] - vertex[0][1]) / 2.0);

   /* Apply Center Positions */
   for(i = 0; i < 4; i++)
   {
      vertex[i][0] -= center[0];
      vertex[i][1] -= center[1];
   }

   /* Init Default Values */
   pos[0] = 0;
   pos[1] = 0;
   angle[0] = 0;
   angle[1] = 0;
   angle[2] = 0;
   scale[0] = 1.0;
   scale[1] = 1.0;

   /* Set the initial Position, scales and angles, based on effect type */
   switch(effectType)
   {
      /* Scale-in Effect */
      case COMIC_BOX_EFFECT_SCALE:
         scale[0] = 0.0;
         scale[1] = 0.0;
      break;
      /* Rotate/Scale-in Effect */
      case COMIC_BOX_EFFECT_ROTATE:
         scale[0] = 0.2;
         scale[1] = 0.2;
         angle[2] = 0.0;
      break;
      /* No Effect */
      case COMIC_BOX_EFFECT_NONE:
      default:
         /* No changes to the default */
      break;
   }
}

/***********************************************************************
 *                                 update                              *
 ***********************************************************************/
void comicBox::update()
{
   Uint32 now = SDL_GetTicks();

   switch(effectType)
   {
      /* Scale-in Effect */
      case COMIC_BOX_EFFECT_SCALE:
         scale[0] += 0.1;
         scale[1] += 0.1;
         if( (scale[0] >= 1.0) || (scale[1] >= 1.0) )
         {
            scale[0] = 1.0;
            scale[1] = 1.0;
            /* Put the effect state at NONE, to count timeout */
            timer = SDL_GetTicks(); 
            effectType = COMIC_BOX_EFFECT_NONE;
         }
      break;
      /* Rotate Scale-in Effect */
      case COMIC_BOX_EFFECT_ROTATE:
      {
         angle[2] += 20;
         scale[0] += 0.1;
         scale[1] += 0.1;
         if( (scale[0] >= 1.0) || (scale[1] >= 1.0) )
         {
            scale[0] = 1.0;
            scale[1] = 1.0;
         }
         if(angle[2] >= 360)
         {
            angle[2] = 0;
            /* Put the effect state at NONE, to count timeout */
            timer = SDL_GetTicks(); 
            effectType = COMIC_BOX_EFFECT_NONE;
         }
      }
      break;
      /* No Effect */
      case COMIC_BOX_EFFECT_NONE:
      default:
         if(now - timer > timeout)
         {
            status = COMIC_BOX_STATUS_DONE;
         }
      break;
   }
}

/***********************************************************************
 *                                 render                              *
 ***********************************************************************/
void comicBox::render()
{
   /* First update to the new position */
   if(status == COMIC_BOX_STATUS_RUNNING)
   {
      update();
   }

   /* Now Render it at the desired one */
   glPushMatrix();
      glTranslatef(pos[0],pos[1],0.0);
      glTranslatef(center[0], center[1], 0.0);
      glRotatef(angle[0],1,0,0);
      glRotatef(angle[1],0,1,0);
      glRotatef(angle[2],0,0,1);
      glScalef(scale[0],scale[1],1.0);
      glBegin(GL_QUADS);
         glTexCoord2fv(texCoord[0]);
         glVertex2fv(vertex[0]);

         glTexCoord2fv(texCoord[1]);
         glVertex2fv(vertex[1]);

         glTexCoord2fv(texCoord[2]);
         glVertex2fv(vertex[2]);

         glTexCoord2fv(texCoord[3]);
         glVertex2fv(vertex[3]);
      glEnd();
   glPopMatrix();
}


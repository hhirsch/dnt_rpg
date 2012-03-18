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

#include "sun.h"
#include "util.h"
#include "../gui/draw.h"
#include "../etc/dirs.h"
#include <iostream>
using namespace std;

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
sun::sun(float hour, float farViewX, float farViewZ)
{
   curHour = hour;
   constantAttenuation = 1.0;
   quadricAttenuation = 0.0;
   linearAttenuation = 0.0;

   where[0] = 0.0;
   where[1] = 0.0;
   where[2] = 0.0;
   where[3] = 1.0;
   
   positionOnHour(0.0,0.0);

   dirs dir;
   glGenTextures(1,&sunTexture);
   SDL_Surface* img = IMG_Load(dir.getRealFile("texturas/sky/sun.png").c_str());
   if(!img)
   {
      cerr << "Failed to open Sun Texture!" << endl;
   }
   else
   {
      Farso::setTextureRGBA(img,sunTexture);
      SDL_FreeSurface(img);
   }

   glGenTextures(1,&moonTexture);
   img = IMG_Load(dir.getRealFile("texturas/sky/moon.png").c_str());
   if(!img)
   {
      cerr << "Failed to open Moon Texture!" << endl;
   }
   else
   {
      Farso::setTextureRGBA(img,moonTexture);
      SDL_FreeSurface(img);
   }

}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
sun::~sun()
{
   glDeleteTextures(1,&sunTexture);
   glDeleteTextures(1, &moonTexture);
}

/*********************************************************************
 *                            visibleTime                            *
 *********************************************************************/
bool sun::visibleTime()
{
   return((curHour >= SUN_HOUR_BORN) && (curHour <= SUN_HOUR_DEATH));
}

/*********************************************************************
 *                             shadowTime                            *
 *********************************************************************/
bool sun::shadowTime()
{
   return((curHour >= SUN_HOUR_BORN+2) && (curHour <= SUN_HOUR_DEATH-2));
}

/*********************************************************************
 *                             shadowTime                            *
 *********************************************************************/
float sun::getShadowAlpha()
{
   float val = (-0.014f*curHour*curHour + 0.334f*curHour - 1.4f);
   return((val>0.0f)?val:0.0f);
}

/*********************************************************************
 *                           positionOnHour                          *
 *********************************************************************/
void sun::positionOnHour(float posX, float posZ)
{
   float halfFarView = (OUTDOOR_FARVIEW / 2.0)-1;
   if(visibleTime())
   {
      rotation = SUN_EQU_B * curHour + SUN_EQU_C;
      where[1] = (sin(deg2Rad(rotation))) * (halfFarView);

      where[0] = (sin(deg2Rad(rotation-90)) * (halfFarView) + posX);
      where[2] = posZ;
   }
   else
   {
      if(curHour >= SUN_HOUR_DEATH)
      {
         rotation = SUN_EQN_B * curHour + SUN_EQN_C;
      }
      else
      {
         /* Past MidNight, equation consider hour as 24+curHour */
         rotation = SUN_EQN_B * (curHour+24) + SUN_EQN_C;
      }
      where[1] = (sin(deg2Rad((rotation-180))))*(halfFarView);

      where[0] = (sin(deg2Rad(rotation-270))*(halfFarView) + posX);
      where[2] = posZ;
   }
   defineShadowMatrix();
}

/*********************************************************************
 *                            colorOnHour                            *
 *********************************************************************/
void sun::colorOnHour()
{
   GLfloat color;
   if(visibleTime())
   {
      /* When Visible Time, is The Sun! */
      if(rotation <= 90)
      {
         color = ((rotation / 90) * 0.5)+0.5;
      }
      else
      {
         color = (((180 - rotation) / 90) * 0.5)+0.5;
      }
      actualColor[0] = color;
      actualColor[1] = color;
      actualColor[2] = color;
      actualColor[3] = 1.0;
   }
   else
   {
      if(rotation >= 270)
      {
         color = (0.0034)*rotation - 0.724;
      }
      else
      {
         color = (-0.0034)*rotation + 1.112;
      }
      /* When Not visible, is the Moon! */
      actualColor[0] = color;
      actualColor[1] = color;
      actualColor[2] = color;
      actualColor[3] = 1.0;
   }
}

/*********************************************************************
 *                          updateHourOfDay                          *
 *********************************************************************/
void sun::updateHourOfDay(float hour, float posX, float posZ)
{
   curHour = hour;
   positionOnHour(posX, posZ);
   colorOnHour();
}

/*********************************************************************
 *                              drawSun                              *
 *********************************************************************/
void sun::drawSun()
{
   GLfloat size;
   glPushAttrib(GL_ENABLE_BIT);
   glDisable(GL_LIGHTING);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_FOG);

   glPushMatrix();
      glTranslatef(where[0], where[1], where[2]);

      if(visibleTime())
      {
         glColor4f(0.9,0.87,0.2,1.0);
         glBindTexture(GL_TEXTURE_2D, sunTexture);
         glScalef(40,40,40);
         size = 10;
      }
      else
      {
         glColor4f(1.0,1.0,1.0,1.0);
         glBindTexture(GL_TEXTURE_2D, moonTexture);
         glScalef(40,40,40);
         size = 10;
      }
      glBegin(GL_QUADS);
         glTexCoord2f(0,0);
         glVertex3f(0, size, -size);
         glTexCoord2f(0,1);
         glVertex3f(0, -size, -size);
         glTexCoord2f(1,1);
         glVertex3f(0, -size, size);
         glTexCoord2f(1,0);
         glVertex3f(0, +size, size);
      glEnd();
   glPopMatrix();
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glColor4f(1,1,1,1);
   glPopAttrib();
}

/*********************************************************************
 *                            getRotation                            *
 *********************************************************************/
GLfloat sun::getRotation()
{
   return(rotation);
}

/*********************************************************************
 *                            getPosition                            *
 *********************************************************************/
void sun::getPosition(GLfloat pos[4])
{
   int i;
   for(i=0; i<4; i++)
   {
      pos[i] = where[i];
   }
}

/*********************************************************************
 *                            setLight                               *
 *********************************************************************/
void sun::setLight()
{
   glLightfv(GL_LIGHT0, GL_AMBIENT, actualColor);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, actualColor);
   glLightfv(GL_LIGHT0, GL_SPECULAR, actualColor);
   glPushMatrix();
      //glRotatef (rotation, 0.0, 1.0, 0.0);  
      glLightfv (GL_LIGHT0, GL_POSITION, where);
   glPopMatrix();
   
   glEnable(GL_LIGHT0);
}

/*********************************************************************
 *                          disableLight                             *
 *********************************************************************/
void sun::disableLight()
{
   glDisable(GL_LIGHT0);
}

/*********************************************************************
 *                       defineShadowMatrix                          *
 *********************************************************************/
void sun::defineShadowMatrix()
{
   shadowMat[0] = where[1];
   shadowMat[4] = 0.f - where[0];
   shadowMat[8] = 0.f;
   shadowMat[12] = 0.f;

   shadowMat[1] = 0.f;
   shadowMat[5] = 0.f;
   shadowMat[9] = 0.f;
   shadowMat[13] = 0.f;

   shadowMat[2] = 0.f;
   shadowMat[6] = 0.f - where[2];
   shadowMat[10] = where[1];
   shadowMat[14] = 0.f;

   shadowMat[3] = 0.f;
   shadowMat[7] = 0.f - 1.0;//where[3];
   shadowMat[11] = 0.f;
   shadowMat[15] = where[1];
}

/*********************************************************************
 *                         getShadowMatrix                           *
 *********************************************************************/
GLfloat* sun::getShadowMatrix()
{
   return((GLfloat*)&shadowMat[0]);
}

/*********************************************************************
 *                         mulShadowMatrix                           *
 *********************************************************************/
void sun::mulShadowMatrix()
{
   glMultMatrixf((GLfloat *) shadowMat);
}


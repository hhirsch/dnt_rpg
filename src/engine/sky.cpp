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

#include "sky.h"
#include "util.h"
#include "../map/map.h"
#include "../gui/draw.h"
#include "../etc/dirs.h"
#include "options.h"
#include <math.h>

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
sky::sky()
{
   dirs dir;
   skySize = 0;
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;
   glGenTextures(1,&skyMap);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/sky/skymap.png").c_str());
   if(img != NULL)
   {
      Farso::setTextureRGBA(img, skyMap);
      SDL_FreeSurface(img);
   }

   /* Box texture */
#if 0
   glGenTextures(4,&boxTexture[0]);
   img = IMG_Load(dir.getRealFile("texturas/sky/box1/ash_hills_fr.png").c_str());
   if(img != NULL)
   {
      setTexture(img, boxTexture[0]);
      SDL_FreeSurface(img);
   }
   img = IMG_Load(dir.getRealFile("texturas/sky/box1/ash_hills_rt.png").c_str());
   if(img != NULL)
   {
      setTexture(img, boxTexture[1]);
      SDL_FreeSurface(img);
   }
   img = IMG_Load(dir.getRealFile("texturas/sky/box1/back.png").c_str());
   if(img != NULL)
   {
      setTextureRGBA(img, boxTexture[2]);
      SDL_FreeSurface(img);
   }
   img = IMG_Load(dir.getRealFile("texturas/sky/box1/right.png").c_str());
   if(img != NULL)
   {
      setTextureRGBA(img, boxTexture[3]);
      SDL_FreeSurface(img);
   }
#endif
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
sky::~sky()
{
   glDeleteTextures(1, &skyMap);
#if 0
   glDeleteTextures(4, &boxTexture[0]);
#endif
}

/*********************************************************************
 *                              drawBox                              *
 *********************************************************************/
void sky::drawBox()
{
#if 0
   glEnable( GL_BLEND );
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_TEXTURE_2D);

   glBindTexture(GL_TEXTURE_2D, boxTexture[0]);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(-0.5, -0.5f, -0.5);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(-0.5, 0.5, -0.5);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.5, 0.5, -0.5);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(0.5, -0.5f, -0.5);
   glEnd();

   glBindTexture(GL_TEXTURE_2D, boxTexture[1]);
   glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(0.5, -0.5f, -0.5);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(0.5, 0.5, -0.5);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(0.5, 0.5, 0.5);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.5, -0.5f, 0.5);
   glEnd();

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
#endif
}

/*********************************************************************
 *                    Draw a Dome Sky to SkyList                     *
 *********************************************************************/
void sky::drawDome(int lats, int longs) 
{
   glColor4f(1.0f, 1.0f, 1.0, 1.0f);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, skyMap );
     
   int i,j;
   GLfloat theta1,theta2,theta3;
   GLfloat ex,ey,ez;
   int n = lats;

   for(j=n/4;j<n/2;j++) 
   {
      theta1 = j * TWOPI / n - PID2;
      theta2 = (j + 1) * TWOPI / n - PID2;

      glBegin(GL_QUAD_STRIP);
      for (i=0;i<= n;i++) 
      {
         theta3 = i * TWOPI / n;

         ex = cos(theta2) * cos(theta3);
         ey = sin(theta2);
         ez = cos(theta2) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f((solarTime) / (24.0f), (3*(j+1) / (GLfloat)n));
         glVertex3f(ex,ey,ez);

         ex = cos(theta1) * cos(theta3);
         ey = sin(theta1);
         ez = cos(theta1) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f((solarTime) / (24.0f), (3*(j) / (GLfloat)n));
         glVertex3f(ex,ey,ez);

      }
      
      glEnd();
   }

   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);

}

/*********************************************************************
 *                                 Draw                              *
 *********************************************************************/
void sky::draw(Map* actualMap, GLfloat sunRot, bool disFog)
{
   options opt;
   skySize = (opt.getFarViewFactor()*OUTDOOR_FARVIEW) - 
             (opt.getFarViewFactor()*256);


   solarTime = (sunRot * 24) / 360.0f;

   if(disFog)
   {
      glDisable(GL_FOG);
   }
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
      glScalef(skySize, skySize, skySize);
      drawDome(15,15);
   glPopMatrix();

   if(disFog)
   {
      glEnable(GL_FOG);
   }
   glDisable(GL_COLOR_MATERIAL);

   /* Draw the box */
#if 0
   glPushMatrix();
      glScalef(skySize, skySize, skySize);
      drawBox();
   glPopMatrix();
#endif
}


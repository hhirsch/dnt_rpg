/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;
   glGenTextures(1,&skyMap);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/sky/skymap.png").c_str());
   if(img != NULL)
   {
      setTextureRGBA(img, skyMap);
      SDL_FreeSurface(img);
   }
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
sky::~sky()
{
   glDeleteTextures(1, &skyMap);
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
void sky::draw(Map* actualMap, GLfloat sunRot)
{
   options opt;
   float skySize = (opt.getFarViewFactor()*OUTDOOR_FARVIEW) - 
                   (opt.getFarViewFactor()*256);


   solarTime = (sunRot * 24) / 360.0f;
   
   glDisable(GL_FOG);
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
      glScalef(skySize, skySize, skySize);
      drawDome(15,15);
   glPopMatrix();

   glEnable(GL_FOG);
   glDisable(GL_COLOR_MATERIAL);
}


#include "sky.h"
#include "util.h"
#include "../map/map.h"
#include "../gui/draw.h"
#include <math.h>

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
sky::sky()
{
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;
   SDL_Surface* img = IMG_Load("../data/texturas/sky/skymap.png");
   setTextureRGBA(img, &skyMap);
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
   glColor4f(1,1,1,1);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, skyMap );
     
   int i,j;
   GLfloat theta1,theta2,theta3;
   GLfloat ex,ey,ez;
   int n = lats;

   for (j=n/4;j<n/2;j++) 
   {
   //for (j=0;j<n/2;j++) 
   //{
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
         glTexCoord2f((solarTime) / (24.0), (3*(j+1) / (GLfloat)n));
         glVertex3f(ex,ey,ez);

         ex = cos(theta1) * cos(theta3);
         ey = sin(theta1);
         ez = cos(theta1) * sin(theta3);

         glNormal3f(ex,ey,ez);
         glTexCoord2f((solarTime) / (24.0), (3*(j) / (GLfloat)n));
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
   solarTime = (sunRot * 24) / 360.0;

   if(!actualMap->fog.enabled)
   {
      glDisable(GL_FOG);
   }
   
   glDisable(GL_COLOR);
   glEnable(GL_COLOR_MATERIAL);
   glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
      glScalef(SKY_SIZE,SKY_SIZE,SKY_SIZE);
      drawDome(15,15);
   glPopMatrix();

   if(!actualMap->fog.enabled)
   {
      glEnable(GL_FOG);
   }
}


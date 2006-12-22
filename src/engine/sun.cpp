/*  DccNiTghtmare is Public Domain, do anything with this code. */

#include "sun.h"
#include "util.h"
#include "../gui/desenho.h"
#include <stdio.h>

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
sun::sun(float hour, float farViewX, float farViewZ)
{
   curHour = hour;
   constantAttenuation = 1.0;
   quadricAttenuation = 0.0;
   linearAttenuation = 0.0;

   where[0] = HALFFARVIEW-5000;
   where[1] = 0.0;
   where[2] = 0.0;//HALFFARVIEW-5000;
   where[3] = 1.0;
   
   positionOnHour();

   SDL_Surface* img = IMG_Load("../data/texturas/sky/sun.png");
   if(!img)
   {
      printf("Failed to open Sun Texture!\n");
   }
   else
   {
      carregaTexturaRGBA(img,&sunTexture);
      SDL_FreeSurface(img);
   }
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
sun::~sun()
{
   glDeleteTextures(1,&sunTexture);
}

/*********************************************************************
 *                            visibleTime                            *
 *********************************************************************/
bool sun::visibleTime()
{
   return((curHour >= SUN_HOUR_BORN) && (curHour <= SUN_HOUR_DEATH));
}

/*********************************************************************
 *                           positionOnHour                          *
 *********************************************************************/
void sun::positionOnHour()
{
   if(visibleTime())
   {
      rotation = SUN_EQU_B * curHour + SUN_EQU_C;
      where[1] = (sin(deg2Rad(rotation / 4.0)))*(HALFFARVIEW-1);
      if( ( (rotation > 90) && (where[0] > 0) ) ||
          ( (rotation < 90) && (where[0] < 0) ) )
      {
         where[0] *= -1;
         where[2] *= -1;
      }
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
      where[1] = (sin(deg2Rad((rotation-180) / 4.0)))*(HALFFARVIEW-1);
      if( ( ((rotation-180) > 90) && (where[0] > 0) ) ||
          ( ((rotation-180) < 90) && (where[0] < 0) ) )
      {
         where[0] *= -1;
         where[2] *= -1;
      }

   }
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
 *                        actualizeHourOfDay                         *
 *********************************************************************/
void sun::actualizeHourOfDay(float hour)
{
   curHour = hour;
   positionOnHour();
   colorOnHour();
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
 *                              drawSun                              *
 *********************************************************************/
void sun::drawSun()
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, sunTexture);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glColor3f(0.9,0.87,0.2);
   glPushMatrix();
      glTranslatef(where[0]-4000, where[1], /*where[2]-4000*/0.0);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3f(0, 1500, -1500);
      glTexCoord2f(0,1);
      glVertex3f(0, -1500, -1500);
      glTexCoord2f(1,1);
      glVertex3f(0, -1500, 1500);
      glTexCoord2f(1,0);
      glVertex3f(0, +1500, 1500);
      glEnd();
   glPopMatrix();
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glColor4f(1,1,1,1);
}

/*********************************************************************
 *                            getRotation                            *
 *********************************************************************/
GLfloat sun::getRotation()
{
   return(rotation);
}


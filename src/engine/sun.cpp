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
   where[2] = HALFFARVIEW-5000;
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
   rotation = SUN_EQU_B * curHour + SUN_EQU_C;
   where[1] = (sin(deg2Rad(rotation)))*(HALFFARVIEW-1);
   if( ( (rotation > 90) && (where[0] > 0) ) ||
       ( (rotation < 90) && (where[0] < 0) ) )
   {
      where[0] *= -1;
      where[2] *= -1;
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
      actualColor[0] = 0.2;
      actualColor[1] = 0.2;
      actualColor[2] = 0.2;
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
   glDisable(GL_DEPTH_FUNC);
   glColor4f(1,1,1,1);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, sunTexture);
   glEnable(GL_COLOR_MATERIAL);
   GLfloat color[] = {0.9, 0.87, 0.2};
   glColor3fv(color);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
   glPushMatrix();
      glBegin(GL_QUADS);
      glTexCoord2f(0,0);
      glVertex3f(where[0]-1500, where[1]+1500, where[2]);
      glTexCoord2f(0,1);
      glVertex3f(where[0]-1500, where[1]-1500, where[2]);
      glTexCoord2f(1,1);
      glVertex3f(where[0]+1500, where[1]-1500, where[2]);
      glTexCoord2f(1,0);
      glVertex3f(where[0]+1500, where[1]+1500, where[2]);
      glEnd();
   glPopMatrix();
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_BLEND);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_FUNC);
}

/*********************************************************************
 *                            getRotation                            *
 *********************************************************************/
GLfloat sun::getRotation()
{
   return(rotation);
}


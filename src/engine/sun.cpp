/*  DccNiTghtmare is Public Domain, do anything with this code. */

#include "sun.h"
#include "util.h"
#include <stdio.h>

sun::sun(float hour, int farViewX, int farViewZ)
{
   curHour = hour;
   constantAttenuation = 1.0;
   quadricAttenuation = 0.0;
   linearAttenuation = 0.0;

   where[0] = farViewX;
   where[1] = 0.0;
   where[2] = farViewZ;
   where[3] = 1.0;
   
   positionOnHour();
}

sun::~sun()
{
}

bool sun::visibleTime()
{
   return((curHour >= SUN_HOUR_BORN) && (curHour <= SUN_HOUR_DEATH));
}

void sun::positionOnHour()
{
   if( visibleTime() )
   {
      rotation = SUN_EQU_B * curHour + SUN_EQU_C;
   }
}

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
      actualColor[0] = 0.0;
      actualColor[1] = 0.0;
      actualColor[2] = 0.0;
      actualColor[3] = 1.0;
   }
}

void sun::actualizeHourOfDay(float hour)
{
   curHour = hour;
   positionOnHour();
   colorOnHour();
   if(visibleTime())
   {
      glLightfv(GL_LIGHT0, GL_AMBIENT, actualColor);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, actualColor);
      glLightfv(GL_LIGHT0, GL_SPECULAR, actualColor);
      glPushMatrix();
         glRotatef (rotation, 0.0, 1.0, 0.0);
         glLightfv (GL_LIGHT0, GL_POSITION, where);
      glPopMatrix();
      glEnable(GL_LIGHT0);
   }
   else
   {
      glDisable(GL_LIGHT0);
   }
}



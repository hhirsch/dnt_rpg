#include "part4.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

part4::part4(float cX,float cY,float cZ):
                               particleSystem(100,PARTICLE_DRAW_INDIVIDUAL)
{
   centerX = cX; 
   centerY=cY; 
   centerZ=cZ;
   actualParticles = 0;
   alpha = 0.5;
   gravity = 15;
   maxLive = 80;
   finalR = 0.387;
   finalG = 0.387;
   finalB = 0.387;
   initR = 0;
   initG = 0;
   initB = 0;
   drawSphereToList(3.0,6,6); 
}

part4::~part4()
{
   glDeleteLists(sphereList,1);
}

void part4::drawSphereToList(double r, int lats, int longs) 
{
   sphereList = glGenLists(1);
   glNewList(sphereList,GL_COMPILE);
   glScalef(r,r,r);
   int i, j;
   for(i = 0; i <= lats; i++) {
      double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
      double z0  = sin(lat0);
      double zr0 =  cos(lat0);
    
      double lat1 = M_PI * (-0.5 + (double) i / lats);
      double z1 = sin(lat1);
      double zr1 = cos(lat1);
 
      glBegin(GL_QUAD_STRIP);
      for(j = 0; j <= longs; j++) {
         double lng = 2 * M_PI * (double) (j - 1) / longs;
         double x = cos(lng);
         double y = sin(lng);
 
         glNormal3f(x * zr0, y * zr0, z0);
         glVertex3f(x * zr0, y * zr0, z0);
         glNormal3f(x * zr1, y * zr1, z1);
         glVertex3f(x * zr1, y * zr1, z1);
      }
      glEnd();
   }
   glEndList();
}

void part4::Render(particle* part)
{
   glPushMatrix();
   glColor4f(part->R,part->G,part->B,alpha);
   glTranslatef(part->posX,part->posY,part->posZ);
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f(part->prvR,part->prvG,part->prvB,alpha);
   glTranslatef(part->prvX,part->prvY,part->prvZ);
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f( (0.67*part->prvR + 0.33*part->R), 
              (0.67*part->prvG + 0.33*part->G),
              (0.67*part->prvB + 0.33*part->B), alpha);
   glTranslatef((0.67*part->prvX + 0.33*part->posX),
                (0.67*part->prvY + 0.33*part->posY),
                (0.67*part->prvZ + 0.33*part->posZ));
   glCallList(sphereList);
   glPopMatrix();
   glPushMatrix();
   glColor4f( (0.33*part->prvR + 0.67*part->R), 
              (0.33*part->prvG + 0.67*part->G),
              (0.33*part->prvB + 0.67*part->B), alpha);
   glTranslatef((0.33*part->prvX + 0.67*part->posX),
                (0.33*part->prvY+ 0.67*part->posY),
                (0.33*part->prvZ + 0.67*part->posZ));
   glCallList(sphereList);
   glPopMatrix();

}

void part4::InitRender()
{
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE );
   glBlendFunc( GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//   glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 50);
}

void part4::EndRender()
{
   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
   glLineWidth(1);
}

void part4::actualize(particle* part)
{
   float percent = (float) part->age / (float) (maxLive-1);

   part->size -= 0.1*(rand() / ((double)RAND_MAX + 1));

   part->prvR = part->R;
   part->prvG = part->G;
   part->prvB = part->B;

   part->R = (1 - (percent) ) * initR + 
                  (percent) * finalR;
   part->G = (1 - (percent) ) * initG + 
                  (percent) * finalG; 
   part->B = (1 - (percent) ) * initB + 
                  (percent) * finalB;

   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   

   part->velY += seconds*gravity*(rand() / ((double)RAND_MAX + 1));

   part->velX += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);
   part->velZ += seconds*(8*((rand() / ((double)RAND_MAX + 1))) - 4);

   part->posY += part->velY*seconds;
   part->posX += part->velX*seconds;
   part->posZ += part->velZ*seconds;
}

bool part4::continueLive(particle* part)
{
   return( (part->age < maxLive) &&
           ((rand() % 200) != 5) );
}

int part4::needCreate()
{
   return(rand() % 80);
}

void part4::createParticle(particle* part)
{
   part->posX = ((rand() / ((double)RAND_MAX + 1))) +centerX;
   part->posY = ((rand() / ((double)RAND_MAX + 1))) +centerY;
   part->posZ = ((rand() / ((double)RAND_MAX + 1))) +centerZ;
   part->prvX = part->posX;
   part->prvY = part->posY;
   part->prvZ = part->posZ;
   part->size = 4; 
   part->velY = 1.0;
   part->velX = 2*(rand() / ((double)RAND_MAX + 1))-1;
   part->velZ = 2*(rand() / ((double)RAND_MAX + 1))-1;
   part->R = initR;//(0.40*(rand() / ((double)RAND_MAX + 1))) + 0.12;
   part->G = initG;//(0.22*(rand() / ((double)RAND_MAX + 1))) + 0.42;
   part->B = initB;//0.84;
   part->prvR = part->R;
   part->prvG = part->G; 
   part->prvB = part->B;
}

void part4::NextStep(float sec)
{
   seconds = 0.02;
   DoStep();
}

int part4::numParticles()
{
   return(actualParticles);
}


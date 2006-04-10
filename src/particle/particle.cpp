#include "particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(int total, int mode)
{
   particles = (particle*) malloc(total*sizeof(particle));
   maxParticles = total;

   PointParameterf = (PFNGLPOINTPARAMETERFARBPROC)
                                  SDL_GL_GetProcAddress("glPointParameterfARB");
   PointParameterfv = (PFNGLPOINTPARAMETERFVARBPROC) 
                                 SDL_GL_GetProcAddress("glPointParameterfvARB");

   int n;
   for(n = 0; n < maxParticles; n++ )
   {
      particles[n].status = PARTICLE_STATUS_DEAD;
      particles[n].internalNumber = n;
   }
   drawMode =  mode;
   if(drawMode == PARTICLE_DRAW_GROUPS)
   {
      vertexArray = (float*) malloc(total*sizeof(float)*3);
      colorArray = (float*) malloc(total*sizeof(float)*4); 
   }
   else
   {
      vertexArray = NULL;
      colorArray = NULL;
   }
}

/***************************************************************
 *                 Particle System Destructor                  *
 ***************************************************************/
particleSystem::~particleSystem()
{
   free(particles); 
   if(drawMode == PARTICLE_DRAW_GROUPS)
   {
      free(vertexArray);
      vertexArray = NULL;
      free(colorArray);
      colorArray = NULL;
   }
   particles = NULL;
}

/***************************************************************
 *                 Do a step to Particle System                *
 ***************************************************************/
void particleSystem::DoStep()
{
   int n;
   int pendingCreate = needCreate();
   int alive = 0;
   int aliveColor = 0;

   InitRender();

   for(n = 0; n < maxParticles; n++)
   { 
      /* Recreate new ones on dead bodies (or on things that is about to die) */
      particles[n].age++;
      if( ( (particles[n].status == PARTICLE_STATUS_DEAD) || 
            (!continueLive(&particles[n])) ) && (pendingCreate > 0)  )
      {
          if(particles[n].status == PARTICLE_STATUS_DEAD)
             actualParticles++; 
          createParticle(&particles[n]);
          particles[n].status = PARTICLE_STATUS_ALIVE;
          particles[n].age = 0;
          pendingCreate--;
      }
      else if( (!continueLive(&particles[n])) && 
               (particles[n].status != PARTICLE_STATUS_DEAD) )
          /* kill particle and not use "body" */
      {
          particles[n].status = PARTICLE_STATUS_DEAD;
          actualParticles--;
      }

      /* Only for alive particles, actualize parameters */
      if( particles[n].status == PARTICLE_STATUS_ALIVE )
      {
         actualize(&particles[n]);
      }

      /* Render "not dead" particles */
      if( particles[n].status != PARTICLE_STATUS_DEAD)
      {
         if(drawMode == PARTICLE_DRAW_GROUPS)
         {
            vertexArray[alive] = particles[n].posX;
            vertexArray[alive+1] = particles[n].posY;
            vertexArray[alive+2] = particles[n].posZ;

            colorArray[aliveColor] = particles[n].R;
            colorArray[aliveColor+1] = particles[n].G;
            colorArray[aliveColor+2] = particles[n].B;
            //colorArray[aliveColor+3] = 1.0;
         }
         else
         {
            Render(&particles[n]);
         }
         alive += 3;
         aliveColor += 3;
      }

   }

   if( (drawMode == PARTICLE_DRAW_GROUPS) && (aliveColor > 0) && (alive > 0) )
   {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(3, GL_FLOAT, 0, colorArray);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertexArray);
      glDrawArrays(GL_POINTS, 0, (int)alive / (int)3);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
   }

   EndRender();

}


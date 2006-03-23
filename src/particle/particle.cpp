#include "particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(int total)
{
   particles = (particle*) malloc(total*sizeof(particle));
   maxParticles = total;

   int n;
   for(n = 0; n < maxParticles; n++ )
   {
      particles[n].status = PARTICLE_STATUS_DEAD;
      particles[n].internalNumber = n;
   }
}

/***************************************************************
 *                 Particle System Destructor                  *
 ***************************************************************/
particleSystem::~particleSystem()
{
   free(particles); 
   particles = NULL;
}

/***************************************************************
 *                 Do a step to Particle System                *
 ***************************************************************/
void particleSystem::DoStep()
{
   int n;
   int pendingCreate = needCreate();

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
         Render(&particles[n]);
      }

   }

   EndRender();

}


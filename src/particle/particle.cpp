#include "particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include <iostream>
#include <fstream>

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(int total, int mode)
{
   init(total, mode);
}

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(string fileName, int mode)
{
   std::ifstream file;
   string aux;
   char aux2[20];

   file.open(fileName.c_str(), ios::in | ios::binary);


   if(!file)
   {
      printf("Error while opening particle file: %s\n",fileName.c_str());
      return;
   }

   getline(file, aux);
   sscanf(aux.c_str(), "%s %d",&aux2[0], &maxLive);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %d",&aux2[0], &maxParticles);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &centerX);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &centerY);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &centerZ);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &gravity);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &initR);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &initG);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &initB);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &finalR);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &finalG);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &finalB);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f",&aux2[0], &alpha);
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dMultCenter[0],
                             &dMultCenter[1], &dMultCenter[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dSumCenter[0],
                             &dSumCenter[1], &dSumCenter[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dMultPos[0],
                             &dMultPos[1], &dMultPos[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dSumPos[0],
                             &dSumPos[1], &dSumPos[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dMultColor[0],
                             &dMultColor[1], &dMultColor[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dSumColor[0],
                             &dSumColor[1], &dSumColor[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dMultVel[0],
                             &dMultVel[1], &dMultVel[2] );
   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &dSumVel[0],
                             &dSumVel[1], &dSumVel[2] );

   file.close();

   init(maxParticles, mode);
  
}

/***************************************************************
 *                Particle System Initialization               *
 ***************************************************************/
void particleSystem::init(int total, int mode)
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
   followPC = false;
   windAffect = false;
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


void particleSystem::Save( string fileName)
{
   std::ofstream file;
   string aux;

   file.open(fileName.c_str(), ios::out | ios::binary);


   if(!file)
   {
      printf("Error while opening particle file: %s\n",fileName.c_str());
      return;
   }

   file << "MAXLIVE " << maxLive << "\n";
   file << "MAXPARTICLES " << maxParticles << "\n";
   file << "CENTERX " << centerX << "\n";
   file << "CENTERY " << centerY << "\n"; 
   file << "CENTERZ " << centerZ << "\n";
   file << "GRAVITY " << gravity << "\n";
   file << "INITR " << initR << "\n";
   file << "INITG " << initG << "\n";
   file << "INITB " << initB << "\n";
   file << "FINALR " << finalR << "\n";
   file << "FINALG " << finalG << "\n";
   file << "FINALB " << finalB << "\n";
   file << "ALPHA " << alpha << "\n";
   file << "DMULTCENTER " << dMultCenter[0] << " " << dMultCenter[1] << " " << 
                             dMultCenter[2] << "\n";
   file << "DSUMCENTER " << dSumCenter[0] << " " <<  dSumCenter[1] << " " <<  
                            dSumCenter[2] << "\n";
   file << "DMULTPOS " << dMultPos[0] << " " <<  dMultPos[1] << " " << 
                          dMultPos[2] << "\n";
   file << "DSUMPOS " << dSumPos[0] << " " <<  dSumPos[1] << " " << 
                         dSumPos[2] << "\n";
   file << "DMULTCOLOR " << dMultColor[0] << " " <<  dMultColor[1] << " " <<  
                            dMultColor[2] << "\n";
   file << "DSUMCOLOR " << dSumColor[0] << " " << dSumColor[1] << " " <<  
                           dSumColor[2] << "\n";
   file << "DMULTVEL " << dMultVel[0] << " " <<  dMultVel[1] << " " << 
                             dMultVel[2] << "\n";
   file << "DSUMVEL " << dSumVel[0] << " " <<  dSumVel[1] << " " << 
                           dSumVel[2] << "\n";

   file.close();
}

void particleSystem::definePosition(float cX, float cZ)
{
   centerX = cX;
   centerZ = cZ;
}

/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#include "particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "../engine/culling.h"

#include <iostream>
#include <fstream>
using namespace std;

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(int total, int mode)
{
   init(total, mode);
   type = DNT_PARTICLE_TYPE_NONE;
   strFileName = "";
}

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem()
{
   init(1,PARTICLE_DRAW_INDIVIDUAL);
   strFileName = "";
   type = DNT_PARTICLE_TYPE_NONE;
}

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem(string fileName, int mode)
{
   dirs dir;
   std::ifstream file;
   string aux;
   char aux2[20];

   type = DNT_PARTICLE_TYPE_NONE;

   strFileName = fileName;
   file.open(dir.getRealFile(fileName).c_str(),
             ios::in | ios::binary);


   if(!file)
   {
      init(maxParticles, mode);
      cerr << "Error while opening particle file: " 
           << dir.getRealFile(fileName) << endl;
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

   getline(file, aux);
   sscanf(aux.c_str(), "%s %f %f %f",&aux2[0], &initVelX, &initVelY, &initVelZ);

   file.close();

   init(maxParticles, mode);
  
}

/***************************************************************
 *                Particle System Initialization               *
 ***************************************************************/
void particleSystem::init(int total, int mode)
{
   systemInitialLiveTime = SDL_GetTicks();
   systemMaxLiveTime = 0;
   actualParticles = 0;
   particles = (particle*) new particle[total];
   maxParticles = total;

   int n;
   for(n = 0; n < maxParticles; n++ )
   {
      particles[n].status = PARTICLE_STATUS_DEAD;
      particles[n].internalNumber = n;
   }
   drawMode =  mode;
   if(drawMode == PARTICLE_DRAW_GROUPS)
   {
      vertexArray = (float*) new float[total*3];
      colorArray = (float*) new float[total*3];
   }
   else
   {
      vertexArray = NULL;
      colorArray = NULL;
   }
   followCharacter = NULL;
   followIsPC = false;
   windAffect = false;
}

/***************************************************************
 *                            finish                           *
 ***************************************************************/
void particleSystem::finish()
{
   if(particles)
   {
      delete []particles; 
      if(drawMode == PARTICLE_DRAW_GROUPS)
      {
         delete[] vertexArray;
         vertexArray = NULL;
         delete[] colorArray;
         colorArray = NULL;
      }
   }
   particles = NULL;
}

/***************************************************************
 *                 Particle System Destructor                  *
 ***************************************************************/
particleSystem::~particleSystem()
{
   finish();
}

/***************************************************************
 *             Reset the Particle System Bounding Box          *
 ***************************************************************/
void particleSystem::resetBoundingBox()
{
   boundX1 = -1;
   boundX2 = -1;
   boundY1 = -1;
   boundY2 = -1;
   boundZ1 = -1;
   boundZ2 = -1;
}

/***************************************************************
 *                 Do a step to Particle System                *
 ***************************************************************/
void particleSystem::doStep(GLfloat** matriz)
{
   int n;
   int pendingCreate = needCreate();
   int alive = 0;
   int aliveColor = 0;

   resetBoundingBox();

   initRender();

   for(n = 0; n < maxParticles; n++)
   { 
      /* Recreate new ones on dead bodies (or on things that is about to die) */
      particles[n].age++;
      if( ( (particles[n].status == PARTICLE_STATUS_DEAD) || 
            (!continueLive(&particles[n])) ) && (pendingCreate > 0)  )
      {
          if(particles[n].status == PARTICLE_STATUS_DEAD)
          {
             actualParticles++; 
          }
          /* Create a new particle */
          createParticle(&particles[n]);
          /* Set State */
          particles[n].status = PARTICLE_STATUS_ALIVE;
          particles[n].age = 0;
          /* Add the initial velocity to it */
          particles[n].velX += initVelX;
          particles[n].velY += initVelY;
          particles[n].velZ += initVelZ;
          /* Decrement the number of pending to create particles */
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
         update(&particles[n]);
      }

      /* Render "not dead" particles */
      if( particles[n].status != PARTICLE_STATUS_DEAD)
      {
         
         /* Actualize Bounding Box */
         if( boundX1 == -1 )
         {
            /* First Particle top take */
            boundX1 = particles[n].posX-5;
            boundX2 = particles[n].posX+5;
            boundY1 = particles[n].posY-5;
            boundY2 = particles[n].posY+5;
            boundZ1 = particles[n].posZ-5;
            boundZ2 = particles[n].posZ+5;
         }
         else
         {
            /* Put New Values, if needed */
            if( particles[n].posX-5 < boundX1)
            {
               boundX1 = particles[n].posX-5;
            }
            if( particles[n].posX+5 > boundX2)
            {
               boundX2 = particles[n].posX+5;
            }
            if( particles[n].posY-5 < boundY1)
            {
               boundY1 = particles[n].posY-5;
            }
            if( particles[n].posY+5 > boundY2)
            {
               boundY2 = particles[n].posY+5;
            }
            if( particles[n].posZ-5 < boundZ1)
            {
               boundZ1 = particles[n].posZ-5;
            }
            if( particles[n].posZ+5 > boundZ2)
            {
               boundZ2 = particles[n].posZ+5;
            }
         }
         
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
         else if((matriz == NULL) ||
                 (visibleCube(boundX1, boundY1, boundZ1, boundX2, 
                                 boundY2, boundZ2,matriz)))
         {
            render(&particles[n]);
         }
         alive += 3;
         aliveColor += 3;
      }

   }

   if( (drawMode == PARTICLE_DRAW_GROUPS) && (aliveColor > 0) && (alive > 0) &&
       ( (matriz == NULL) || 
         (visibleCube(boundX1, boundY1, boundZ1, boundX2, boundY2, boundZ2,
                           matriz))))
   {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(3, GL_FLOAT, 0, colorArray);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertexArray);
      glDrawArrays(GL_POINTS, 0, (int)alive / (int)3);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
   }

   endRender();

}

/***********************************************************
 *                         Save                            *
 ***********************************************************/
bool particleSystem::save( string fileName)
{
   std::ofstream file;
   string aux;

   file.open(fileName.c_str(), ios::out | ios::binary);


   if(!file)
   {
      return(false);
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
   file << "INITIALVEL " << initVelX << " " << initVelY << " " <<
                           initVelZ << "\n";

   file.close();
   return(true);
}

/****************************************************************************
 *                              LoadTexture                                 *
 ****************************************************************************/
GLuint particleSystem::loadTexture(string fileName)
{
   GLuint indice;
   SDL_Surface* img = IMG_Load(fileName.c_str());

   glGenTextures(1, &(indice));
   glBindTexture(GL_TEXTURE_2D, indice);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
   return(indice);
}

/***********************************************************
 *                    definePosition                       *
 ***********************************************************/
void particleSystem::definePosition(float cX, float cZ)
{
   centerX = cX;
   centerZ = cZ;
}

/***********************************************************
 *                    definePosition                       *
 ***********************************************************/
void particleSystem::definePosition(float cX, float cY, float cZ)
{
   centerX = cX;
   centerY = cY;
   centerZ = cZ;
}

/***********************************************************
 *                      getFileName                        *
 ***********************************************************/
string particleSystem::getFileName()
{
   return(strFileName);
}

/***********************************************************
 *                      getPosition                        *
 ***********************************************************/
void particleSystem::getPosition(GLfloat& x, GLfloat &y, GLfloat& z)
{
   x=centerX;
   y=centerY;
   z=centerZ;
}

/***********************************************************
 *                    getMaxParticles                      *
 ***********************************************************/
int particleSystem::getMaxParticles()
{
   return(maxParticles);
}

/***********************************************************
 *                     numParticles                        *
 ***********************************************************/
int particleSystem::numParticles()
{
   return(actualParticles);
}

/***********************************************************
 *                      setFollowPC                        *
 ***********************************************************/
void particleSystem::setFollowCharacter(void* follow, bool isPC)
{
   followCharacter = follow;
   followIsPC = isPC;
}

/***********************************************************
 *                     setDurationTime                     *
 ***********************************************************/
void particleSystem::setDurationTime(int time)
{
   systemMaxLiveTime = time;
}


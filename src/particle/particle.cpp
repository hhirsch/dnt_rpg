/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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
#include "../etc/defparser.h"

#include <iostream>
#include <fstream>
using namespace std;

/***************************************************************
 *                 Particle System Constructor                 *
 ***************************************************************/
particleSystem::particleSystem()
{
   /* Default values */
   drawMode = DNT_PARTICLE_DRAW_INDIVIDUAL;
   renderMode = DNT_PARTSYSTEM_RENDER_DEFAULT;
   colorArray = NULL;
   vertexArray = NULL;

   gravity = 10;
   initialLifeTime = 0;
   maxLifeTime = 0;
   maxParticleLifeTime = 0;

   type = DNT_PARTSYSTEM_TYPE_DEFAULT;
   maxParticles = 0;
   actualParticles = 0;
   followCharacter = NULL;
   followIsPC = false;
   windAffect = false;
   strFileName = "";
   particles = NULL;
}

/***************************************************************
 *                          destructor                         *
 ***************************************************************/
particleSystem::~particleSystem()
{
   if(particles)
   {
      delete []particles; 
      particles = NULL;
   }

   if(vertexArray)
   {
      delete[] vertexArray;
      vertexArray = NULL;
   }

   if(colorArray)
   {
      delete[] colorArray;
      colorArray = NULL;
   }

   if(!textureFileName.empty())
   {
      glDeleteTextures(1, &(partTexture));
   }
}

/***************************************************************
 *                              load                           *
 ***************************************************************/
bool particleSystem::load(string fileName)
{
   defParser def;
   string key="", value="";

   type = DNT_PARTICLE_TYPE_DEFAULT;

   strFileName = fileName;
   if(!def.load(fileName))
   {
      cerr << "Couldn't load particle system " << fileName << endl;
      return(false);
   }

   /* Get each tuple */
   while(def.getNextTuple(key, value))
   {
      /* drawMode */
      if(key == "drawMode")
      {
          if(value == "individual")
          {
             drawMode = DNT_PARTICLE_DRAW_INDIVIDUAL;
          }
          else if(value == "group")
          {
             drawMode = DNT_PARTICLE_DRAW_GROUP;
          }
      }
      /* renderMode */
      else if(key == "renderMode")
      {
         if(value == "default")
         {
            renderMode = DNT_PARTSYSTEM_RENDER_DEFAULT;
         }
      }
      /* type */
      else if(key == "type")
      {
         sscanf(value.c_str(), "%d", &type);
      }
      /* max particles */
      else if(key == "maxParticles")
      {
         sscanf(value.c_str(), "%d", &maxParticles);
      }
      /* wind affect */
      else if(key == "windAffect")
      {
         windAffect = (value == "true");
      }
      /* texture file name */
      else if(key == "texture")
      {
         textureFileName = value;
      }
      /* maxLifeTime */
      else if(key == "maxLifeTime")
      {
         sscanf(value.c_str(), "%d", &maxLifeTime);
      }
      /* maxParticleLifeTime */
      else if(key == "maxParticleLifeTime")
      {
         sscanf(value.c_str(), "%d", &maxParticleLifeTime);
      }
      /* gravity */
      else if(key == "gravity")
      {
         sscanf(value.c_str(), "%f", &gravity);
      }
      /* origin */
      else if(key == "origin")
      {
         origin.fromString(value);
      }
      /* colorRed */
      else if(key == "colorRed")
      {
         color[0].fromString(value);
      }
      /* colorGreen */
      else if(key == "colorGreen")
      {
         color[1].fromString(value);
      }
      /* colorBlue */
      else if(key == "colorBlue")
      {
         color[2].fromString(value);
      }
      /* colorAlpha */
      else if(key == "colorAlpha")
      {
         color[3].fromString(value);
      }
      /* velocityX */
      else if(key == "velocityX")
      {
         velocity[0].fromString(value);
      }
      /* velocityY */
      else if(key == "velocityY")
      {
         velocity[1].fromString(value);
      }
      /* velocityZ */
      else if(key == "velocityZ")
      {
         velocity[2].fromString(value);
      }
      /* positionX */
      else if(key == "positionX")
      {
         position[0].fromString(value);
      }
      /* positionY */
      else if(key == "positionY")
      {
         position[1].fromString(value);
      }
      /* positionZ */
      else if(key == "positionZ")
      {
         position[2].fromString(value);
      }
   }

   /* Init the ParticleSystem with loaded values */
   init();

   return(true);
}

/***************************************************************
 *                Particle System Initialization               *
 ***************************************************************/
void particleSystem::init()
{
   int n;

   /* Verify total particles */
   if(maxParticles <= 0)
   {
      cerr << "Couldn't init partSystem (" << strFileName << ") with "
           << "maxParticles = " << maxParticles << endl;
      return;
   }

   /* Reset the timer */
   initialLifeTime = SDL_GetTicks();

   /* Create particle vector, and initialize it */
   particles = (particle*) new particle[maxParticles];
   for(n = 0; n < maxParticles; n++ )
   {
      particles[n].status = PARTICLE_STATUS_DEAD;
      particles[n].internalNumber = n;
   }

   /* Create render arrays if needed */
   if(drawMode == DNT_PARTICLE_DRAW_GROUP)
   {
      vertexArray = (float*) new float[maxParticles*3];
      colorArray = (float*) new float[maxParticles*3];
   }

   /* Load the texture */
   if(!textureFileName.empty())
   {
      loadTexture();
   }
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
void particleSystem::loadTexture()
{
   dirs dir;
   SDL_Surface* img = IMG_Load(dir.getRealFile(fileName).c_str());

   glGenTextures(1, &(partTexture));
   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   SDL_FreeSurface(img);
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


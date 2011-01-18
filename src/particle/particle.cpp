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
   renderMode = DNT_PARTICLE_RENDER_DEFAULT;
   colorArray = NULL;
   vertexArray = NULL;

   gravity = 10;
   initialLifeTime = 0;
   maxLifeTime = 0;
   maxParticleLifeTime = 0;

   type = DNT_PARTICLE_SYSTEM_TYPE_DEFAULT;
   maxParticles = 0;
   actualParticles = 0;
   followCharacter = NULL;
   followIsPC = false;
   windAffect = false;
   strFileName = "";
   particles = NULL;
   pointSize = 8;
}

/***************************************************************
 *                          destructor                         *
 ***************************************************************/
particleSystem::~particleSystem()
{
   /* Delete all particles */
   if(particles)
   {
      delete []particles; 
      particles = NULL;
   }

   /* Delete all created arrays */
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

   /* Delete the particles texture */
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

   strFileName = fileName;
   if(!def.load(fileName))
   {
      cerr << "Couldn't load particle system " << fileName << endl;
      return(false);
   }

   /* Get each tuple */
   while(def.getNextTuple(key, value))
   {
      /* Type */
      if(key == "type")
      {
         if(value == "default")
         {
            type = DNT_PARTICLE_SYSTEM_TYPE_DEFAULT;
         }
      }
      /* pointSize */
      else if(key == "pointSize")
      {
         sscanf(value.c_str(), "%d", &pointSize);
      }
      /* drawMode */
      else if(key == "drawMode")
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
            renderMode = DNT_PARTICLE_RENDER_DEFAULT;
         }
         else if(value == "glow")
         {
            renderMode = DNT_PARTICLE_RENDER_GLOW;
         }
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
      /* scale */
      else if(key == "scale")
      {
         scale.fromString(value);
      }
   }

   /* Init the ParticleSystem with loaded values */
   init();

   return(true);
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

   /* Type */
   switch(type)
   {
      default:
      case DNT_PARTICLE_SYSTEM_TYPE_DEFAULT:
      {
         file << "type = default" << endl;
      }
      break;
   }
   /* drawMode */
   switch(drawMode)
   {
      case DNT_PARTICLE_DRAW_INDIVIDUAL:
      {
         file << "drawMode = individual" << endl;
      }
      break;
      case DNT_PARTICLE_DRAW_GROUP:
      default:
      {
         file << "drawMode = group" << endl;
      }
      break;
   }
   /* renderMode */
   switch(renderMode)
   {
      case DNT_PARTICLE_RENDER_GLOW:
      {
         file << "renderMode = glow" << endl;
      }
      break;
      case DNT_PARTICLE_RENDER_DEFAULT:
      default:
      {
         file << "renderMode = default" << endl;
      }
      break;
   }
   /* pointSize */
   file << "pointSize = " << pointSize << endl;
   /* max particles */
   file << "maxParticles = " << maxParticles << endl;
   /* wind affect */
   file << "windAffect = " << ((windAffect)?"true":"false") << endl;
   /* texture file name */
   file << "texture = " << textureFileName << endl;
   /* maxLifeTime */
   file << "maxLifeTime = " << maxLifeTime << endl;
   /* maxParticleLifeTime */
   file << "maxParticleLifeTime = " << maxParticleLifeTime << endl;
   /* gravity */
   file << "gravity = " << gravity << endl;
   /* origin */
   file << "origin = " << origin.toString() << endl;
   /* colorRed */
   file << "colorRed = " << color[0].toString() << endl;
   /* colorGreen */
   file << "colorGreen = " << color[1].toString() << endl;
   /* colorBlue */
   file << "colorBlue = " << color[2].toString() << endl;
   /* colorAlpha */
   file << "colorAlpha = " << color[3].toString() << endl;
   /* velocityX */
   file << "velocityX = " << velocity[0].toString() << endl;
   /* velocityY */
   file << "velocityY = " << velocity[1].toString() << endl;
   /* velocityZ */
   file << "velocityZ = " << velocity[2].toString() << endl;
   /* positionX */
   file << "positionX = " << position[0].toString() << endl;
   /* positionY */
   file << "positionY = " << position[1].toString() << endl;
   /* positionZ */
   file << "positionZ = " << position[2].toString() << endl;
   /* scale */
   file << "scale = " << scale.toString() << endl;

   file.close();
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
 *                       createParticle                        *
 ***************************************************************/
void particleSystem::createParticle(particle* part)
{
   /* Set State */
   part->status = PARTICLE_STATUS_ALIVE;
   part->age = 0;

   /* Set initial position */
   origin.generateNewInitialPosition(part->posX, part->posY, part->posZ);
   
   /* Set its initial velocity */
   part->velX = velocity[0].getInitialValue();
   part->velY = velocity[1].getInitialValue();
   part->velZ = velocity[2].getInitialValue();

   /* Set its initial color */
   part->R = color[0].getInitialValue();
   part->G = color[1].getInitialValue();
   part->B = color[2].getInitialValue();
   part->A = color[3].getInitialValue();

   /* Set its initial size */
   part->size = scale.getInitialValue();
}

/***************************************************************
 *                          initRender                         *
 ***************************************************************/
void particleSystem::initRender()
{
   float maxPointSize = 0;
   float quadratic[] =  { 0.01f, 0.01f, 0.0f };

   glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_CULL_FACE);

   /* Verify extensions */
   if(!ext.hasPointTexture())
   {
      /* No extensions, simple color and point. */
      glEnable(GL_COLOR);
      glPointSize(1);
      return;
   }

   glEnable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);

   switch(renderMode)
   {
      /* Default Rendererd */
      case DNT_PARTICLE_RENDER_DEFAULT:
      {
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      }
      break;
      /* Glow (fire) renderer */
      case DNT_PARTICLE_RENDER_GLOW:
      {
         glBlendFunc(GL_DST_ALPHA,GL_SRC_ALPHA);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      }
      break;
   }

   /* point extension things */
   glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxPointSize);
   ext.arbPointParameterfv(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);
   ext.arbPointParameterf(GL_POINT_SIZE_MIN_ARB, 2.0f);
   ext.arbPointParameterf(GL_POINT_SIZE_MAX_ARB, maxPointSize);

   glPointSize(pointSize);

   /* set the texture */
   glBindTexture(GL_TEXTURE_2D, partTexture);
   glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
   glEnable(GL_POINT_SPRITE_ARB);
}

/***************************************************************
 *                           endRender                         *
 ***************************************************************/
void particleSystem::endRender()
{
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   if( ext.hasPointTexture() )
   {
      glDisable(GL_POINT_SPRITE_ARB);
   }
   else
   {
      glDisable(GL_COLOR);
      glDisable(GL_POINT_SMOOTH);
   }

   glEnable(GL_LIGHTING);
   glDisable( GL_BLEND );
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
         
         /* Update Bounding Box */
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
         
         /* Insert at the arrays, if defined */
         if(drawMode == DNT_PARTICLE_DRAW_GROUP)
         {
            vertexArray[alive] = particles[n].posX;
            vertexArray[alive+1] = particles[n].posY;
            vertexArray[alive+2] = particles[n].posZ;

            colorArray[aliveColor] = particles[n].R;
            colorArray[aliveColor+1] = particles[n].G;
            colorArray[aliveColor+2] = particles[n].B;
            colorArray[aliveColor+3] = particles[n].A;
         }
         /* Or render individually, if visible */
         else if((matriz == NULL) ||
                 (visibleCube(boundX1, boundY1, boundZ1, boundX2, 
                                 boundY2, boundZ2,matriz)))
         {
            render(&particles[n]);
         }
         alive += 3;
         aliveColor += 4;
      }

   }

   /* Render the arrays, if defined as group and visible */
   if( (drawMode == DNT_PARTICLE_DRAW_GROUP) && 
       (aliveColor > 0) && (alive > 0) &&
       ( (matriz == NULL) || 
         (visibleCube(boundX1, boundY1, boundZ1, boundX2, boundY2, boundZ2,
                           matriz))))
   {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_FLOAT, 0, colorArray);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertexArray);
      glDrawArrays(GL_POINTS, 0, (int)alive / (int)3);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);
   }

   endRender();

}

/****************************************************************************
 *                              LoadTexture                                 *
 ****************************************************************************/
void particleSystem::loadTexture()
{
   dirs dir;
   SDL_Surface* img = IMG_Load(dir.getRealFile(textureFileName).c_str());
   glGenTextures(1, &(partTexture));

   if(!img)
   {
      cerr << "Error: couldn't load particle texture: "
           << textureFileName << endl;
      return;
   }

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
   origin.update(cX, cZ);
}

/***********************************************************
 *                    definePosition                       *
 ***********************************************************/
void particleSystem::definePosition(float cX, float cY, float cZ)
{
   origin.update(cX, cY, cZ);
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
   origin.getPosition(x, y, z);
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
   maxLifeTime = time;
}


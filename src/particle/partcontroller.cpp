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

#include "partcontroller.h"
#include "../etc/dirs.h"
#include "../etc/extensions.h"

#include <fstream>
#include <iostream>
using namespace std;

/**********************************************************************
 *                              init                                  *
 **********************************************************************/
void partController::init()
{
   particles = new particleList();
   colDetect = NULL;
   currentMap = NULL;
}

/**********************************************************************
 *                               finish                               *
 **********************************************************************/
void partController::finish()
{
   delete(particles);
}

/**********************************************************************
 *                              deleteAll                             *
 **********************************************************************/
void partController::deleteAll(bool keepPCRelated)
{
   if(keepPCRelated)
   {
      particles->removeNonPCs();
   }
   else
   {
      particles->clearList();
   }
}

/**********************************************************************
 *                         deleteAllPCRelated                         *
 **********************************************************************/
void partController::deleteAllPCRelated()
{
   particles->removePCs();
}

/**********************************************************************
 *                               updateAll                            *
 **********************************************************************/
void partController::updateAll(float PCposX, float PCposY, float PCposZ, 
                              GLfloat** matriz, bool enableGrass)
{
   int i, total;
   int time = SDL_GetTicks();

   particleSystem* part = (particleSystem*)particles->getFirst();
   total = particles->getTotal();
   for(i = 0; i < total; i++)
   {
      /* Apply PC position change */
      if(part->followPC)
      {
         part->definePosition(PCposX, PCposZ);
      }

      /* Do the next step */
      if( (part->type == DNT_PARTICLE_TYPE_FIRE) || 
          (part->type == DNT_PARTICLE_TYPE_METEOR) )
      {
         glDisable(GL_FOG);
      }

      if(part->type != DNT_PARTICLE_TYPE_GRASS)
      {
         part->nextStep(matriz);
      }
      else
      {
         /* FIXME: wind! */
         grass* gr = (grass*)part;
         gr->nextStep(matriz, PCposX, PCposY, PCposZ, NULL);
      }

      if( (part->type == DNT_PARTICLE_TYPE_FIRE) || 
          (part->type == DNT_PARTICLE_TYPE_METEOR) )
      {
         glEnable(GL_FOG);
      }


      /* Verify Living */
      if( (part->systemMaxLiveTime != 0) && 
            (time - part->systemInitialLiveTime >= part->systemMaxLiveTime) )
      {
         part = (particleSystem*)part->getNext();
         particles->removeSystem((particleSystem*)part->getPrevious());
      }
      else
      {
         part = (particleSystem*)part->getNext();
      }
   }

   glColor3f(1.0,1.0,1.0);
}

/**********************************************************************
 *                            addParticle                             *
 **********************************************************************/
particleSystem* partController::addParticle(int type, GLfloat x1, GLfloat z1,
                                        GLfloat x2, GLfloat z2, int total,
                                        GLfloat scale, string fileName)
{
   switch(type)
   {
      case DNT_PARTICLE_TYPE_GRASS:
      {
         grass* gr = new grass(x1,z1,x2,z2,total, scale, fileName);
         particles->addSystem(gr);
         return(gr);
      }
      break;
   }
   return(NULL);
}

/**********************************************************************
 *                            addParticle                             *
 **********************************************************************/
meteor* partController::addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                                GLfloat varX, GLfloat varY, GLfloat varZ,
                                GLfloat targX, GLfloat targY, GLfloat targZ,
                                string fileName)
{
   switch(type)
   {
      case DNT_PARTICLE_TYPE_METEOR:
      {
         meteor* mt = new meteor(X, Y, Z, varX, varY, varZ,
                                 targX, targY, targZ, fileName);
         particles->addSystem(mt);
         mt->defineCollision(colDetect);
         return(mt);
      }
      break;
   }
   return(NULL);
}

/**********************************************************************
 *                            addParticle                             *
 **********************************************************************/
particleSystem* partController::addParticle(int type, GLfloat X, GLfloat Y, 
                                        GLfloat Z, string fileName )
{ 
   particleSystem* part = NULL;

   switch(type)
   {
       case DNT_PARTICLE_TYPE_WATERFALL:
       {
          part = new part1(X,Y,Z,fileName);
       }
       break;
       case DNT_PARTICLE_TYPE_FIRE:
       {
          part = new part2(X,Y,Z,fileName);
       }
       break;
       case DNT_PARTICLE_TYPE_WATER_SURFACE:
       {
          part = new part3(X,Y,Z);
       }
       break;
       case DNT_PARTICLE_TYPE_SMOKE:
       {
          part = new part4(X,Y,Z,fileName);
       }
       break;
       case DNT_PARTICLE_TYPE_BLOOD:
       {
          part5* bl = new part5(X,Y,Z,fileName);
          if(currentMap != NULL)
          {
             Map* m = (Map*)currentMap;
             bl->setTerrainHeight(m->getHeight(X,Z));
          }
          part = bl;
       }
       break;
       case DNT_PARTICLE_TYPE_LIGHTNING:
       {
          part = new part6(X,Y,Z,fileName);
       }
       break;
       case DNT_PARTICLE_TYPE_SNOW:
       {
          part = new part7(X,Y,Z,fileName);
       }
       break;
   }

   if(part != NULL)
   {
      particles->addSystem(part);
   }

   return(part);
}

/**********************************************************************
 *                           removeParticle                           *
 **********************************************************************/
void partController::removeParticle(particleSystem* part)
{
   particles->remove(part);
}

/**********************************************************************
 *                             stabilizeAll                           *
 **********************************************************************/
void partController::stabilizeAll()
{
   int i;
   GLfloat** matriz = NULL;  //not needed to draw, so...
   
   for(i=0; i< PART_STABILIZE_LOOP;i++)
   {
      /* Actualize All, except the grass, whose isn't need to stabilize */
      updateAll(0,0,0,matriz, false);
   }
}

/**********************************************************************
 *                                setMap                              *
 **********************************************************************/
void partController::setActualMap(void* acMap, collision* col)
{
   int i;
   particleSystem* part;
   grass* gr;
   meteor* mt;

   /* set the pointer */
   currentMap = acMap;

   /* Collision system */
   colDetect = col;

   /* Set for all that need this info */
   part = (particleSystem*)particles->getFirst();
   for(i = 0; i < particles->getTotal(); i++)
   {
      /* Grass needs map */
      if(part->type == DNT_PARTICLE_TYPE_GRASS)
      {
         gr = (grass*)part;
         gr->defineMap(acMap);
      }
      /* Meteor needs coldetect */
      else if(part->type == DNT_PARTICLE_TYPE_METEOR)
      {
         mt = (meteor*)part;
         mt->defineCollision(colDetect);
      }
      part = (particleSystem*)part->getNext();
   }
}

/**********************************************************************
 *                             numParticles                           *
 **********************************************************************/
int partController::numParticles()
{
   int i;
   int total = 0;
   
   particleSystem* part = (particleSystem*)particles->getFirst();
   for(i = 0; i < particles->getTotal(); i++)
   {
      total += part->actualParticles;
      part = (particleSystem*)part->next;
   }

   return(total);
}

/**********************************************************************
 *                             loadFromfile                           *
 **********************************************************************/
void partController::loadFromFile(string fileName)
{
   dirs dir;
   fstream file;
   string strBuffer;
   int type; GLfloat X,Y,Z;
   char buffer[150];
   part1* particula;
   int totalPlanes = 0;
   float x1, y1, z1; 
   float x2, y2, z2;
   float dX, dZ; 
   int inclination;

   /* Clear any previous particles */
   deleteAll(true);

   /* Now, try to load from file */
   file.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);

   if(!file)
   {
       cerr << "Error while opening Map particle file: "
            << dir.getRealFile(fileName) << endl;
       return;
   }

   while(!file.eof())
   {
      getline(file, strBuffer);
      sscanf(strBuffer.c_str(), "%d %f %f %f %s", &type,&X,&Y,&Z,&buffer[0]);
      
      /* Waterfall Extra Info */
      if(type == DNT_PARTICLE_TYPE_WATERFALL)
      {
         particula = (part1*) addParticle(type, X, Y, Z, buffer);
         getline(file, strBuffer);
         sscanf(strBuffer.c_str(), "%d", &totalPlanes);
         
         /* read Planes */
         while(totalPlanes > 0)
         {
            getline(file, strBuffer);
            sscanf(strBuffer.c_str(), "%f %f %f %f %f %f %f %f %d",
                   &x1, &y1, &z1, &x2, &y2, &z2, &dX, &dZ, &inclination);
            particula->addPlane(x1, y1, z1, x2, y2, z2, dX, dZ, inclination);
            totalPlanes--;
         }
      }

      /* Grass Extra Info */
      else if(type == DNT_PARTICLE_TYPE_GRASS)
      {
         GLfloat x2, z2, scale;
         int total;

         getline(file, strBuffer);
         sscanf(strBuffer.c_str(), "%f %f %f %d",&x2, &z2, &scale, &total);
         addParticle(type, X, Z, x2, z2, total, scale, buffer);
      }

      /* Other Particles */
      else
      {
         addParticle(type, X, Y, Z, buffer);
      }
   }
   file.close();
}

/**********************************************************************
 *                             saveToFile                             *
 **********************************************************************/
void partController::saveToFile(string fileName)
{
   FILE* file; 
   GLfloat X,Y,Z;
   int i, p;
   interPlane* plane;

   if(!(file=fopen(fileName.c_str(),"w")))
   {
       cerr << "Error while opening Map particle file: " << fileName << endl;
       return;
   }

   /* Save all particles */
   particleSystem* part = (particleSystem*)particles->getFirst();
   for(i = 0; i < particles->getTotal(); i++)
   {
      /* Don't save blood and lightning */
      if( (part->type != DNT_PARTICLE_TYPE_BLOOD) && 
          (part->type != DNT_PARTICLE_TYPE_LIGHTNING) && 
          (part->type != DNT_PARTICLE_TYPE_GRASS))
      {
         /* Save the general info */
         part->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n", part->type, X, Y, Z,
               part->getFileName().c_str());

         /* Save some specific info */
         switch(part->type)
         {
            case DNT_PARTICLE_TYPE_WATERFALL:
            {
               /* Save Planes */
               part1* wt = (part1*)part;
               if(wt->getTotalPlanes() > 0)
               {
                  fprintf(file,"%d\n",wt->getTotalPlanes());
               }
               plane = wt->getLastPlane();
               for(p = 0; p < wt->getTotalPlanes(); p++)
               {
                  fprintf(file,"%f %f %f %f %f %f %f %f %d\n", 
                        plane->x1, plane->y1, plane->z1, 
                        plane->x2, plane->y2, plane->z2, 
                        plane->dX, plane->dZ, plane->inclination);
                  plane = (interPlane*)plane->getNext();
               }
            }
            break;

         }
      }
      else if(part->type == DNT_PARTICLE_TYPE_GRASS)
      {
         grass* gr = (grass*)part;
         GLfloat x1,x2,z1,z2;
         gr->getPosition(x1,z1,x2,z2);
         string name = gr->getGrassFileName();
         fprintf(file,"%d %f %f %f %s\n", part->type, 
               x1, 0.0, z1, name.c_str());
         fprintf(file,"%f %f %f %d\n", x2, z2, gr->getScaleFactor(),
               gr->getMaxParticles());
      }

      part = (particleSystem*)part->getNext();
   }
   
   fclose(file);
}

/**********************************************************************
 *                          static members                            *
 **********************************************************************/
particleList* partController::particles = NULL;
collision* partController::colDetect = NULL;
void* partController::currentMap = NULL;


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

#include "partcontroller.h"
#include "../etc/dirs.h"
#include "../etc/extensions.h"
#include "../engine/character.h"

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
void partController::updateAll(GLfloat** matriz, bool enableGrass)
{
   int i, total;
   int time = SDL_GetTicks();
   particleSystem* part = (particleSystem*)particles->getFirst();

   total = particles->getTotal();
   for(i = 0; i < total; i++)
   {
      /* Do the next step */
      part->doStep(matriz);

      /* Verify Living */
      if( (part->maxLifeTime != 0) && 
            (time - part->initialLifeTime >= part->maxLifeTime) )
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
particleSystem* partController::addParticle(GLfloat X, GLfloat Y, 
                                        GLfloat Z, string fileName )
{ 
   /* Create and load particle system */
   particleSystem* part =new particleSystem();
   part->load(fileName);
   /* Set position on map */
   part->definePosition(X, Y, Z);

   /* Add to the list */
   particles->addSystem(part);

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
      updateAll(matriz, false);
   }
}

/**********************************************************************
 *                                setMap                              *
 **********************************************************************/
void partController::setActualMap(void* acMap, collision* col)
{
   int i;
   particleSystem* part;

   /* set the pointer */
   currentMap = acMap;

   /* Collision system */
   colDetect = col;

   /* Set for all that need this info */
   part = (particleSystem*)particles->getFirst();
   for(i = 0; i < particles->getTotal(); i++)
   {
      /* FIXME: Define map and collision detection */
      //gr->defineMap(acMap);
      //mt->defineCollision(colDetect);

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
      if(getline(file, strBuffer) > 0)
      {
         sscanf(strBuffer.c_str(), "%d %f %f %f %s", &type,&X,&Y,&Z,&buffer[0]);

         /* FIXME: Waterfall Extra Info */
         if(type == DNT_PARTICLE_SYSTEM_TYPE_WATERFALL)
         {
            //particula = (part1*) addParticle(type, X, Y, Z, buffer);
            getline(file, strBuffer);
            sscanf(strBuffer.c_str(), "%d", &totalPlanes);

            /* read Planes */
            while(totalPlanes > 0)
            {
               getline(file, strBuffer);
               sscanf(strBuffer.c_str(), "%f %f %f %f %f %f %f %f %d",
                     &x1, &y1, &z1, &x2, &y2, &z2, &dX, &dZ, &inclination);
               //particula->addPlane(x1, y1, z1, x2, y2, z2, dX, dZ, inclination);
               totalPlanes--;
            }
         }
         /* Other Particles */
         else
         {
            addParticle(X, Y, Z, buffer);
         }
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
   int i;
   //interPlane* plane;

   if(!(file=fopen(fileName.c_str(),"w")))
   {
       cerr << "Error while opening Map particle file: " << fileName << endl;
       return;
   }

   /* Save all particles */
   particleSystem* part = (particleSystem*)particles->getFirst();
   cerr << particles->getTotal() << endl;
   for(i = 0; i < particles->getTotal(); i++)
   {
      /* Save the general info */
      part->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n", part->type, X, Y, Z,
               part->getFileName().c_str());

      /* TODO Save some specific info */
#if 0
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
#endif

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


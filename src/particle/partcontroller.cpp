/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "partcontroller.h"
#include "../etc/dirs.h"
#include "../etc/extensions.h"

/**********************************************************************
 *                             Constructor                            *
 **********************************************************************/
partController::partController()
{
   waterfall = new particleList();
   fire = new particleList();
   waterSurface = new particleList();
   smoke = new particleList();
   blood = new particleList();
   lightning = new particleList();
   snow = new particleList();
   grassParticles = new particleList();
   meteorParticles = new particleList();
   colDetect = NULL;
}

/**********************************************************************
 *                              Destructor                            *
 **********************************************************************/
partController::~partController()
{
   deleteAll();
   delete(waterfall);
   delete(fire);
   delete(waterSurface);
   delete(smoke);
   delete(blood);
   delete(lightning);
   delete(snow);
   delete(grassParticles);
   delete(meteorParticles);
}

/**********************************************************************
 *                              deleteAll                             *
 **********************************************************************/
void partController::deleteAll()
{
   while(waterfall->getTotal() > 0)
   {
      part1* wt = (part1*)waterfall->getFirst();
      waterfall->removeSystem(waterfall->getFirst());
      delete(wt);
   }

   while(fire->getTotal() > 0)
   {
      part2* fr = (part2*)fire->getFirst();
      fire->removeSystem(fire->getFirst());
      delete(fr);
   }

   while(waterSurface->getTotal() > 0)
   {
      part3* ws = (part3*)waterSurface->getFirst();
      waterSurface->removeSystem(waterSurface->getFirst());
      delete(ws);
   }

   while(smoke->getTotal() > 0)
   {
      part4* sm = (part4*)smoke->getFirst();
      smoke->removeSystem(smoke->getFirst());
      delete(sm);
   }

   while(blood->getTotal() > 0)
   {
      part5* bl = (part5*)blood->getFirst();
      blood->removeSystem(blood->getFirst());
      delete(bl);
   }

   while(lightning->getTotal() > 0)
   {
      part6* lt = (part6*)lightning->getFirst();
      lightning->removeSystem(lightning->getFirst());
      delete(lt);
   }

   while(snow->getTotal() > 0)
   {
      part7* sn = (part7*)snow->getFirst();
      snow->removeSystem(snow->getFirst());
      delete(sn);
   }

   while(grassParticles->getTotal() > 0)
   {
      grass* gr = (grass*)grassParticles->getFirst();
      grassParticles->removeSystem(grassParticles->getFirst());
      delete(gr);
   }

   while(meteorParticles->getTotal() > 0)
   {
      meteor* mt = (meteor*)meteorParticles->getFirst();
      meteorParticles->removeSystem(meteorParticles->getFirst());
      delete(mt);
   }

}

/**********************************************************************
 *                               updateAll                            *
 **********************************************************************/
void partController::updateAll(float PCposX, float PCposY, float PCposZ, 
                              GLfloat matriz[6][4], bool enableGrass)
{
   int i, total;

   int time = SDL_GetTicks();

   /* Grass */
   if(enableGrass)
   {
      grass* gr = (grass*)grassParticles->getFirst();
      total = grassParticles->getTotal();
      for(i = 0; i < total; i++)
      {
         /* FIXME -> set the Wind! */
         gr->NextStep(matriz, PCposX, PCposY, PCposZ, NULL);

         /* Verify Living */
         if( (gr->systemMaxLiveTime != 0) && 
             (time - gr->systemInitialLiveTime >= gr->systemMaxLiveTime) )
         {
            gr = (grass*)gr->next;
            removeParticle(PART_GRASS, gr->previous);
         }
         else
         {
            gr = (grass*)gr->next;
         }
      }
   }

   /* Waterfall */ 
   part1* wt = (part1*)waterfall->getFirst();
   total = waterfall->getTotal();
   for(i = 0; i < total; i++)
   {
      if(wt->followPC)
      {
         wt->definePosition(PCposX, PCposY, PCposZ);
      }
      wt->NextStep(matriz);

      /* Verify Live */
      if( (wt->systemMaxLiveTime != 0) && 
          (time - wt->systemInitialLiveTime >= wt->systemMaxLiveTime) )
      {
         wt = (part1*)wt->next;
         removeParticle(PART_WATERFALL, wt->previous);
      }
      else
      {
         wt = (part1*)wt->next;
      }
   }

   glDisable(GL_FOG);

   /* Fire */
   part2* fr = (part2*)fire->getFirst();
   total = fire->getTotal();
   for(i = 0; i < total; i++)
   {
      if(fr->followPC)
      {
         fr->definePosition(PCposX, PCposY, PCposZ);
      }
      fr->NextStep(matriz);
   
      /* Verify Live */
      if( (fr->systemMaxLiveTime != 0) && 
          (time - fr->systemInitialLiveTime >= fr->systemMaxLiveTime) )
      {
         fr = (part2*)fr->next;
         removeParticle(PART_FIRE, fr->previous);
      }
      else
      {
         fr = (part2*)fr->next;
      }
   }

   /* Meteor */
   meteor* mt = (meteor*)meteorParticles->getFirst();
   total = meteorParticles->getTotal();
   for(i = 0; i < total; i++)
   {
      mt->NextStep(matriz);


      /* Verify Live */
      if(!mt->isLiving())
      {
         mt = (meteor*)mt->next;
         removeParticle(PART_METEOR, mt->previous);
      }
      else
      {
         mt = (meteor*)mt->next;
      }
   }
   glEnable(GL_FOG);

   /* WaterSurface */
   part3* ws = (part3*)waterSurface->getFirst();
   total = waterSurface->getTotal();
   for(i = 0; i < total; i++)
   {
      if(ws->followPC)
      {
         ws->definePosition(PCposX, PCposY, PCposZ);
      }
      ws->NextStep(matriz);

      /* Verify Live */
      if( (ws->systemMaxLiveTime != 0) && 
          (time - ws->systemInitialLiveTime >= ws->systemMaxLiveTime) )
      {
         ws = (part3*)ws->next;
         removeParticle(PART_WATER_SURFACE, ws->previous);
      }
      else
      {
         ws = (part3*)ws->next;
      }
   }

   /* Smoke */
   part4* sm = (part4*)smoke->getFirst();
   total = smoke->getTotal();
   for(i = 0; i < total; i++)
   {
      if(sm->followPC)
      {
         sm->definePosition(PCposX, PCposY, PCposZ);
      }
      sm->NextStep(matriz);

      /* Verify Live */
      if( (sm->systemMaxLiveTime != 0) && 
          (time - sm->systemInitialLiveTime >= sm->systemMaxLiveTime) )
      {
         sm = (part4*)sm->next;
         removeParticle(PART_SMOKE, sm->previous);
      }
      else
      {
         sm = (part4*)sm->next;
      }
   }

   /* Blood */
   part5* bl = (part5*)blood->getFirst();
   total = blood->getTotal();
   for(i = 0; i < total; i++)
   {
      if(bl->followPC)
      {
         bl->definePosition(PCposX, PCposY, PCposZ);
      }
      bl->NextStep(matriz);

      /* Verify Live */
      if( (bl->systemMaxLiveTime != 0) && 
          (time - bl->systemInitialLiveTime >= bl->systemMaxLiveTime) )
      {
         bl = (part5*)bl->next;
         removeParticle(PART_BLOOD, bl->previous);
      }
      else
      {
         bl = (part5*)bl->next;
      }
   }

   /* Lightning */
   part6* lt = (part6*)lightning->getFirst();
   total = lightning->getTotal();
   for(i = 0; i < total; i++)
   {
      if(lt->followPC)
      {
         lt->definePosition(PCposX, PCposY, PCposZ);
      }
      lt->NextStep(matriz);

      /* Verify Live */
      if( (lt->systemMaxLiveTime != 0) && 
          (time - lt->systemInitialLiveTime >= lt->systemMaxLiveTime) )
      {
         lt = (part6*)lt->next;
         removeParticle(PART_LIGHTNING, lt->previous);
      }
      else
      {
         lt = (part6*)lt->next;
      }
   }

   /* Snow */
   part7* sn = (part7*)snow->getFirst();
   total = snow->getTotal();
   for(i = 0; i < total; i++)
   {
      if(sn->followPC)
      {
         sn->definePosition(PCposX, PCposY, PCposZ);
      }
      sn->NextStep(matriz);

      /* Verify Live */
      if( (sn->systemMaxLiveTime != 0) && 
          (time - sn->systemInitialLiveTime >= sn->systemMaxLiveTime) )
      {
         sn = (part7*)sn->next;
         removeParticle(PART_SNOW, sn->previous);
      }
      else
      {
         sn = (part7*)sn->next;
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
      case PART_GRASS:
      {
         grass* gr = new grass(x1,z1,x2,z2,total, scale, fileName);
         grassParticles->addSystem(gr);
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
      case PART_METEOR:
      {
         meteor* mt = new meteor(X, Y, Z, varX, varY, varZ,
                                 targX, targY, targZ, fileName);
         meteorParticles->addSystem(mt);
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
   switch(type)
   {
       case PART_WATERFALL:
       {
          part1* wt = new part1(X,Y,Z,fileName);
          waterfall->addSystem(wt);
          return(wt);
       }
       break;
       case PART_FIRE:
       {
          part2* fr = new part2(X,Y,Z,fileName);
          fire->addSystem(fr);
          return(fr);
       }
       break;
       case PART_WATER_SURFACE:
       {
          part3* ws = new part3(X,Y,Z);
          waterSurface->addSystem(ws);
          return(ws);
       }
       break;
       case PART_SMOKE:
       {
          part4* sm = new part4(X,Y,Z,fileName);
          smoke->addSystem(sm);
          return(sm);
       }
       break;
       case PART_BLOOD:
       {
          part5* bl =  new part5(X,Y,Z,fileName);
          blood->addSystem(bl);
          return(bl);
       }
       break;
       case PART_LIGHTNING:
       {
          part6* ln = new part6(X,Y,Z,fileName);
          lightning->addSystem(ln);
          return(ln);
       }
       break;
       case PART_SNOW:
       {
          part7* sn = new part7(X,Y,Z,fileName);
          snow->addSystem(sn);
          return(sn);
       }
       break;
   }

   return(NULL);
}

/**********************************************************************
 *                             removeParticle                         *
 **********************************************************************/
void partController::removeParticle(int type, void* part)
{ 
   switch(type)
   {
       case PART_WATERFALL:
       {
          part1* wt = (part1*) part;
          waterfall->removeSystem(wt);
          delete(wt);
       }
       break;
       case PART_FIRE:
       {
          part2* fr = (part2*) part;
          fire->removeSystem(fr);
          delete(fr);
       }
       break;
       case PART_WATER_SURFACE:
       {
          part3* ws = (part3*) part;
          waterSurface->removeSystem(ws);
          delete(ws);
       }
       break;
       case PART_SMOKE:
       {
          part4* sm = (part4*) part;
          smoke->removeSystem(sm);
          delete(sm);
       }
       break;
       case PART_BLOOD:
       {
          part5* bl = (part5*)part;
          blood->removeSystem(bl);
          delete(bl);
       }
       break;
       case PART_LIGHTNING:
       {
          part6* lt = (part6*)part;
          lightning->removeSystem(lt);
          delete(lt);
       }
       break;
       case PART_SNOW:
       {
          part7* sn = (part7*)part;
          snow->removeSystem(sn);
          delete(sn);
       }
       break;
       case PART_GRASS:
       {
          grass* gr = (grass*)part;
          grassParticles->removeSystem(gr);
          delete(gr);
       }
       break;
       case PART_METEOR:
       {
         meteor* mt = (meteor*)part;
         meteorParticles->removeSystem(mt);
         delete(mt);
       }
       break;
   }

   return;
}

/**********************************************************************
 *                             stabilizeAll                           *
 **********************************************************************/
void partController::stabilizeAll()
{
   int i;
   GLfloat matriz[6][4];  //not needed to draw, so...
   
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

   /* Collision system */
   colDetect = col;

   /* Grass */
   grass* gr = (grass*)grassParticles->getFirst();
   for(i = 0; i < grassParticles->getTotal(); i++)
   {
      gr->defineMap(acMap);
      gr = (grass*)gr->next;
   }

   /* Meteor */
   meteor* mt = (meteor*)meteorParticles->getFirst();
   for(i = 0; i < meteorParticles->getTotal(); i++)
   {
      mt->defineCollision(colDetect);
      mt = (meteor*)mt->next;
   }

}

/**********************************************************************
 *                             numParticles                           *
 **********************************************************************/
int partController::numParticles()
{
   int i;
   int total = 0;
   
   part1* wt = (part1*)waterfall->getFirst();
   for(i = 0; i < waterfall->getTotal(); i++)
   {
      total += wt->numParticles();
      wt = (part1*)wt->next;
   }

   part2* fr = (part2*)fire->getFirst();
   for(i = 0; i < fire->getTotal(); i++)
   {
      total += fr->numParticles();
      fr = (part2*) fr->next;
   }

   part3* ws = (part3*)waterSurface->getFirst();
   for(i = 0; i < waterSurface->getTotal(); i++)
   {
      total += ws->numParticles();
      ws = (part3*)ws->next;
   }

   part4* sm = (part4*)smoke->getFirst();
   for(i = 0; i < smoke->getTotal(); i++)
   {
      total += sm->numParticles();
      sm = (part4*)sm->next;
   }

   part5* bl = (part5*)blood->getFirst();
   for(i = 0; i < blood->getTotal(); i++)
   {
      total += bl->numParticles();
      bl = (part5*) bl->next;
   }

   part6* lt = (part6*) lightning->getFirst();
   for(i = 0; i < lightning->getTotal(); i++)
   {
      total += lt->numParticles();
      lt = (part6*)lt->next;
   }

   part7* sn = (part7*)snow->getFirst();
   for(i = 0; i < snow->getTotal(); i++)
   {
      total += sn->numParticles();
      sn = (part7*)sn->next;
   }

   grass* gr = (grass*)grassParticles->getFirst();
   for(i = 0; i < grassParticles->getTotal(); i++)
   {
      total += gr->numParticles();
      gr = (grass*)gr->next;
   }

   meteor* mt = (meteor*)meteorParticles->getFirst();
   for(i = 0; i < meteorParticles->getTotal(); i++)
   {
      total += mt->numParticles();
      mt = (meteor*)mt->next;
   }


   return(total);
}

/**********************************************************************
 *                             loadFromfile                           *
 **********************************************************************/
void partController::loadFromFile(string fileName)
{
   dirs dir;
   FILE* file; 
   int type; GLfloat X,Y,Z;
   char buffer[150];
   part1* particula;
   int totalPlanes = 0;
   float x1, y1, z1; 
   float x2, y2, z2;
   float dX, dZ; 
   int inclination;

   deleteAll();

   if(!(file=fopen(dir.getRealFile(fileName).c_str(),"r")))
   {
       printf("Error while opening Map particle file: %s\n",
              dir.getRealFile(fileName).c_str());
       return;
   }
   while(fscanf(file,"%d %f %f %f %s",&type,&X,&Y,&Z,&buffer[0]) != EOF)
   {
      if(type == PART_WATERFALL)
      {
         particula = (part1*) addParticle(type, X, Y, Z, buffer);
         fscanf(file,"%d", &totalPlanes);
         
         /* read Planes */
         while(totalPlanes > 0)
         {
            fscanf(file,"%f %f %f %f %f %f %f %f %d",
                   &x1, &y1, &z1, &x2, &y2, &z2, &dX, &dZ, &inclination);
            particula->addPlane(x1, y1, z1, x2, y2, z2, dX, dZ, inclination);
            totalPlanes--;
         }
      }
      else if(type == PART_GRASS)
      {
         GLfloat x2, z2, scale;
         int total;
         fscanf(file, "%f %f %f %d",&x2, &z2, &scale, &total);
         addParticle(type, X, Z, x2, z2, total, scale, buffer);
      }
      else
      {
         addParticle(type, X, Y, Z, buffer);
      }
   }
   fclose(file);
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
       printf("Error while opening Map particle file: %s\n",fileName.c_str());
       return;
   }

   /* Save Waterfalls */
   part1* wt = (part1*)waterfall->getFirst();
   for(i = 0; i < waterfall->getTotal(); i++)
   {
      wt->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n",PART_WATERFALL, X, Y, Z,
              wt->getFileName().c_str());

      /* Save Planes */
      if(wt->getTotalPlanes() > 0)
      {
         fprintf(file,"%d\n",wt->getTotalPlanes());
      }
      plane = wt->getLastPlane();
      for(p = 0; p < wt->getTotalPlanes(); p++)
      {
         fprintf(file,"%f %f %f %f %f %f %f %f %d\n", plane->x1, plane->y1, 
                 plane->z1, plane->x2, plane->y2, plane->z2, plane->dX, 
                 plane->dZ, plane->inclination);
         plane = plane->next;
      }
      wt = (part1*)wt->next;
   }

   /* Save Fires */
   part2* fr = (part2*)fire->getFirst();
   for(i = 0; i < fire->getTotal(); i++)
   {
      fr->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n",PART_FIRE, X, Y, Z,
              fr->getFileName().c_str());
      fr = (part2*)fr->next;
   }

   /* Save Water Surfaces */
   part3* ws = (part3*)waterSurface->getFirst();
   for(i = 0; i < waterSurface->getTotal(); i++)
   {
      ws->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n",PART_WATER_SURFACE, X, Y, Z,
              ws->getFileName().c_str());
      ws = (part3*)ws->next;
   }

   /* Save Smokes */
   part4* sm = (part4*)smoke->getFirst();
   for(i = 0; i < smoke->getTotal(); i++)
   {
      sm->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n",PART_SMOKE, X, Y, Z,
              sm->getFileName().c_str());
      sm = (part4*)sm->next;
   }

   /* Don't Save Blood Particles */
   /*for(i = 0; i < blood->getTotal(); i++)
   {
      if(blood[i] != NULL)
   }*/

   /* Don't Save Lightning Particles */
   /*for(i = 0; i < lightning->getTotal(); i++)
   {
      if(lightning[i] != NULL)
   }*/

   /* Save Snows */
   part7* sn = (part7*)snow->getFirst();
   for(i = 0; i < snow->getTotal(); i++)
   {
      sn->getPosition(X, Y, Z);
      fprintf(file,"%d %f %f %f %s\n",PART_SNOW, X, Y, Z,
              sn->getFileName().c_str());
      sn = (part7*)sn->next;
   }

   /* Save Grass */
   grass* gr = (grass*)grassParticles->getFirst();
   for(i = 0; i < grassParticles->getTotal(); i++)
   {
      GLfloat x1,x2,z1,z2;
      gr->getPosition(x1,z1,x2,z2);
      string name = gr->getGrassFileName();
      fprintf(file,"%d %f %f %f %s\n",PART_GRASS, x1, 0.0, z1, name.c_str());
      fprintf(file,"%f %f %f %d\n", x2, z2, gr->getScaleFactor(),
                                    gr->getMaxParticles());
      gr = (grass*)gr->next;
   }
   
   fclose(file);
}


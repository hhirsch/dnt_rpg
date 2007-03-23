/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "partSystem.h"

/**********************************************************************
 *                             Constructor                            *
 **********************************************************************/
partSystem::partSystem()
{
   int i;
   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      waterfall[i] = NULL;
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      fire[i] = NULL;
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      waterSurface[i] = NULL;
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      smoke[i] = NULL;
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      blood[i] = NULL;
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      lightning[i] = NULL;
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      snow[i] = NULL;
   }

   for(i = 0; i < MAX_GRASS; i++)
   {
      grassParticles[i] = NULL;
   }

}

/**********************************************************************
 *                              Destructor                            *
 **********************************************************************/
partSystem::~partSystem()
{
   deleteAll();
}

/**********************************************************************
 *                              deleteAll                             *
 **********************************************************************/
void partSystem::deleteAll()
{
   int i;
   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
      {
         delete(waterfall[i]);
         waterfall[i] = NULL;
      }
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
      {
         delete(fire[i]);
         fire[i] = NULL;
      }
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
      {
         delete(waterSurface[i]);
         waterSurface[i] = NULL;
      }
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
      {
         delete(smoke[i]);
         smoke[i] = NULL;
      }
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
      {
         delete(blood[i]);
         blood[i] = NULL;
      }
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
      {
         delete(lightning[i]);
         lightning[i] = NULL;
      }
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
      {
         delete(snow[i]);
         snow[i] = NULL;
      }
   }

   for(i = 0; i < MAX_GRASS; i++)
   {
      if(grassParticles[i] != NULL)
      {
         delete(grassParticles[i]);
         grassParticles[i] = NULL;
      }
   }

}

/**********************************************************************
 *                             actualizeAll                           *
 **********************************************************************/
void partSystem::actualizeAll(float PCposX, float PCposZ, GLfloat matriz[6][4],
                              bool enableGrass)
{
   int i;

   if(enableGrass)
   {
      for(i = 0; i < MAX_GRASS; i++)
      {
         if(grassParticles[i] != NULL)
         {
            grassParticles[i]->NextStep(matriz);
         }
      }
   }

   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
      {
         if(waterfall[i]->followPC)
         {
            waterfall[i]->definePosition(PCposX, PCposZ);
         }
         waterfall[i]->NextStep(matriz);
      }
   }

   glDisable(GL_FOG);
   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
      {
         if(fire[i]->followPC)
         {
            fire[i]->definePosition(PCposX, PCposZ);
         }
         fire[i]->NextStep(matriz);
      }
   }
   glEnable(GL_FOG);

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
      {
         if(waterSurface[i]->followPC)
         {
            waterSurface[i]->definePosition(PCposX, PCposZ);
         }
         waterSurface[i]->NextStep(matriz);
      }
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
      {
         if(smoke[i]->followPC)
         {
            smoke[i]->definePosition(PCposX, PCposZ);
         }
         smoke[i]->NextStep(matriz);
      }
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
      {
         if(blood[i]->followPC)
         {
            blood[i]->definePosition(PCposX, PCposZ);
         }
         blood[i]->NextStep(matriz);
      }
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
      {
         if(lightning[i]->followPC)
         {
            lightning[i]->definePosition(PCposX, PCposZ);
         }
         lightning[i]->NextStep(matriz);
      }
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
      {
         if(snow[i]->followPC)
         {
            snow[i]->definePosition(PCposX, PCposZ);
         }
         snow[i]->NextStep(matriz);
      }
   }

   glColor3f(1.0,1.0,1.0);
}

/**********************************************************************
 *                            addParticle                             *
 **********************************************************************/
particleSystem* partSystem::addParticle(int type, GLfloat x1, GLfloat z1,
                                        GLfloat x2, GLfloat z2, int total,
                                        string fileName)
{
   int i;
   switch(type)
   {
      case PART_GRASS:
         for(i = 0; i < MAX_GRASS; i++)
         {
            if(grassParticles[i] == NULL)
            {
               grassParticles[i] = new grass(x1,z1,x2,z2,total, fileName);
               return(grassParticles[i]);
            }
         }
         if(i ==  MAX_GRASS)
         {
            printf("Warn: Too much Grass\n");
         }
       break;
   }
   return(NULL);
}

/**********************************************************************
 *                            addParticle                             *
 **********************************************************************/
particleSystem* partSystem::addParticle(int type, GLfloat X, GLfloat Y, 
                                        GLfloat Z, string fileName )
{ 
   int i;

   switch(type)
   {
       case PART_WATERFALL:
          for(i = 0; i < MAX_WATERFALL; i++)
          {
             if( waterfall[i] == NULL)
             {
                 waterfall[i] = new part1(X,Y,Z,fileName);
                 return(waterfall[i]);
             }
          }
          if(i == MAX_WATERFALL)
          {
              printf("Warn: Too much Waterfalls\n");
          }
       break;
       case PART_FIRE:
          for(i = 0; i < MAX_FIRE; i++)
          {
             if( fire[i] == NULL)
             {
                 fire[i] = new part2(X,Y,Z,fileName);
                 return(fire[i]);
             }
          }
          if(i == MAX_FIRE)
          {
              printf("Warn: Too much Fires\n");
          }
       break;
       case PART_WATER_SURFACE:
          for(i = 0; i < MAX_WATER_SURFACE; i++)
          {
             if( waterSurface[i] == NULL)
             {
                 waterSurface[i] = new part3(X,Y,Z);
                 return(waterSurface[i]);
             }
          }
          if(i == MAX_WATER_SURFACE)
          {
              printf("Warn: Too much Water Surfaces\n");
          }
       break;
       case PART_SMOKE:
          for(i = 0; i < MAX_SMOKE; i++)
          {
             if( smoke[i] == NULL)
             {
                 smoke[i] = new part4(X,Y,Z,fileName);
                 return(smoke[i]);
             }
          }
          if(i == MAX_SMOKE)
          {
              printf("Warn: Too much Smokes\n");
          }
       break;
       case PART_BLOOD:
          for(i = 0; i < MAX_BLOOD; i++)
          {
             if( blood[i] == NULL)
             {
                 blood[i] = new part5(X,Y,Z,fileName);
                 return(blood[i]);
             }
          }
          if(i == MAX_BLOOD)
          {
              printf("Warn: Too much Blood!\n");
          }
       break;
       case PART_LIGHTNING:
          for(i = 0; i < MAX_LIGHTNING; i++)
          {
             if( lightning[i] == NULL)
             {
                 lightning[i] = new part6(X,Y,Z,fileName);
                 return(lightning[i]);
             }
          }
          if(i == MAX_LIGHTNING)
          {
              printf("Warn: Too much Lightning\n");
          }
       break;
       case PART_SNOW:
          for(i = 0; i < MAX_SNOW; i++)
          {
             if( snow[i] == NULL)
             {
                 snow[i] = new part7(X,Y,Z,fileName);
                 return(snow[i]);
             }
          }
          if(i == MAX_SNOW)
          {
              printf("Warn: Too much Snow\n");
          }
       break;
   }

   return(NULL);
}

/**********************************************************************
 *                             removeParticle                         *
 **********************************************************************/
void partSystem::removeParticle(int type, particleSystem* part)
{ 
   int i;

   switch(type)
   {
       case PART_WATERFALL:
          for(i = 0; i < MAX_WATERFALL; i++)
          {
             if( (particleSystem*)waterfall[i] == part)
             {
                 delete(waterfall[i]);
                 waterfall[i] = NULL;
                 return;
             }
          }
       break;
       case PART_FIRE:
          for(i = 0; i < MAX_FIRE; i++)
          {
             if( (particleSystem*)fire[i] == part)
             {
                 delete(fire[i]);
                 fire[i] = NULL;
                 return;
             }
          }
       break;
       case PART_WATER_SURFACE:
          for(i = 0; i < MAX_WATER_SURFACE; i++)
          {
             if( (particleSystem*)waterSurface[i] == part)
             {
                 delete(waterSurface[i]);
                 waterSurface[i] = NULL;
                 return;
             }
          }
       break;
       case PART_SMOKE:
          for(i = 0; i < MAX_SMOKE; i++)
          {
             if( (particleSystem*)smoke[i] == part)
             {
                 delete(smoke[i]);
                 smoke[i] = NULL;
                 return;
             }
          }
       break;
       case PART_BLOOD:
          for(i = 0; i < MAX_BLOOD; i++)
          {
             if( (particleSystem*)blood[i] == part)
             {
                 delete(blood[i]);
                 blood[i] = NULL;
                 return;
             }
          }
       break;
       case PART_LIGHTNING:
          for(i = 0; i < MAX_LIGHTNING; i++)
          {
             if( (particleSystem*)lightning[i] == part)
             {
                 delete(lightning[i]);
                 lightning[i] = NULL;
                 return;
             }
          }
       break;
       case PART_SNOW:
          for(i = 0; i < MAX_SNOW; i++)
          {
             if( (particleSystem*)snow[i] == part)
             {
                 delete(snow[i]);
                 snow[i] = NULL;
                 return;
             }
          }
       break;
       case PART_GRASS:
          for(i = 0; i < MAX_GRASS; i++)
          {
             if( (particleSystem*)grassParticles[i] == part)
             {
                delete(grassParticles[i]);
                grassParticles[i] = NULL;
                return;
             }
          }
       break;
   }

   return;
}

/**********************************************************************
 *                             stabilizeAll                           *
 **********************************************************************/
void partSystem::stabilizeAll()
{
   int i;
   GLfloat matriz[6][4];  //not needed to draw, so...
   
   for(i=0; i< PART_STABILIZE_LOOP;i++)
   {
      /* Actualize All, except the grass, whose isn't need to stabilize */
      actualizeAll(0,0,matriz, false);
   }
}

/**********************************************************************
 *                                setMap                              *
 **********************************************************************/
void partSystem::setActualMap(void* map)
{
   int i;

   for(i = 0; i < MAX_GRASS; i++)
   {
      if(grassParticles[i] != NULL)
      {
         grassParticles[i]->defineMap(map);
      }
   }
}

/**********************************************************************
 *                             numParticles                           *
 **********************************************************************/
int partSystem::numParticles()
{
   int i;
   int total = 0;
   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
         total += waterfall[i]->numParticles();
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
        total += fire[i]->numParticles();
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
        total += waterSurface[i]->numParticles();
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
        total += smoke[i]->numParticles();
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
        total += blood[i]->numParticles();
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
        total += lightning[i]->numParticles();
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
        total += snow[i]->numParticles();
   }

   for(i = 0; i < MAX_GRASS; i++)
   {
      if(grassParticles[i] != NULL)
      {
         total += grassParticles[i]->numParticles();
      }
   }

   return(total);
}

/**********************************************************************
 *                             loadFromfile                           *
 **********************************************************************/
void partSystem::loadFromFile(string fileName)
{
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

   if(!(file=fopen(fileName.c_str(),"r")))
   {
       printf("Error while opening Map particle file: %s\n",fileName.c_str());
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
         GLfloat x2, z2;
         int total;
         fscanf(file, "%f %f %d",&x2, &z2, &total);
         addParticle(type, X, Z, x2, z2, total, buffer);
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
void partSystem::saveToFile(string fileName)
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

   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
      {
         waterfall[i]->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n",PART_WATERFALL, X, Y, Z,
                                       waterfall[i]->getFileName().c_str());
         if(waterfall[i]->getTotalPlanes() > 0)
         {
            fprintf(file,"%d\n",waterfall[i]->getTotalPlanes());
         }
         for(p = 0; p < waterfall[i]->getTotalPlanes(); p++)
         {
            plane = waterfall[i]->getPlane(p);
            fprintf(file,"%f %f %f %f %f %f %f %f %d\n", plane->x1, plane->y1, 
                    plane->z1, plane->x2, plane->y2, plane->z2, plane->dX, 
                    plane->dZ, plane->inclination);
         }
      }
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
      {
         fire[i]->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n",PART_FIRE, X, Y, Z,
                                       fire[i]->getFileName().c_str());
      }
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
      {
         waterSurface[i]->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n",PART_WATER_SURFACE, X, Y, Z,
                                       waterSurface[i]->getFileName().c_str());
      }
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
      {
         smoke[i]->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n",PART_SMOKE, X, Y, Z,
                                       smoke[i]->getFileName().c_str());
      }
   }

   /* Don't Save Blood Particles */
   /*for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
   }*/

   /* Don't Save Lightning Particles */
   /*for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
   }*/

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
      {
         snow[i]->getPosition(X, Y, Z);
         fprintf(file,"%d %f %f %f %s\n",PART_SNOW, X, Y, Z,
                                       snow[i]->getFileName().c_str());
      }
   }

   for(i = 0; i < MAX_GRASS; i++)
   {
      if(grassParticles[i] != NULL)
      {
         GLfloat x1,x2,z1,z2;
         grassParticles[i]->getPosition(x1,z1,x2,z2);
         string name = grassParticles[i]->getGrassFileName();
         fprintf(file,"%d %f %f %f %s\n",PART_GRASS, x1, 0.0, z1,
                                  name.c_str());
         fprintf(file,"%f %f %d\n", x2, z2, 
                                    grassParticles[i]->getMaxParticles());
      }
   }

   
   fclose(file);
}


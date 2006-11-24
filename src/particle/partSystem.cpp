/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "partSystem.h"

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

}

partSystem::~partSystem()
{
   deleteAll();
}

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
}

void partSystem::actualizeAll(float PCposX, float PCposZ, GLfloat matriz[6][4])
{
   int i;
   
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
   }

   return;
}

void partSystem::stabilizeAll()
{
   int i;
   GLfloat matriz[6][4];  //not needed to draw, so...
   
   for(i=0; i< PART_STABILIZE_LOOP;i++)
   {
      actualizeAll(0,0,matriz);
   }
}

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

   return(total);
}

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

/*   particula = (part1*) addParticle(PART_WATERFALL,150,60,120,
                                            "../data/particles/waterfall1.par");
   particula->addPlane(148,59,118,152,59,123,-1,0,PLANE_NO_INCLINATION);
   particula->addPlane(150,40,118,160,32,123,-1,0,PLANE_INCLINATION_X);
   particula->addPlane(160,20,110,175,20,130,-1,0,PLANE_NO_INCLINATION);*/

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
      else
      {
         addParticle(type, X, Y, Z, buffer);
      }
   }
   fclose(file);
}

#if 0
void partSystem::saveToFile(string fileName)
{
   FILE* file; 
   int type; GLfloat X,Y,Z;
   char buffer[150];

   deleteAll();

   /*Remove it*/
   part1* particula;

   particula = (part1*) addParticle(PART_WATERFALL,150,60,120,
                                            "../data/particles/waterfall1.par");
   particula->addPlane(148,59,118,152,59,123,-1,0,PLANE_NO_INCLINATION);
   particula->addPlane(150,40,118,160,32,123,-1,0,PLANE_INCLINATION_X);
   particula->addPlane(160,20,110,175,20,130,-1,0,PLANE_NO_INCLINATION);

   /*Remove */

   if(!(file=fopen(fileName.c_str(),"r")))
   {
       printf("Error while opening Map particle file: %s\n",fileName.c_str());
       return;
   }
   while(fscanf(file,"%d %f %f %f %s",&type,&X,&Y,&Z,&buffer[0]) != EOF)
   {
      addParticle(type, X, Y, Z, buffer);
   }
   fclose(file);
}
#endif


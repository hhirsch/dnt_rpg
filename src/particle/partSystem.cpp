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
   int i;
   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
         delete(waterfall[i]);
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
         delete(fire[i]);
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
         delete(waterSurface[i]);
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
         delete(smoke[i]);
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
         delete(blood[i]);
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
         delete(lightning[i]);
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
        delete(snow[i]);
   }

}

void partSystem::actualizeAll()
{
   int i;
   
   for(i = 0; i < MAX_WATERFALL; i++)
   {
      if( waterfall[i] != NULL)
         waterfall[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_FIRE; i++)
   {
      if(fire[i] != NULL)
         fire[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_WATER_SURFACE; i++)
   {
      if(waterSurface[i] != NULL)
         waterSurface[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_SMOKE; i++)
   {
      if(smoke[i] != NULL)
         smoke[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_BLOOD; i++)
   {
      if(blood[i] != NULL)
         blood[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_LIGHTNING; i++)
   {
      if(lightning[i] != NULL)
         lightning[i]->NextStep(0.0);
   }

   for(i = 0; i < MAX_SNOW; i++)
   {
      if(snow[i] != NULL)
        snow[i]->NextStep(0.0);
   }
}

particle* partSystem::addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                                  string fileName )
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
                 return((particle*)waterfall[i]);
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
                 return((particle*)fire[i]);
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
                 return((particle*)waterSurface[i]);
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
                 return((particle*)smoke[i]);
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
                 return((particle*)blood[i]);
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
                 return((particle*)lightning[i]);
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
                 return((particle*)snow[i]);
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

void partSystem::removeParticle(int type, particle* part)
{ 
   int i;

   switch(type)
   {
       case PART_WATERFALL:
          for(i = 0; i < MAX_WATERFALL; i++)
          {
             if( (particle*)waterfall[i] == part)
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
             if( (particle*)fire[i] == part)
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
             if( (particle*)waterSurface[i] == part)
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
             if( (particle*)smoke[i] == part)
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
             if( (particle*)blood[i] == part)
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
             if( (particle*)lightning[i] == part)
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
             if( (particle*)snow[i] == part)
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

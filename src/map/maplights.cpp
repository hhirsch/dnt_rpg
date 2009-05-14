#include "maplights.h"
#include "../engine/util.h"
#include "../etc/dirs.h"
#include <math.h>

/************************************************************
 *                        Constructor                       *
 ************************************************************/
mapLight::mapLight()
{
   enableLight = false;
   enableDiffuse = false;  
   enableSpecular = false;
   enableAmbient = false; 
   enableSpot = false;
   enableAtenuation = false;
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
mapLight::~mapLight()
{
}

/************************************************************
 *                        Constructor                       *
 ************************************************************/
mapLights::mapLights()
{
   int i;
   fileName = "";
   for(i=0; i < 3; i++)
   {
      activeLights[i].lightNumber = -1;
      activeLights[i].distance = (-1*i)-1;
   }
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
mapLights::~mapLights()
{
}

/************************************************************
 *                            Load                          *
 ************************************************************/
void mapLights::load(string arq)
{
   int curLight = -1;
   GLfloat aux0,aux1,aux2,aux3;
   char tipo;
   FILE* file;
   dirs dir;
   totalLights = 0;

   if(!(file=fopen(dir.getRealFile(arq).c_str(),"r")))
   {
       printf("Error while opening lights: %s\n",
              dir.getRealFile(arq).c_str());
       return;
   }

   while(fscanf(file,"%c %f %f %f %f",&tipo,&aux0,&aux1,&aux2,&aux3) != EOF)
   {
      switch(tipo)
      {
         case 'L':/* new light */
            curLight++;
            if(curLight > MAX_LIGHTS_PER_MAP)
            {
               printf("Error: Lights Overflow!\n");
            }
            light[curLight].enableLight = true;
         break;
         case 'a':/* ambient */
           light[curLight].light_ambient[0] = aux0;
           light[curLight].light_ambient[1] = aux1;
           light[curLight].light_ambient[2] = aux2;
           light[curLight].light_ambient[3] = aux3;
           light[curLight].enableAmbient = true;
         break;
         case 'd':/* diffuse */
           light[curLight].light_diffuse[0] = aux0;
           light[curLight].light_diffuse[1] = aux1;
           light[curLight].light_diffuse[2] = aux2;
           light[curLight].light_diffuse[3] = aux3;
           light[curLight].enableDiffuse = true;
         break;
         case 's':/* specular */
           light[curLight].light_specular[0] = aux0;
           light[curLight].light_specular[1] = aux1;
           light[curLight].light_specular[2] = aux2;
           light[curLight].light_specular[3] = aux3;
           light[curLight].enableSpecular = true;
         break;
         case 'p':/* position */
           light[curLight].light_position[0] = aux0;
           light[curLight].light_position[1] = aux1;
           light[curLight].light_position[2] = aux2;
           light[curLight].light_position[3] = aux3;
         break;
         case 'i':/* direction */
           light[curLight].light_direction[0] = aux0;
           light[curLight].light_direction[1] = aux1;
           light[curLight].light_direction[2] = aux2;
           light[curLight].light_direction[3] = 1.0;
           light[curLight].cutOff = aux3;
           light[curLight].enableSpot = true;
         break;
         case 'q':/* Atenuation */
           light[curLight].constantAtenuation = aux0;
           light[curLight].linearAtenuation = aux1;
           light[curLight].quadricAtenuation = aux2;
           light[curLight].enableAtenuation = true;
         break;
      }
   }

   totalLights = curLight+1;
   if(totalLights <= 3 )
   {
      /* Only set the lights one time */
      setNearLights(0,0);
   }

   fclose(file);
   fileName = arq;
}

/************************************************************
 *                       setNearLights                      *
 ************************************************************/
void mapLights::setNearLights(GLfloat posX, GLfloat posZ)
{
   GLfloat auxD, prvD;
   int l,auxN,prvN; /* Counters and Auxiliars */
   GLfloat dist = 0;
   for(l=0; l< MAX_LIGHTS_PER_MAP; l++)
   {
      if(light[l].enableLight)
      {
         dist = sqrt( (posX - light[l].light_position[0]) *
                      (posX - light[l].light_position[0]) +
                      (posZ - light[l].light_position[2]) *
                      (posZ - light[l].light_position[2]));
         if( (activeLights[0].distance >= dist) || 
             (activeLights[0].lightNumber < 0) ||
             (activeLights[0].lightNumber == l) )
         {
            /* Change the Light and Shift It */
            if(activeLights[0].lightNumber != l)
            {
               prvN = activeLights[0].lightNumber;
               prvD = activeLights[0].distance;
               activeLights[0].lightNumber = l;
               activeLights[0].distance = dist;
               auxN = activeLights[1].lightNumber;
               auxD = activeLights[1].distance;
               activeLights[1].lightNumber = prvN;
               activeLights[1].distance = prvD;
               activeLights[2].lightNumber = auxN;
               activeLights[2].distance = auxD;
            }
         }
         else if((activeLights[1].distance >= dist) ||
                 (activeLights[1].lightNumber < 0) ||
                 (activeLights[1].lightNumber == l))
         {
            /* Change and shift */
            if(activeLights[1].lightNumber != l)
            {
               prvN = activeLights[1].lightNumber;
               prvD = activeLights[1].distance;
               activeLights[1].lightNumber = l;
               activeLights[1].distance = dist;
               activeLights[2].lightNumber = prvN;
               activeLights[2].distance = prvD;
            }
         }
         else if((activeLights[2].distance >= dist) ||
                 (activeLights[2].lightNumber < 0) ||
                 (activeLights[2].lightNumber == l))
         {
            /* Change Light */
            if(activeLights[2].lightNumber != l)
            {
               activeLights[2].lightNumber = l;
               activeLights[2].distance = dist;
            }
         }
      }
   }
}

/************************************************************
 *                         actualize                        *
 ************************************************************/
void mapLights::update(GLfloat posX, GLfloat posZ)
{
   int i,l;
   int gLight = 0;

   if(totalLights > 3)
   {
      setNearLights(posX, posZ);
   }
   for(i=0; i<3; i++)
   {
      switch(i)
      {
         case 0: 
            gLight = GL_LIGHT1;
         break;
         case 1: 
            gLight = GL_LIGHT2;
         break;
         case 2: 
           gLight = GL_LIGHT3;
         break;
      }
      if( (activeLights[i].lightNumber >= 0) && 
          (light[activeLights[i].lightNumber].enableLight) &&
          (activeLights[i].distance <= 2*INDOOR_FARVIEW))
      {
         l = activeLights[i].lightNumber;
         /*glDisable(GL_LIGHTING);
         glColor3f(1.0,0.2,0.2);
         glBegin(GL_QUADS);
            glVertex3f(light[l].light_position[0]-5, light[l].light_position[1],
                       light[l].light_position[2]-5);
            glVertex3f(light[l].light_position[0]-5, light[l].light_position[1],
                       light[l].light_position[2]+5);
            glVertex3f(light[l].light_position[0]+5, light[l].light_position[1],
                       light[l].light_position[2]+5);
            glVertex3f(light[l].light_position[0]+5, light[l].light_position[1],
                       light[l].light_position[2]-5);
         glEnd();
         glEnable(GL_LIGHTING);*/
         /* Define Position */
         glLightfv(gLight, GL_POSITION, light[l].light_position);
         /* Ambient */
         if(light[l].enableAmbient)
         {
            glLightfv(gLight, GL_AMBIENT, light[l].light_ambient);
         }
         /* Diffuse */
         if(light[l].enableDiffuse)
         {
            glLightfv(gLight, GL_DIFFUSE, light[l].light_diffuse);
         }
         /* Specular */
         if(light[l].enableSpecular)
         {
            glLightfv(gLight, GL_SPECULAR, light[l].light_specular);
         }
         /* Specular */
         if(light[l].enableSpot)
         {
            glLightfv(gLight, GL_SPOT_DIRECTION,
                      light[l].light_direction);
            glLightf(gLight, GL_SPOT_CUTOFF, light[l].cutOff);
            glLightf(gLight, GL_SPOT_EXPONENT, 2.0);
         }
         /* Atenuation */
         if(light[l].enableAtenuation)
         {
            glLightf(gLight, GL_CONSTANT_ATTENUATION, 
                     light[l].constantAtenuation);
            glLightf(gLight, GL_LINEAR_ATTENUATION, 
                     light[l].linearAtenuation);
            glLightf(gLight, GL_QUADRATIC_ATTENUATION, 
                     light[l].quadricAtenuation);
         }
         glEnable(gLight);
      }
      else
      {
         glDisable(gLight);
      }
   }
}

/************************************************************
 *                        getFileName                       *
 ************************************************************/
string mapLights::getFileName()
{
   return(fileName);
}



#include "maplights.h"

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
void mapLights::Load(string arq)
{
   int curLight = -1;
   GLfloat aux0,aux1,aux2,aux3;
   char tipo;
   FILE* file;

   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening lights: %s\n",arq.c_str());
       return;
   }

   while(fscanf(file,"%c %f %f %f %f",&tipo,&aux0,&aux1,&aux2,&aux3) != EOF)
   {
      switch(tipo)
      {
         case 'L':/* new light */
            curLight++;
            light[curLight].enableLight = true;
            /* Select Correct Light */
            switch(curLight)
            {
               case 0: 
                  light[curLight].Glight = GL_LIGHT1;
               break;
               case 1: 
                  light[curLight].Glight = GL_LIGHT2;
               break;
               case 2: 
                  light[curLight].Glight = GL_LIGHT3;
               break;
               case 3: 
                  light[curLight].Glight = GL_LIGHT4;
               break;
               case 4: 
                  light[curLight].Glight = GL_LIGHT5;
               break;
            }
            glEnable(light[curLight].Glight);
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

   fclose(file);

   /* Disable All Unused Lights */
   curLight++;
   while(curLight < 5)
   {
      /* Select Correct Light */
      switch(curLight)
      {
         case 0: 
            light[curLight].Glight = GL_LIGHT1;
         break;
         case 1: 
            light[curLight].Glight = GL_LIGHT2;
         break;
         case 2: 
            light[curLight].Glight = GL_LIGHT3;
         break;
         case 3: 
            light[curLight].Glight = GL_LIGHT4;
         break;
         case 4: 
            light[curLight].Glight = GL_LIGHT5;
         break;
      }
      glDisable(light[curLight].Glight);
      curLight++;
   }
}

/************************************************************
 *                         actualize                        *
 ************************************************************/
void mapLights::actualize()
{
   int l;
   for(l=0; l<5; l++)
   {
      if(light[l].enableLight)
      {
         glDisable(GL_LIGHTING);
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
         glEnable(GL_LIGHTING);
         /* Define Position */
         glLightfv(light[l].Glight, GL_POSITION, light[l].light_position);
         /* Ambient */
         if(light[l].enableAmbient)
         {
            glLightfv(light[l].Glight, GL_AMBIENT, light[l].light_ambient);
         }
         /* Diffuse */
         if(light[l].enableDiffuse)
         {
            glLightfv(light[l].Glight, GL_DIFFUSE, light[l].light_diffuse);
         }
         /* Specular */
         if(light[l].enableSpecular)
         {
            glLightfv(light[l].Glight, GL_SPECULAR, light[l].light_specular);
         }
         /* Specular */
         if(light[l].enableSpot)
         {
            glLightfv(light[l].Glight, GL_SPOT_DIRECTION,
                      light[l].light_direction);
            glLightf(light[l].Glight, GL_SPOT_CUTOFF, light[l].cutOff);
            glLightf(light[l].Glight, GL_SPOT_EXPONENT, 2.0);
         }
         /* Atenuation */
         if(light[l].enableAtenuation)
         {
            glLightf(light[l].Glight, GL_CONSTANT_ATTENUATION, 
                     light[l].constantAtenuation);
            glLightf(light[l].Glight, GL_LINEAR_ATTENUATION, 
                     light[l].linearAtenuation);
            glLightf(light[l].Glight, GL_QUADRATIC_ATTENUATION, 
                     light[l].quadricAtenuation);
         }
      }
   }
}


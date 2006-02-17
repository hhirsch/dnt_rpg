#include "maplights.h"

mapLight::mapLight()
{
   enableLight = false;
}

mapLight::~mapLight()
{
}

mapLights::mapLights()
{
}

mapLights::~mapLights()
{
}

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

   while(fscanf(file,"%c %f %f %f %f",&tipo,&aux0,&aux1,&aux2,&aux3))
   {
      switch(tipo)
      {
         case 'l':/* new light */
            curLight++;
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
           light[curLight].light_direction[3] = aux3;
           light[curLight].enableSpot = true;
         break;
      }
   }

   fclose(file);
}


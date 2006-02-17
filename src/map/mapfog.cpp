#include "mapfog.h"

mapFog::mapFog()
{
   enabled = false;
}

mapFog::~mapFog()
{
}


void mapFog::Load(string arq)
{
   FILE* file;

   if(!(file=fopen(arq.c_str(),"r")))
   {
       printf("Error while opening fog: %s\n",arq.c_str());
       return;
   }
   fscanf(file,"%f %f %f %f",&color[0],&color[1],&color[2],&color[3]);
   fscanf(file,"%f",&density);
   fscanf(file,"%f",&start);
   fscanf(file,"%f",&end);
   enabled = true;

   fclose(file);
}


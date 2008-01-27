#include "mapfog.h"
#include "../etc/dirs.h"

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
mapFog::mapFog()
{
   int i;
   for(i=0; i < 4; i++)
   {
      color[i] = 0;
   }
   density = 0;
   start = 0;
   end = 0;
   enabled = false;
   fileName = "";
}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
mapFog::~mapFog()
{
   fileName = "";
}

/****************************************************************
 *                             Load                             *
 ****************************************************************/
void mapFog::Load(string arq)
{
   FILE* file;
   dirs dir;

   if(!(file=fopen(dir.getRealFile(arq).c_str(),"r")))
   {
       printf("Error while opening fog: %s\n",arq.c_str());
       return;
   }
   fileName = arq;
   fscanf(file,"%f %f %f %f",&color[0],&color[1],&color[2],&color[3]);
   fscanf(file,"%f",&density);
   fscanf(file,"%f",&start);
   fscanf(file,"%f",&end);
   enabled = true;

   fclose(file);
}

/****************************************************************
 *                             save                             *
 ****************************************************************/
bool mapFog::save()
{
   FILE* file;

   if(!(file=fopen(fileName.c_str(),"w")))
   {
       printf("Error while opening fog: %s\n",fileName.c_str());
       return(false);
   }
   fprintf(file,"%f %f %f %f\n",color[0],color[1],color[2],color[3]);
   fprintf(file,"%f\n",density);
   fprintf(file,"%f\n",start);
   fprintf(file,"%f\n",end);
   fclose(file);
   return(true);
}


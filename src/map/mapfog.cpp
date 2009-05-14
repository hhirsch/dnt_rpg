#include "mapfog.h"
#include "../etc/dirs.h"

#include <iostream>
#include <fstream>
using namespace std;

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
void mapFog::load(string arq)
{
   ifstream file;
   string buffer;
   dirs dir;

   file.open(dir.getRealFile(arq).c_str(), ios::in | ios::binary);

   if(!file)
   {
       cerr << "Error while opening fog file: " << arq << endl;
       return;
   }
   fileName = arq;
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f %f %f %f",&color[0],&color[1],&color[2],&color[3]);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&density);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&start);
   getline(file, buffer);
   sscanf(buffer.c_str(),"%f",&end);
   enabled = true;

   file.close();
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


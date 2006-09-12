#include "wave.h"

waves::waves(string waveFile)
{
   actualWaves = NULL;
}

waves::~waves()
{
   while(actualWaves)
   {
      removeWave(actualWaves);
   }
}

void waves::removeDeadWaves()
{
   int i;
   wave* aux = actualWaves;
   for(i=0; i < getTotalWaves(); i++)
   {
      if(aux->dead)
      {
         aux = aux->next;
         removeWave(aux->previous);
      }
      else
      {
         aux = aux->next;
      }
   }
}

void waves::insertWave(int initialX, int initialZ, int initialAmplitude,
                      int deltaAmplitude, int generationTime, int attrition)
{
   wave* aux;

   aux = new(wave);
   aux->initialX = initialX;
   aux->initialZ = initialZ;
   aux->initialAmplitude = initialAmplitude;
   aux->deltaAmplitude = deltaAmplitude;
   aux->generationTime = generationTime;
   aux->attrition = attrition;
   aux->nearLoop = 0;
   aux->farLoop = 0;
   aux->actualLifeTime = 0;
   aux->dead = false;

   if(!actualWaves)
   {
      aux->next = aux;
      aux->previous = aux;
      actualWaves = aux;
   }
   else
   {
      aux->next = actualWaves;
      aux->previous = actualWaves->previous;
      aux->previous->next = aux;
      actualWaves->previous = aux;
      actualWaves = aux;
   }
   
   totalWaves++;
}

void waves::removeWave(wave* remWave)
{
   if(remWave)
   {
      if(getTotalWaves() > 1)
      {
         remWave->next->previous = remWave->previous;
         remWave->previous->next = remWave->next;
         if(remWave == actualWaves)
         {
            actualWaves = remWave->next;
         }
      }
      else
      {
         actualWaves = NULL;
      }
      delete(remWave);
      totalWaves--;
   }
}

int waves::getTotalWaves()
{
   return(totalWaves);
}



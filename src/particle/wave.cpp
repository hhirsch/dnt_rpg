#include "wave.h"
#include <math.h>

#define SURFACE_VX 8
#define SURFACE_VZ 8

/**********************************************************************
 *                             Constructor                            *
 **********************************************************************/
waves::waves(string waveFile, GLfloat X, GLfloat Y, GLfloat Z, int nX, int nZ)
{
   int i,w;
   actualWaves = NULL;
   surfX = nX;
   surfZ = nZ;
   initialX = X;
   initialY = Y;
   initialZ = Z;
   surface = (position**) malloc(nX*sizeof(position*));
   for(i = 0; i < surfX; i++)
   {
      surface[i] = (position*) malloc(nZ*sizeof(position));
   }

   for(i=0; i < surfX; i++)
   {
      for(w=0; w < surfZ; w++)
      {
         surface[i][w].x = i*SURFACE_VX;
         surface[i][w].y = 0;
         surface[i][w].z = w*SURFACE_VZ;
      }
   }
}

/**********************************************************************
 *                              Destructor                            *
 **********************************************************************/
waves::~waves()
{
   int i;
   while(actualWaves)
   {
      removeWave(actualWaves);
   }
   for(i = 0; i < surfX; i++)
   {
      free(surface[i]);
   }
   free(surface);
}

/**********************************************************************
 *                            removeDeadWaves                         *
 **********************************************************************/
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

/**********************************************************************
 *                             insertWave                             *
 **********************************************************************/
void waves::insertWave(int initialX, int initialZ, 
                       GLfloat amplitude, GLfloat deltaAmplitude, 
                       int generationTime, int attrition, int direction)
{
   wave* aux;

   aux = new(wave);
   aux->initialX = initialX;
   aux->initialZ = initialZ;
   aux->amplitude = amplitude;
   aux->deltaAmplitude = deltaAmplitude;
   aux->generationTime = generationTime;
   aux->attrition = attrition;
   aux->direction = direction;
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

/**********************************************************************
 *                            removeWave                              *
 **********************************************************************/
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

/**********************************************************************
 *                             getTotalWaves                          *
 **********************************************************************/
int waves::getTotalWaves()
{
   return(totalWaves);
}

/**********************************************************************
 *                                doStep                              *
 **********************************************************************/
void waves::doStep()
{
   int i, w;
   int vX = 0, vZ = 0;
   wave* aux = actualWaves;

   GLfloat x = 0,y = 0,z = 0;

   /* First, reset Surface */
   for(i=0; i < surfX; i++)
   {
      for(w=0; w < surfZ; w++)
      {
         surface[i][w].px = surface[i][w].x;
         surface[i][w].x = i*SURFACE_VX;
         surface[i][w].py = surface[i][w].y;
         surface[i][w].y = 0;
         surface[i][w].pz = surface[i][w].z;
         surface[i][w].z = w*SURFACE_VZ;
      }
   }
   
   for(i=0; i < getTotalWaves(); i++)
   {
      if(aux->dead)
      {
         /* Remove Dead Waves */
         aux = aux->next;
         removeWave(aux->previous);
      }
      else
      {
         /* Only Actualize Living Waves */
         aux->dead = true; //mark as dead unless is alive after
         aux->actualLifeTime++;
         if(aux->generationTime >= aux->actualLifeTime)
         {
            aux->nearLoop = 0;
         }
         else
         {
            aux->nearLoop++;
         }
         aux->farLoop++;

         for(w = aux->nearLoop; w < aux->farLoop; w++)
         {
            int mod;
            y = w*aux->deltaAmplitude + aux->actualLifeTime*aux->deltaAmplitude;
            mod = (int(y / (2*aux->amplitude)) * 
                  (int)(2*aux->amplitude));
            y = y - mod/* - w*aux->attrition*/;

            if(y >= 0)
            {
               y = y - aux->amplitude;
               if(aux->direction == WAVE_DIRECTION_UP)
               {
                  vX = aux->initialX;
                  vZ = aux->initialZ - w;
                  z = sqrt((aux->amplitude*aux->amplitude)+(y*y));
                  x = vX*SURFACE_VX;
               }
               else if(aux->direction == WAVE_DIRECTION_DOWN)
               {
                  vX = aux->initialX;
                  vZ = aux->initialZ + w;
                  z = sqrt((aux->amplitude*aux->amplitude)+(y*y));
                  x = vX*SURFACE_VZ;
               }
               else if(aux->direction == WAVE_DIRECTION_LEFT)
               {
                  vX = aux->initialX - w;
                  vZ = aux->initialZ;
                  x = sqrt((aux->amplitude*aux->amplitude)+(y*y));
                  z = vZ*SURFACE_VZ;
               }
               else if(aux->direction == WAVE_DIRECTION_RIGHT)
               {
                  vX = aux->initialX + w;
                  vZ = aux->initialZ;
                  x = sqrt((aux->amplitude*aux->amplitude)+(y*y));
                  z = vZ*SURFACE_VZ;
               }

               /* If Vertex Exists */
               if( (vX >= 0) && (vX < surfX) && (vZ >= 0) && (vZ < surfZ))
               {
                  aux->dead = false;
                  if(y < surface[vX][vZ].py)
                  {
                     if( (aux->direction == WAVE_DIRECTION_LEFT) ||
                         (aux->direction == WAVE_DIRECTION_RIGHT))
                     {
                        x = -x + vX*SURFACE_VX;
                     }
                     else
                     {
                        z = -z + vZ*SURFACE_VZ;
                     }
                  }
                  else
                  {
                     if( (aux->direction == WAVE_DIRECTION_LEFT) ||
                         (aux->direction == WAVE_DIRECTION_RIGHT))
                     {
                        x += vX*SURFACE_VX;
                     }
                     else
                     {
                        z += vZ*SURFACE_VZ;
                     }
                  }
                  surface[vX][vZ].x += x;
                  surface[vX][vZ].y += y;
                  surface[vX][vZ].z += z;
               }
               else
               {
                  //TODO wave colision with end of the surface (create one with 
                  // opost direction)
               }
            }
         }

         
         aux = aux->next;
      }
   }
}

/**********************************************************************
 *                                 draw                               *
 **********************************************************************/
void waves::draw()
{
   int x,z;
   glBegin(GL_QUADS);

   for(x=0; x < (surfX-1); x++)
   {
      for(z = 0; z < (surfZ-1); z++)
      {
        glVertex3f(surface[x][z].x + initialX,
                   surface[x][z].y + initialY,
                   surface[x][z].z + initialZ);
        glVertex3f(surface[x+1][z].x + initialX,
                   surface[x+1][z].y + initialY,
                   surface[x+1][z].z + initialZ);
        glVertex3f(surface[x+1][z+1].x + initialX,
                   surface[x+1][z+1].y + initialY,
                   surface[x+1][z+1].z + initialZ);
        glVertex3f(surface[x][z+1].x + initialX,
                   surface[x][z+1].y + initialY,
                   surface[x][z+1].z + initialZ);
      }
   }
   
   glEnd();
}


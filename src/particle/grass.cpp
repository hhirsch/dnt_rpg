#include "grass.h"
#include "../gui/draw.h"
#include "../map/map.h"
#include "../engine/util.h"

/**************************************************************************
 *                             Constructor                                *
 **************************************************************************/
grass::grass(float cX1,float cZ1, float cX2, float cZ2, int total, 
             float scale, string fileName)
                                :particleSystem(total,PARTICLE_DRAW_INDIVIDUAL)
{
   SDL_Surface* img;
   centerX1 = cX1;
   centerX2 = cX2;
   centerZ1 = cZ1;
   centerZ2 = cZ2;
   scaleFactor = scale;
   usedMap = NULL;


   /* Load Texture */
   grassFileName = fileName;
   img = IMG_Load(fileName.c_str());
   if(!img)
   {
      printf("Error when loading grass! Maybe Crash Soon!\n File: %s\n",
             fileName.c_str());
   }
   glGenTextures(1, &(grassTexture));
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->w,img->h, 
                0,GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR/*_MIPMAP_NEAREST*/);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->w,
                     img->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                     img->pixels );*/
   
   SDL_FreeSurface(img);

   /* Alloc Structs */
   partPosition = (quadPos*) new quadPos[total];
   vertexArray = (float*) new float[total*3*4];
   textureArray = (float*) new float[total*2*4];
}

/**************************************************************************
 *                              Destructor                                *
 **************************************************************************/
grass::~grass()
{
   glDeleteTextures(1, &grassTexture);
   usedMap = NULL;

   /* Dealloc structs */
   if(partPosition)
   {
      delete[] partPosition;
   }
   if(vertexArray)
   {
      delete[] vertexArray;
   }
   if(textureArray)
   {
      delete[] textureArray;
   }
}

/**************************************************************************
 *                                Render                                  *
 **************************************************************************/
void grass::Render(particle* part)
{
   int n = part->internalNumber;

   /*glTexCoord2f(0.0,0.0);
   glVertex3f(partPosition[n].x1 + part->posX, 
              partPosition[n].y1,
              partPosition[n].z1 + part->posZ);
   glTexCoord2f(1.0,0.0);
   glVertex3f(partPosition[n].x2 + part->posX, 
              partPosition[n].y2,
              partPosition[n].z2 + part->posZ);
   glTexCoord2f(1.0,1.0);
   glVertex3f(partPosition[n].x3 + part->posX, 
              partPosition[n].y3,
              partPosition[n].z3 + part->posZ);
   glTexCoord2f(0.0,1.0);
   glVertex3f(partPosition[n].x4 + part->posX, 
              partPosition[n].y4,
              partPosition[n].z4 + part->posZ);*/

   textureArray[tArrayPos] = 0.0;
   textureArray[tArrayPos+1] = 0.0;
   vertexArray[vArrayPos] = partPosition[n].x1 + part->posX;
   vertexArray[vArrayPos+1] = partPosition[n].y1;
   vertexArray[vArrayPos+2] = partPosition[n].z1 + part->posZ;

   textureArray[tArrayPos+2] = 1.0;
   textureArray[tArrayPos+3] = 0.0;
   vertexArray[vArrayPos+3] = partPosition[n].x2 + part->posX;
   vertexArray[vArrayPos+4] = partPosition[n].y2;
   vertexArray[vArrayPos+5] = partPosition[n].z2 + part->posZ;

   textureArray[tArrayPos+4] = 1.0;
   textureArray[tArrayPos+5] = 1.0;
   vertexArray[vArrayPos+6] = partPosition[n].x3 + part->posX;
   vertexArray[vArrayPos+7] = partPosition[n].y3;
   vertexArray[vArrayPos+8] = partPosition[n].z3 + part->posZ;

   textureArray[tArrayPos+6] = 0.0;
   textureArray[tArrayPos+7] = 1.0;
   vertexArray[vArrayPos+9] = partPosition[n].x4 + part->posX;
   vertexArray[vArrayPos+10] = partPosition[n].y4;
   vertexArray[vArrayPos+11] = partPosition[n].z4 + part->posZ;

   vArrayPos += 12;
   tArrayPos += 8;

   /*glTexCoord2f(0.0,0.0);
   glVertex3f(part->posX - (part->velX * scaleFactor),
              part->posY,
              part->posZ + (part->velZ  * part->velY * scaleFactor));
   glTexCoord2f(1.0,0.0);
   glVertex3f(part->posX + (part->velX * scaleFactor),
              part->prvX,
              part->posZ - (part->velZ * part->velY * scaleFactor));
   glTexCoord2f(1.0,1.0);
   glVertex3f(part->posX + (part->velX * scaleFactor),
              part->prvY,
              part->posZ - (part->velZ * scaleFactor));
   glTexCoord2f(0.0,1.0);
   glVertex3f(part->posX - (part->velX * scaleFactor),
              part->prvZ,
              part->posZ + (part->velZ * scaleFactor));*/
}

/**************************************************************************
 *                              InitRender                                *
 **************************************************************************/
void grass::InitRender()
{
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER,0.1f);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   vArrayPos = 0;
   tArrayPos = 0;
   //glBegin(GL_QUADS);
}

/**************************************************************************
 *                              EndRender                                 *
 **************************************************************************/
void grass::EndRender()
{
   //glEnd();


   /* Draw as arrays */
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, textureArray);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertexArray);
   glDrawArrays(GL_QUADS, 0, tArrayPos / 2);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

   /* Undo the changes on the GL state */
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_ALWAYS,0.0f);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);

}

/**************************************************************************
 *                              actualize                                 *
 **************************************************************************/
void grass::actualize(particle* part)
{
   part->R += part->size;
   part->velY = sin(deg2Rad(part->R));


   if(part->R <= part->prvR-40)
   {
      part->size = 0.75;
   }
   else if(part->R >= part->prvR+40)
   {
      part->size = -0.75;
   }
}

/**************************************************************************
 *                            continueLive                                *
 **************************************************************************/
bool grass::continueLive(particle* part)
{
   return( true );
}

/**************************************************************************
 *                             needCreate                                 *
 **************************************************************************/
int grass::needCreate()
{
   if(actualParticles == 0)
   {
      return(maxParticles);
   }
   return(0);
}

/**************************************************************************
 *                           createParticle                               *
 **************************************************************************/
void grass::createParticle(particle* part)
{
   /* Define all Rotations */
   part->R = 75*(rand() / ((double)RAND_MAX + 1));
   part->B = 15*(rand() / ((double)RAND_MAX + 1));

   part->velY = sin(deg2Rad(part->R));
   
   /* Define Rotation variation */
   if(rand() > 0.5)
   {
      part->size = -0.75;
   }
   else
   {
      part->size = 0.75;
   }

   /* Define previous value of actualized rotation */
   part->prvR = part->R;

   /* Define Particle Size and Position */
   defineSize(part);
}

/**************************************************************************
 *                               NextStep                                 *
 **************************************************************************/
void grass::NextStep(GLfloat matriz[6][4], 
                     GLfloat pcPosX, GLfloat pcPosY, GLfloat pcPosZ,
                     wind& affectWind)
{
   seconds = 0.02;

   //TODO get wind and character position

   DoStep(matriz);
}

/**************************************************************************
 *                             numParticles                               *
 **************************************************************************/
int grass::numParticles()
{
   return(actualParticles);
}

/**************************************************************************
 *                             getPosition                                *
 **************************************************************************/
void grass::getPosition(GLfloat& cX1, GLfloat& cZ1, GLfloat& cX2, GLfloat& cZ2 )
{
   cX1 = centerX1;
   cX2 = centerX2;
   cZ1 = centerZ1;
   cZ2 = centerZ2;
}

/**************************************************************************
 *                           getGrassFileName                             *
 **************************************************************************/
string grass::getGrassFileName()
{
   return(grassFileName);
}

/**************************************************************************
 *                            getScaleFactor                              *
 **************************************************************************/
GLfloat grass::getScaleFactor()
{
   return(scaleFactor);
}

/**************************************************************************
 *                            setScaleFactor                              *
 **************************************************************************/
void grass::setScaleFactor(GLfloat scale)
{
   int i;
   scaleFactor = scale;

   /* Actualize All Particles */
   for(i = 0; i < maxParticles; i++)
   {
      defineSize(&particles[i]);
   }
}

/**************************************************************************
 *                                setTotal                                *
 **************************************************************************/
void grass::setTotal(int total)
{
   /* Delete the alloced structs */
   delete []particles;

   /* Reinit the system with the new total */
   init(total,PARTICLE_DRAW_INDIVIDUAL);
}

/**************************************************************************
 *                              defineSize                                *
 **************************************************************************/
void grass::defineSize(particle* part)
{
   /* Define Position X on Map */
   part->posX = ((centerX2-centerX1)*(rand() / ((double)RAND_MAX + 1)))
                + (centerX1);
   /* Define Position Z on Map */
   part->posZ = ((centerZ2-centerZ1)*(rand() / ((double)RAND_MAX + 1)))
                + (centerZ1);

   /* Define the grass orientation */
   GLfloat orientation = 90*(rand() / ((double)RAND_MAX + 1));

   /* Define fixed Rotations */
     /* Around Y axis */
   GLfloat cosOri = 8 * scaleFactor * cos(deg2Rad(orientation));
   GLfloat sinOri = 8 * scaleFactor * sin(deg2Rad(orientation));

   int n = part->internalNumber;

   /* Do the initial rotation around Y axys */
   partPosition[n].x1 = -cosOri;
   partPosition[n].z1 = -sinOri;
   partPosition[n].x2 = +cosOri;
   partPosition[n].z2 = +sinOri;
   partPosition[n].x3 = +cosOri;
   partPosition[n].z3 = +sinOri;
   partPosition[n].x4 = -cosOri;
   partPosition[n].z4 = -sinOri;

   /* Define Height  */
   partPosition[n].y1 = 10*scaleFactor;
   partPosition[n].y2 = 10*scaleFactor;
   partPosition[n].y3 = 0;
   partPosition[n].y4 = 0;

#if 0
   if(usedMap)
   {
      Map* map = (Map*) usedMap;
      part->posY = map->getHeight(part->posX - 8.0, part->posZ) + 10*scaleFactor;
      part->prvX = map->getHeight(part->posX + 8.0, part->posZ) + 10*scaleFactor;
      part->prvY = map->getHeight(part->posX + 8.0, part->posZ);
      part->prvZ = map->getHeight(part->posX - 8.0, part->posZ);
   }
   else
   {
      part->posY = 10*scaleFactor; /* Used as Height on First Vertex */
      part->prvX = 10*scaleFactor; /* Used as Height on Second Vertex */
      part->prvY = 0; /* Used as Height on Third Vertex */
      part->prvZ = 0; /* Used as Height on Forth Vertex */
   }
#endif
}

/**************************************************************************
 *                              defineMap                                 *
 **************************************************************************/
void grass::defineMap(void* actualMap)
{
   int i;
   usedMap = actualMap;
   Map* mapa = (Map*)actualMap;
   for(i = 0; i < maxParticles; i++)
   {
      if(particles[i].status != PARTICLE_STATUS_DEAD)
      {
         particles[i].posY = mapa->getHeight(particles[i].posX - 8.0, 
                                             particles[i].posZ) + 10;
         particles[i].prvX = mapa->getHeight(particles[i].posX + 8.0, 
                                             particles[i].posZ) + 10;
         particles[i].prvY = mapa->getHeight(particles[i].posX + 8.0, 
                                             particles[i].posZ);
         particles[i].prvZ = mapa->getHeight(particles[i].posX - 8.0, 
                                             particles[i].posZ);
      }
   }
}



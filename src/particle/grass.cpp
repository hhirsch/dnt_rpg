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
}

/**************************************************************************
 *                              Destructor                                *
 **************************************************************************/
grass::~grass()
{
   glDeleteTextures(1, &grassTexture);
   usedMap = NULL;
}

/**************************************************************************
 *                                Render                                  *
 **************************************************************************/
void grass::Render(particle* part)
{
   //FIXME the X rotation when grass is on different height!
   glTexCoord2f(0.0,0.0);
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
              part->posZ + (part->velZ * scaleFactor));
}

/**************************************************************************
 *                              InitRender                                *
 **************************************************************************/
void grass::InitRender()
{
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glBegin(GL_QUADS);
}

/**************************************************************************
 *                              EndRender                                 *
 **************************************************************************/
void grass::EndRender()
{
   glEnd();
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
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
   /* Define Position X on Map */
   part->posX = ((centerX2-centerX1)*(rand() / ((double)RAND_MAX + 1)))
                + centerX1;
   /* Define Position Z on Map */
   part->posZ = ((centerZ2-centerZ1)*(rand() / ((double)RAND_MAX + 1)))
                + centerZ1;

   if(usedMap)
   {
      Map* map = (Map*) usedMap;
      part->posY = map->getHeight(part->posX - 8.0, part->posZ) + 10;
      part->prvX = map->getHeight(part->posX + 8.0, part->posZ) + 10;
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
   
   /* Define all Rotations */
   part->R = 75*(rand() / ((double)RAND_MAX + 1));
   part->G = 90*(rand() / ((double)RAND_MAX + 1));
   part->B = 15*(rand() / ((double)RAND_MAX + 1));

   /* Define fixed Rotations */
     /* Around Y axis */
   part->velZ = 8*cos(deg2Rad(part->G));
   part->velX = 8*sin(deg2Rad(part->G));
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
}

/**************************************************************************
 *                               NextStep                                 *
 **************************************************************************/
void grass::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
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



#include "grass.h"
#include "../gui/desenho.h"

grass::grass(float cX1,float cZ1, float cX2, float cZ2, int total, 
             string fileName):particleSystem(total,PARTICLE_DRAW_INDIVIDUAL)
{
   SDL_Surface* img;
   centerX1 = cX1;
   centerX2 = cX2;
   centerZ1 = cZ1;
   centerZ2 = cZ2;
   //grassModel = glmReadOBJ(fileName.c_str(),"",1);
   img = IMG_Load("../data/models/natural/matos/grass.png");
   if(!img)
   {
      printf("Error when loading grass! Maybe Crash Soon!\n");
   }
   //carregaTexturaRGBA(img, &grassTexture);
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

grass::~grass()
{
   glDeleteTextures(1, &grassTexture);
   //glmDelete(grassModel);
}


void grass::Render(particle* part)
{
   //printf("%f %f %f %f\n",part->posX, part->posZ,part->prvX,part->prvZ  );
   glPushMatrix();
      glTranslatef(part->posX, 0 ,part->posZ);
      glRotatef(part->prvY,0,1,0);
      glRotatef(part->prvX,1,0,0);
      glRotatef(part->prvZ,0,0,1);
      //glNormal3f(0.0,0.0,1.0);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0,0.0);
         glVertex3f(-5.0,10.0,0.0);
         glTexCoord2f(1.0,0.0);
         glVertex3f(5.0,10.0,0.0);
         glTexCoord2f(1.0,1.0);
         glVertex3f(5.0,0.0,0.0);
         glTexCoord2f(0.0,1.0);
         glVertex3f(-5.0,0.0,0.0);
      glEnd();
         
      //glmDraw(modelo);
      //glmDrawLists(grassModel);
  glPopMatrix();
}

void grass::InitRender()
{
   //glDisable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_FALSE);
   //glEnable(GL_CULL_FACE);
   //glEnable(GL_COLOR_MATERIAL);
   
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, grassTexture);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

}

void grass::EndRender()
{
   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
   glDepthMask(GL_TRUE);
   glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
   //glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glEnable(GL_LIGHTING);
}

void grass::actualize(particle* part)
{
}

bool grass::continueLive(particle* part)
{
   return( true );
}

int grass::needCreate()
{
   if(actualParticles == 0)
   {
      return(maxParticles);
   }
   return(0);
}

void grass::createParticle(particle* part)
{
   part->posX = ((centerX2-centerX1)*(rand() / ((double)RAND_MAX + 1)))
                + centerX1;
   part->posY = 0;
   part->posZ = ((centerZ2-centerZ1)*(rand() / ((double)RAND_MAX + 1)))
                + centerZ2;
   
   part->prvX = 15*(rand() / ((double)RAND_MAX + 1));
   part->prvY = 90*(rand() / ((double)RAND_MAX + 1));;
   part->prvZ = 15*(rand() / ((double)RAND_MAX + 1));
   part->size = 1; 
   part->velY = 0.0;
   part->velX = 0.0;
   part->velZ = 0.0;
   part->R = 0;
   part->G = 0;
   part->B = 0;
   part->prvR = 0;
   part->prvG = 0; 
   part->prvB = 0;
}

void grass::NextStep(GLfloat matriz[6][4])
{
   seconds = 0.02;
   DoStep(matriz);
}

int grass::numParticles()
{
   return(actualParticles);
}


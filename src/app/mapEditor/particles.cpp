#include "particles.h"

particles::particles(Map* map)
{
   actualMap = map;
   actualParticle = NULL;
   state = -1;
}

particles::~particles()
{
   state = -1;
   actualMap = NULL;
   deleteParticle();
}

void particles::deleteParticle()
{
   if(actualParticle)
   {
      if(state == TOOL_PARTICLE_FIRE)
      {
         part2* tmp = (part2*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
   }
}

void particles::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                             Uint8 mButton, Uint8* keys, int tool,
                             GLdouble proj[16],GLdouble modl[16],
                             GLint viewPort[4])
{
   if(tool != state)
   {
      deleteParticle(); 
   }

   GLfloat posX = mouseX;
   GLfloat posZ = mouseZ;
   if( (mouseX < 0) || (mouseX > actualMap->x*SQUARESIZE))
   {
      posX = 0;
   }
   if( (mouseZ < 0) || (mouseZ > actualMap->z*SQUARESIZE))
   {
      posZ = 0;
   }
   
   if( (tool == TOOL_PARTICLE_FIRE) && (!actualParticle) )
   {
      state = TOOL_PARTICLE_FIRE; 
      part2* tmpPart = NULL;
      string fileToOpen = getStringFromUser("FileName Input","../data/particles/",
                                            proj, modl, viewPort);
      if(fileToOpen != "../data/particles/")
      {
         height = 0;
         tmpPart = new part2(mouseX, height, mouseZ, fileToOpen);
         if(!tmpPart)
         {
            printf("Error opening: %s\n", fileToOpen.c_str());
            actualParticle = NULL;
            return;
         }
         actualParticle = (particleSystem*) tmpPart;
      }
      else
      {
         actualParticle = NULL;
      }
   }
   
   if( (actualParticle) )
   {
      if(keys[SDLK_EQUALS])
      {
         height += 1;
      }
      if(keys[SDLK_MINUS])
      {
         height -= 1;
      }

      actualParticle->definePosition(mouseX, height, mouseZ);
   }
}

void particles::drawTemporary(GLfloat matriz[6][4])
{
   if(actualParticle != NULL)
   {
      if(state == TOOL_PARTICLE_FIRE)
      {
         glPushMatrix();
            part2* tmp = (part2*) actualParticle;
            tmp->NextStep(matriz);
         glPopMatrix();
      }
   }
}


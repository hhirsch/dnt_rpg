#include "particles.h"
#include "../../gui/messages.h"

#define STATE_PLANES     800
#define STATE_GRASS_INIT 801
#define STATE_LAKE_INIT  802

/*****************************************************************
 *                          Constructor                          *
 *****************************************************************/
particles::particles(Map* acMap)
{
   actualMap = acMap;
   actualParticle = NULL;
   state = -1;
   particleType = -1;
   previousText = "";
}

/*****************************************************************
 *                           Destructor                          *
 *****************************************************************/
particles::~particles()
{
   state = -1;
   actualMap = NULL;
   particleType = -1;
   string particleFileName = "";
   deleteParticle();
}

/*****************************************************************
 *                          deleteParticle                       *
 *****************************************************************/
void particles::deleteParticle()
{
   if(actualParticle)
   { 
      /* Waterfall */
      if(state == TOOL_PARTICLE_WATERFALL)
      {
         part1* tmp = (part1*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
      /* Fire */
      else if(state == TOOL_PARTICLE_FIRE)
      {
         part2* tmp = (part2*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
      /* Smoke */
      else if(state == TOOL_PARTICLE_SMOKE)
      {
         part4* tmp = (part4*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
      /* Snow */
      else if(state == TOOL_PARTICLE_SNOW)
      {
         part7* tmp = (part7*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
   }
}

/*****************************************************************
 *                          verifyAction                         *
 *****************************************************************/
void particles::verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                             Uint8 mButton, Uint8* keys, guiIO* gui, 
                             partController* pS, GLdouble proj[16],
                             GLdouble modl[16], GLint viewPort[4],
                             string selectedText,
                             grassWindow* grWindow, 
                             waterWindow* wtWindow,
                             Map* actualMap)
{
   int tool = gui->getTool();

   /* Set the system controller to the windows */
   if(grWindow)
   {
      grWindow->setPartSystem(pS);
   }
   if(wtWindow)
   {
      wtWindow->setPartSystem(pS);
   }

   if( ( (tool != state) || (selectedText != previousText) ) && 
       (state != STATE_PLANES) && (state != TOOL_PARTICLE_GRASS))
   {
      deleteParticle();
   }

   previousText = selectedText;

   /* Set Position */
   GLfloat posX = mouseX;
   GLfloat posZ = mouseZ;
   if( (mouseX < 0) || (mouseX > actualMap->getSizeX()*actualMap->squareSize()))
   {
      posX = 0;
   }
   if( (mouseZ < 0) || (mouseZ > actualMap->getSizeZ()*actualMap->squareSize()))
   {
      posZ = 0;
   }

   /* Grass TOOL */
   if( tool == TOOL_PARTICLE_GRASS )
   {
      if(state == TOOL_PARTICLE_GRASS)
      {
         if(mButton & SDL_BUTTON(1))
         {
            x1 = mouseX;
            z1 = mouseZ;
            state = STATE_GRASS_INIT;
         }
      }
      else if(state == STATE_GRASS_INIT)
      {
         if(!(mButton & SDL_BUTTON(1)))
         {
            GLfloat tmp;
            if(x2 < x1)
            {
               tmp = x2;
               x2 = x1;
               x1 = tmp;
            }
            if(z2 < z1)
            {
               tmp = z2;
               z2 = z1;
               z1 = tmp;
            }

            GLfloat povValue = 700.0;
            int total = (int)floor((z2-z1)*(x2-x1) / povValue);
            grass* gr = (grass*) pS->addParticle(PART_GRASS, x1, z1, x2, z2, 
                                                 total, 3.0, selectedText); 
            grWindow->setGrass(gr);
            state = TOOL_PARTICLE_GRASS;
         }
         x2 = mouseX;
         z2 = mouseZ;
      }
      else
      {
         state = TOOL_PARTICLE_GRASS;
         particleType = PART_GRASS;
      }
      
   }

   /* Lake TOOL */
   else if(tool == TOOL_PARTICLE_LAKE)
   {
      if(state == TOOL_PARTICLE_LAKE)
      {
         if(mButton & SDL_BUTTON(1))
         {
            x1 = mouseX;
            z1 = mouseZ;
            state = STATE_LAKE_INIT;
         }
      }
      else if(state == STATE_LAKE_INIT)
      {
         if(!(mButton & SDL_BUTTON(1)))
         {
            GLfloat tmp;
            if(x2 < x1)
            {
               tmp = x2;
               x2 = x1;
               x1 = tmp;
            }
            if(z2 < z1)
            {
               tmp = z2;
               z2 = z1;
               z1 = tmp;
            }

            /* Add the lake */
            actualMap->addLake(x1,z1,x2,z2);

            state = TOOL_PARTICLE_LAKE;
         }
         x2 = mouseX;
         z2 = mouseZ;
      }
      else
      {
         state = TOOL_PARTICLE_LAKE;
         particleType = -1;
      }
   }

   /* fire TOOL */
   else if( (tool == TOOL_PARTICLE_FIRE) && (!actualParticle) )
   {
      state = TOOL_PARTICLE_FIRE; 
      particleType = PART_FIRE;
      part2* tmpPart = NULL;
      string fileToOpen = selectedText;
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

   /* Smoke */
   else if( (tool == TOOL_PARTICLE_SMOKE) && (!actualParticle) )
   {
      state = TOOL_PARTICLE_SMOKE; 
      particleType = PART_SMOKE;
      part4* tmpPart = NULL;
      string fileToOpen = selectedText;
      if(fileToOpen != "../data/particles/")
      {
         height = 0;
         tmpPart = new part4(mouseX, height, mouseZ, fileToOpen);
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

   /* Waterfall */
   else if( (tool == TOOL_PARTICLE_WATERFALL) && (!actualParticle) )
   {
      particleType = PART_WATERFALL;
      state = TOOL_PARTICLE_WATERFALL; 
      part1* tmpPart = NULL;
      string fileToOpen = selectedText;
      if(fileToOpen != "../data/particles/")
      {
         height = 0;
         tmpPart = new part1(mouseX, height, mouseZ, fileToOpen);
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
   
   /* Snow */
   else if( (tool == TOOL_PARTICLE_SNOW) && (!actualParticle) )
   {
      state = TOOL_PARTICLE_SNOW; 
      particleType = PART_SNOW;
      part7* tmpPart = NULL;
      string fileToOpen = selectedText;
      if(fileToOpen != "../data/particles/")
      {
         height = 0;
         tmpPart = new part7(mouseX, height, mouseZ, fileToOpen);
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
   
   /* Default particle Actions (Up, down, etc) */
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

      if(mButton & SDL_BUTTON(1))
      {
         if( (tool != TOOL_PARTICLE_GRASS) && (tool != TOOL_PARTICLE_LAKE))
         {
            particleSystem* p = pS->addParticle(particleType, 
                                                mouseX, height, mouseZ,
                                                actualParticle->getFileName());

            /* Set the waterfall window */
            if(tool == TOOL_PARTICLE_WATERFALL)
            {
               wtWindow->setWater((part1*)p);
            }
            deleteParticle();
            gui->setTool(TOOL_NONE);
            actualParticle = NULL;

         }
         while(mButton & SDL_BUTTON(1))
         {
            //Wait for Mouse Button Release
            SDL_PumpEvents();
            int x,y;
            mButton = SDL_GetMouseState(&x,&y);
         }
      }
   }
}

/*****************************************************************
 *                          drawTemporary                        *
 *****************************************************************/
void particles::drawTemporary(GLfloat matriz[6][4])
{
   if(state == STATE_GRASS_INIT)
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.01,0.1,0.8);
      glBegin(GL_QUADS);
         glVertex3f(x1, 1, z1);
         glVertex3f(x2, 1, z1);
         glVertex3f(x2, 1, z2);
         glVertex3f(x1, 1, z2);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   else if(state == STATE_LAKE_INIT)
   {
      glDisable(GL_LIGHTING);
      glColor3f(0.5,0.4,0.9);
      glBegin(GL_QUADS);
         glVertex3f(x1, -4, z1);
         glVertex3f(x2, -4, z1);
         glVertex3f(x2, -4, z2);
         glVertex3f(x1, -4, z2);
      glEnd();
      glEnable(GL_LIGHTING);
   }
   else if(actualParticle != NULL)
   {
      if(state == TOOL_PARTICLE_FIRE)
      {
         glPushMatrix();
            part2* tmp = (part2*) actualParticle;
            tmp->nextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_SMOKE)
      {
         glPushMatrix();
            part4* tmp = (part4*) actualParticle;
            tmp->nextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_WATERFALL)
      {
         glPushMatrix();
            part1* tmp = (part1*) actualParticle;
            tmp->nextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_SNOW)
      {
         glPushMatrix();
            part6* tmp = (part6*) actualParticle;
            tmp->nextStep(matriz);
         glPopMatrix();
      }

   }
}


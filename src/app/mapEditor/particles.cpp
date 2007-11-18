#include "particles.h"
#include "../../gui/messages.h"

#define STATE_PLANES     800
#define STATE_GRASS_INIT 801

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
      if(state == TOOL_PARTICLE_FIRE)
      {
         part2* tmp = (part2*) actualParticle;
         delete(tmp);
         actualParticle = NULL;
      }
      else if(state == TOOL_PARTICLE_SMOKE)
      {
         part4* tmp = (part4*) actualParticle;
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
                             partSystem* pS, GLdouble proj[16],
                             GLdouble modl[16], GLint viewPort[4],
                             string selectedText,
                             grassWindow* grWindow, 
                             waterWindow* wtWindow)
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

            GLfloat povValue = 20.0;
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
         /*if( state == STATE_PLANES )
         {
            part1* tmp;
            tmp = (part1*) actualParticle;
            i = tmp->addPlane(actualPlane->x1, height-1, actualPlane->z1, 
                              actualPlane->x2, height-1, actualPlane->z2,
                              actualPlane->dX, actualPlane->dZ, 
                              PLANE_NO_INCLINATION);
            actualPlane = tmp->getPlane(i);
         }
         else if(particleType == PART_WATERFALL)
         {
            part1* tmp;
            tmp = (part1*) pS->addParticle(particleType, mouseX, height, mouseZ,
                                           actualParticle->getFileName());
            delete(actualParticle);
            actualParticle = (particleSystem*) tmp;
            state = STATE_PLANES;
            i = tmp->addPlane(mouseX-2, height-1, mouseZ-2, 
                              mouseX+2, height-1, mouseZ+2,
                              1, 0, PLANE_NO_INCLINATION);
            actualPlane = tmp->getPlane(i);
         }
         else*/ 
         if(tool != TOOL_PARTICLE_GRASS)
         {
            particleSystem* p = pS->addParticle(particleType, mouseX, height, mouseZ,
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

#if 0
      /* Verify Planes Keys */
      if(state == STATE_PLANES)
      {
         /* Change Modes of Inclination */
         if(keys[SDLK_1])
         {
            actualPlane->inclination = PLANE_NO_INCLINATION;
         }
         if(keys[SDLK_2])
         {
            actualPlane->inclination = PLANE_INCLINATION_X;
         }
         if(keys[SDLK_3])
         {
            actualPlane->inclination = PLANE_INCLINATION_Z;
         }
         /* Move Plane on X Axis */
         if(keys[SDLK_a])
         {
            actualPlane->x1 -= 0.1;
            actualPlane->x2 -= 0.1;
         }
         if(keys[SDLK_d])
         {
            actualPlane->x1 += 0.1;
            actualPlane->x2 += 0.1;
         }
         /* Move Plane on Z axis */
         if(keys[SDLK_w])
         {
            actualPlane->z1 -= 0.1;
            actualPlane->z2 -= 0.1;
         }
         if(keys[SDLK_s])
         {
            actualPlane->z1 += 0.1;
            actualPlane->z2 += 0.1;
         }
         /* Move Plane on Y Axis */
         if(keys[SDLK_f])
         {
            actualPlane->y1 -= 0.1;
            actualPlane->y2 -= 0.1;
         }
         if(keys[SDLK_r])
         {
            actualPlane->y1 += 0.1;
            actualPlane->y2 += 0.1;
         }
         if(keys[SDLK_q])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->y1 += 0.1;
            }
            else
            {
               actualPlane->y1 -= 0.1;
            }
         }
         if(keys[SDLK_e])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->y2 += 0.1;
            }
            else
            {
               actualPlane->y2 -= 0.1;
            }

         }
         /* Scale Plane on Z axis */
         if(keys[SDLK_z])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->z1 += 0.05;
               actualPlane->z2 -= 0.05;
            }
            else
            {
               actualPlane->z1 -= 0.05;
               actualPlane->z2 += 0.05;
            }
         }
         /* Scale Plane on X axis */
         if(keys[SDLK_x])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->x1 += 0.05;
               actualPlane->x2 -= 0.05;
            }
            else
            {
               actualPlane->x1 -= 0.05;
               actualPlane->x2 += 0.05;
            }
         }
         /* Scale Plane on Y axis */
         if(keys[SDLK_y])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->y1 += 0.05;
               actualPlane->y2 -= 0.05;
            }
            else
            {
               actualPlane->y1 -= 0.05;
               actualPlane->y2 += 0.05;
            }
         }
         /* dX Modification */
         if(keys[SDLK_n])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->dX = 0;
            }
            else if( (keys[SDLK_RCTRL]) || (keys[SDLK_LCTRL]))
            {
               actualPlane->dX = -1;
            }
            else
            {
               actualPlane->dX = 1;
            }
         }
         if(keys[SDLK_j])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->dX -= 0.1;
            }
            else
            {
               actualPlane->dX += 0.1;
            }
         }
         /* dZ Modification */
         if(keys[SDLK_m])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->dZ = 0;
            }
            else if( (keys[SDLK_RCTRL]) || (keys[SDLK_LCTRL]))
            {
               actualPlane->dZ = -1;
            }
            else
            {
               actualPlane->dZ = 1;
            }
         }
         if(keys[SDLK_k])
         {
            if( (keys[SDLK_RSHIFT]) || (keys[SDLK_LSHIFT]))
            {
               actualPlane->dZ -= 0.1;
            }
            else
            {
               actualPlane->dZ += 0.1;
            }
         }



         if(keys[SDLK_ESCAPE])
         {
            /* Exit Plane Add Mode */
            state = TOOL_PARTICLE_WATERFALL;
            part1* tmp;
            tmp = (part1*) actualParticle;
            tmp->removeLastPlane();
            actualParticle = NULL;
         }
      }
#endif 
   }
}

/*****************************************************************
 *                          drawTemporary                        *
 *****************************************************************/
void particles::drawTemporary(GLfloat matriz[6][4])
{
   if( state == STATE_PLANES )
   {
      glDisable(GL_LIGHTING);
      glColor3f(0,0,0);
      glBegin(GL_QUADS);
      if(actualPlane->inclination == PLANE_NO_INCLINATION)
      {
         glVertex3f(actualPlane->x1, actualPlane->y1, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y1, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y1, actualPlane->z2);
         glVertex3f(actualPlane->x1, actualPlane->y1, actualPlane->z2);
      }
      else if(actualPlane->inclination == PLANE_INCLINATION_X)
      {
         glVertex3f(actualPlane->x1, actualPlane->y1, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y2, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y2, actualPlane->z2);
         glVertex3f(actualPlane->x1, actualPlane->y1, actualPlane->z2);
      }
      else if(actualPlane->inclination == PLANE_INCLINATION_Z)
      {
         glVertex3f(actualPlane->x1, actualPlane->y1, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y1, actualPlane->z1);
         glVertex3f(actualPlane->x2, actualPlane->y2, actualPlane->z2);
         glVertex3f(actualPlane->x1, actualPlane->y2, actualPlane->z2);
      }

      glEnd();
      glEnable(GL_LIGHTING);
   }
   else if(state == STATE_GRASS_INIT)
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
   else if(actualParticle != NULL)
   {
      if(state == TOOL_PARTICLE_FIRE)
      {
         glPushMatrix();
            part2* tmp = (part2*) actualParticle;
            tmp->NextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_SMOKE)
      {
         glPushMatrix();
            part4* tmp = (part4*) actualParticle;
            tmp->NextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_WATERFALL)
      {
         glPushMatrix();
            part1* tmp = (part1*) actualParticle;
            tmp->NextStep(matriz);
         glPopMatrix();
      }
      else if(state == TOOL_PARTICLE_SNOW)
      {
         glPushMatrix();
            part6* tmp = (part6*) actualParticle;
            tmp->NextStep(matriz);
         glPopMatrix();
      }

   }
}


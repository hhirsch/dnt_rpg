#include "politic.h"
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

politic::politic():potentAgent(false)
{
   GLfloat x; GLfloat z;
   state = new stateMachine(STATE_LOOK_OBJECT);
   srand(SDL_GetTicks());
   x = drand48() *  384 + 74;
   z = drand48() *  384 + 74;
   defineDestiny(x,z);
   brief = NULL;
}

politic::~politic()
{
   delete(state);
}

void politic::actualizeMachineAndPosition()
{
   GLfloat dist;
   GLfloat x, z;
   dist = sqrt( (destinyX - actualX)*(destinyX - actualX) +
                (destinyZ - actualZ)*(destinyZ - actualZ));
   if( (state->getActualState() == STATE_LOOK_OBJECT) &&
       brief != NULL)
   {
      state->nextState(SDL_GetTicks());
      defineDestiny(brief->x, brief->z);
   }
   else if( (state->getActualState() == STATE_LOOK_OBJECT) && 
       ( (dist < 10) || (SDL_GetTicks()-state->getTime() > 10000)) ) 
   {
      state->setTime(SDL_GetTicks());
      srand(SDL_GetTicks());
      x = drand48() *  384 + 74;
      z = drand48() *  384 + 74;
      defineDestiny(x,z);
   }
   else if( (state->getActualState() == STATE_GET_OBJECT) &&
            (dist < 30))
   {
      state->nextState(SDL_GetTicks());
      defineDestiny((baseX1+baseX2)/2.0, (baseZ1+baseZ2)/2.0);
   }
   else if( (state->getActualState() == STATE_BRING_OBJECT) &&
            (actualX >= baseX1) && (actualX <=baseX2) && 
            (actualZ >= baseZ1) && (actualZ <= baseZ2))
   {
      brief = NULL;
      srand(SDL_GetTicks());
      state->nextState(SDL_GetTicks());
      x = drand48() *  384 + 74;
      z = drand48() *  384 + 74;
      defineDestiny(x,z);
   }
   actualize();
   if( (state->getActualState() == STATE_BRING_OBJECT) && (brief!=NULL))
   {
      brief->x = actualX;
      brief->z = actualZ;
   }
}

briefCase* politic::currentBriefCase()
{
   return(brief);
}

void politic::setBriefCase(briefCase* br)
{
   brief = br;
   if(brief)
   {
      brief->owned = true;
   }
}

void politic::setBaseArea(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2)
{
   baseX1 = x1;
   baseX2 = x2;
   baseZ1 = z1;
   baseZ2 = z2;
}

int politic::getState()
{
   return(state->getActualState());
}


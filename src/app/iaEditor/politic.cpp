#include "politic.h"
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

/**********************************************************************
 *                           Constructor                              *
 **********************************************************************/
politic::politic():potentAgent(false)
{
   GLfloat x; GLfloat z;
   state = new stateMachine(STATE_LOOK_OBJECT);
   srand(SDL_GetTicks());
   x = drand48() *  384 + 74;
   z = drand48() *  448;
   defineDestiny(x,z);
   brief = NULL;
   lastWork = 0;
   busted = false;
}

/**********************************************************************
 *                            Destructor                              *
 **********************************************************************/
politic::~politic()
{
   delete(state);
}

/**********************************************************************
 *                      ActualizeMachineAndPosition                   *
 **********************************************************************/
void politic::actualizeMachineAndPosition(bool workTime)
{
   GLfloat dist;
   GLfloat x, z;
   dist = sqrt( (destinyX - actualX)*(destinyX - actualX) +
                (destinyZ - actualZ)*(destinyZ - actualZ));

   /*WORK*/
   if((state->getActualState() != STATE_WORK) &&
      (state->getActualState() != STATE_GOING_WORK) &&
      (workTime) && (state->getActualState() != STATE_BUSTED) &&
      (state->getActualState() != STATE_INTERROGATION) )
   {
      state->setState(STATE_GOING_WORK, SDL_GetTicks());
      if(brief)
      {
         /* Leave BriefCase */
         brief->owned = false;
         brief = NULL;
      }
      defineDestiny(congressX, congressZ );
   }
   else if( (state->getActualState() == STATE_GOING_WORK) &&
            (actualX >= congressX-32) && (actualX <= congressX+32) &&
            (actualZ >= congressZ-32) && (actualZ <= congressZ+32))
   {
      lastWork = SDL_GetTicks();
      state->nextState(lastWork);
   }
   else if( (state->getActualState() == STATE_WORK) &&
            ((SDL_GetTicks() - lastWork) > WORK_TIME) )
   {
      lastWork = SDL_GetTicks();
      srand(SDL_GetTicks());
      state->nextState(SDL_GetTicks());
      x = drand48() *  384 + 74;
      z = drand48() *  448;
      defineDestiny(x,z);
   }
   /* LOOK_OBJECT */
   else if( (state->getActualState() == STATE_LOOK_OBJECT) &&
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
      z = drand48() *  448;
      defineDestiny(x,z);
   }
   
   /* GET_OBJECT */
   else if( (state->getActualState() == STATE_GET_OBJECT) &&
            (dist < 15))
   {
      state->nextState(SDL_GetTicks());
      defineDestiny((baseX1+baseX2)/2.0, (baseZ1+baseZ2)/2.0);
   }

   /* BRING_OBJECT */
   else if( (state->getActualState() == STATE_BRING_OBJECT) &&
            (actualX >= baseX1) && (actualX <=baseX2) && 
            (actualZ >= baseZ1) && (actualZ <= baseZ2))
   {
      brief->delivered = true;
      brief = NULL;
      srand(SDL_GetTicks());
      state->nextState(SDL_GetTicks());
      x = drand48() *  384 + 74;
      z = drand48() *  448;
      defineDestiny(x,z);
   }

   /* BUSTED */
   else if( (state->getActualState() == STATE_BUSTED) &&
            (actualX >= federalX-32) && (actualX <= federalX + 32) &&
            (actualZ >= federalZ-32) && (actualZ <= federalZ + 32))
   {
      state->nextState(SDL_GetTicks());
      brief->delivered = true;
      brief = NULL;
   }

   /* INTERROGATION */
   else if( (state->getActualState() == STATE_INTERROGATION) &&
            ((SDL_GetTicks() - state->getTime()) >= INTERROGATION_TIME))
   {
      state->nextState(SDL_GetTicks());
      lastWork = SDL_GetTicks();
      srand(SDL_GetTicks());
      x = drand48() *  384 + 74;
      z = drand48() *  448;
      defineDestiny(x,z);
   }

   /* PRETEND */
   else if( (state->getActualState() == STATE_PRETEND) )
   {
      if(SDL_GetTicks()-state->getTime() >= PRETEND_TIME)
      {
         /* End of Pretend Time */
         state->nextState(SDL_GetTicks());
         srand(SDL_GetTicks());
         x = drand48() *  384 + 74;
         z = drand48() *  448;
         defineDestiny(x,z);
      }
      else if((dist < 10) || (SDL_GetTicks()-lastWork >= 10000))
      {
         /* Change Pretend Destiny */
         lastWork = SDL_GetTicks();
         srand(SDL_GetTicks());
         x = drand48() *  384 + 74;
         z = drand48() *  448;
         defineDestiny(x,z);
      }
   }

   /* Only Actualize position when not working or interrogated */
   if( (state->getActualState() != STATE_WORK) &&
       (state->getActualState() != STATE_INTERROGATION) &&
       (state->getActualState() != STATE_BUSTED))
   {
      actualize();
   }
   
   if( (state->getActualState() == STATE_BRING_OBJECT) && (brief!=NULL))
   {
      brief->x = actualX;
      brief->z = actualZ;
   }
}

/**********************************************************************
 *                           currentBriefCase                         *
 **********************************************************************/
briefCase* politic::currentBriefCase()
{
   return(brief);
}

/**********************************************************************
 *                           setBriefCase                             *
 **********************************************************************/
void politic::setBriefCase(briefCase* br)
{
   brief = br;
   if(brief)
   {
      brief->owned = true;
   }
}

/**********************************************************************
 *                           setBaseArea                              *
 **********************************************************************/
void politic::setBaseArea(int group, GLfloat x1, GLfloat z1, 
                                     GLfloat x2, GLfloat z2)
{
   rogueGroup = group;
   baseX1 = x1;
   baseX2 = x2;
   baseZ1 = z1;
   baseZ2 = z2;
}

/**********************************************************************
 *                           setCongress                              *
 **********************************************************************/
void politic::setCongress(GLfloat x, GLfloat z)
{
   congressX = x;
   congressZ = z;
}

/**********************************************************************
 *                             getState                               *
 **********************************************************************/
int politic::getState()
{
   return(state->getActualState());
}

/**********************************************************************
 *                           setFederal                               *
 **********************************************************************/
void politic::setFederal(GLfloat x, GLfloat z)
{
   federalX = x;
   federalZ = z;
}

/**********************************************************************
 *                            setState                                *
 **********************************************************************/
void politic::setState(int st)
{
   state->setState(st, SDL_GetTicks());
}


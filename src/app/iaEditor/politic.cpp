#include "politic.h"
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

#define MAX_TIME_WITHOUT_STEAL 500000
#define MAX_AVARICE            500

/**********************************************************************
 *                           Constructor                              *
 **********************************************************************/
politic::politic(int* score):potentAgent(false)
{
   GLfloat x; GLfloat z;
   state = new stateMachine(STATE_LOOK_OBJECT);
   srand(SDL_GetTicks());
   x = (rand()/(double)RAND_MAX) *  384 + 74;
   z = (rand()/(double)RAND_MAX) *  448;
   defineDestiny(x,z);
   brief = NULL;
   lastWork = 0;
   busted = false;
   groupScore = score;
   lastSteal = SDL_GetTicks();
   srand(SDL_GetTicks());
   avarice = rand() % MAX_AVARICE;
   fuzzyInit();
}

/**********************************************************************
 *                            fuzzyInit                               *
 **********************************************************************/
void politic::fuzzyInit()
{
   fuzzyRule* tmpRule;
   fuzzyVariable* take;
   fuzzyVariable* maybeTake;
   fuzzyVariable* notTake;
   
   fuzzyFunction *muchAvarice,
                 *normalAvarice,
                 *fewAvarice;

   /* Add Variables */
   notTake = fuzzyLogic.addVariable(FUZZY_FUNCTION_INVERTED_STEP,
                                    0.2, 0.3, 0.0, 0.0);
   maybeTake = fuzzyLogic.addVariable(FUZZY_FUNCTION_TRAPEZOIDAL,
                                      0.2,0.3,0.7,0.8);
   take = fuzzyLogic.addVariable(FUZZY_FUNCTION_STEP,0.7,0.8,0.0,0.0);
   
   /* Add Functions */
   fewMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                     0.2,0.3,0.0,0.0);
   normalMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRAPEZOIDAL,
                                        0.2,0.3,0.7,0.8);
   muchMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.7,0.8,0.0,0.0);
   fewTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                    0.2,0.5,0.0,0.0);
   normalTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRIANGULAR,
                                       0.2,0.5,0.8,0.0);
   muchTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.5,0.8,0.0,0.0);
   fewAvarice = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                       0.3,0.5,0.0,0.0);
   normalAvarice = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRAPEZOIDAL,
                                        0.3,0.5,0.6,0.8);
   muchAvarice = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.6,0.8,0.0,0.0);

   /* Add Rules */

    /* Much Time -> take */
   tmpRule = fuzzyLogic.addRule(1);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchTime);

    /* Much Money -> take */
   tmpRule = fuzzyLogic.addRule(1);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchMoney);

    /* Much Avarice AND few Money -> maybe take */
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(maybeTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewMoney);

    /* Much Avarice AND  Not Few Money -> take*/
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, fewMoney);

    /* Normal Avarice AND Normal Time AND ! Few Money -> take*/
   tmpRule = fuzzyLogic.addRule(3);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalTime);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, fewMoney);

    /* Normal Avarice AND normal Time AND Few Money -> maybe take*/
   tmpRule = fuzzyLogic.addRule(3);
   tmpRule->addVariable(maybeTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalTime);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewMoney);

    /* Normal Avarice AND few time AND few MOney -> notTake */
   tmpRule = fuzzyLogic.addRule(3);
   tmpRule->addVariable(notTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewTime);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewMoney);

   /* Normal Avarice AND few time AND normal MOney -> maybeTake */
   tmpRule = fuzzyLogic.addRule(3);
   tmpRule->addVariable(maybeTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewTime);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalMoney);

    /* few Avarice AND ! much time -> notTake */
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(notTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewAvarice);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, muchTime);

   /* Set Fixed Values of Fixed Functions */
   muchAvarice->setCrispValue(avarice / (float)MAX_AVARICE);
   normalAvarice->setCrispValue(avarice / (float)MAX_AVARICE);
   fewAvarice->setCrispValue(avarice / (float)MAX_AVARICE);
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
      x = (rand()/(double)RAND_MAX) *  384 + 74;
      z = (rand()/(double)RAND_MAX) *  448;
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
      x = (rand()/(double)RAND_MAX) *  384 + 74;
      z = (rand()/(double)RAND_MAX) *  448;
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
      *groupScore += brief->value;
      brief = NULL;
      lastSteal = SDL_GetTicks();
      srand(SDL_GetTicks());
      state->nextState(SDL_GetTicks());
      x = (rand()/(double)RAND_MAX) *  384 + 74;
      z = (rand()/(double)RAND_MAX) *  448;
      defineDestiny(x,z);
   }

   /* BUSTED */
   else if( (state->getActualState() == STATE_BUSTED) &&
            (actualX >= federalX-32) && (actualX <= federalX + 32) &&
            (actualZ >= federalZ-32) && (actualZ <= federalZ + 32))
   {
      state->nextState(SDL_GetTicks());
      brief->delivered = true;
      *groupScore -= BUSTED_VALUE;
      brief = NULL;
   }

   /* INTERROGATION */
   else if( (state->getActualState() == STATE_INTERROGATION) &&
            ((SDL_GetTicks() - state->getTime()) >= INTERROGATION_TIME))
   {
      state->nextState(SDL_GetTicks());
      lastWork = SDL_GetTicks();
      srand(SDL_GetTicks());
      x = (rand()/(double)RAND_MAX) *  384 + 74;
      z = (rand()/(double)RAND_MAX) *  448;
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
         x = (rand()/(double)RAND_MAX) *  384 + 74;
         z = (rand()/(double)RAND_MAX) *  448;
         defineDestiny(x,z);
      }
      else if((dist < 10) || (SDL_GetTicks()-lastWork >= 10000))
      {
         /* Change Pretend Destiny */
         lastWork = SDL_GetTicks();
         srand(SDL_GetTicks());
         x = (rand()/(double)RAND_MAX) *  384 + 74;
         z = (rand()/(double)RAND_MAX) *  448;
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

float politic::getTimePercent()
{
   float result = (float) ((float)(SDL_GetTicks() - lastSteal) 
                          / (float)MAX_VALUE);

   if(result > 1)
   {
      result = 1.0;
   }
   return(result);
}

float politic::getAvaricePercent()
{
   return(avarice / (float)MAX_AVARICE);
}


/**********************************************************************
 *                           setBriefCase                             *
 **********************************************************************/
bool politic::setBriefCase(briefCase* br)
{
   if(br)
   {
      /* Verify if will take in Fuzzy Logic */
      float percValue = (float) ((float)br->value / 
                                 (float)(MAX_VALUE+SUM_VALUE));
      muchMoney->setCrispValue(percValue); 
      normalMoney->setCrispValue(percValue);
      fewMoney->setCrispValue(percValue);
      float percTime = getTimePercent();
      if(percTime > 1)
      {
         percTime = 1.0;
      }
      muchTime->setCrispValue(percTime);
      normalTime->setCrispValue(percTime); 
      fewTime->setCrispValue(percTime);
      
      fuzzyLogic.evalute();

      float result = fuzzyLogic.defuzzyfication();

      if(result >= 0.7)
      {
         brief = br;
         brief->owned = true;
         return(true);
      }
      else
      {
         return(false);
      }
   }
   return(false);
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



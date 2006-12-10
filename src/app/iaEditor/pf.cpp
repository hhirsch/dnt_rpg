#include "pf.h"
#include <math.h>

#define MAX_CORRUPT 200

/*****************************************************************
 *                        Constructor                            *
 *****************************************************************/
pf::pf()
{
   patAg = new pattAgent(true);
   potAg = new potentAgent(false);
   state = new stateMachine(STATE_PATROL);
   target = NULL;
   potAg->defineConstants(0.5, 100000, 0.5);
   lastPrison = SDL_GetTicks();
   srand(SDL_GetTicks());
   corrupt = rand() % MAX_CORRUPT;
   fuzzyInit();
}

/*****************************************************************
 *                         Destructor                            *
 *****************************************************************/
pf::~pf()
{
   delete(patAg);
   delete(potAg);
   delete(state);
}

/*****************************************************************
 *                          fuzzyInit                            *
 *****************************************************************/
void pf::fuzzyInit()
{
   fuzzyRule* tmpRule;
   fuzzyVariable* take;
   fuzzyVariable* maybeTake;
   fuzzyVariable* notTake;
   
   fuzzyFunction *muchCorrupt,
                 *normalCorrupt,
                 *fewCorrupt;

   /* Add Variables */
   notTake = fuzzyLogic.addVariable(FUZZY_FUNCTION_INVERTED_STEP,
                                    0.2, 0.4, 0.0, 0.0);
   maybeTake = fuzzyLogic.addVariable(FUZZY_FUNCTION_TRAPEZOIDAL,
                                      0.2,0.4,0.6,0.8);
   take = fuzzyLogic.addVariable(FUZZY_FUNCTION_STEP,0.6,0.8,0.0,0.0);
   
   /* Add Functions */
   fewMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                     0.2,0.3,0.0,0.0);
   normalMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRAPEZOIDAL,
                                        0.2,0.3,0.7,0.8);
   muchMoney = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.7,0.8,0.0,0.0);
   fewTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                    0.1,0.5,0.0,0.0);
   normalTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRIANGULAR,
                                       0.1,0.5,0.9,0.0);
   muchTime = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.5,0.9,0.0,0.0);
   fewCorrupt = fuzzyLogic.addFunction(FUZZY_FUNCTION_INVERTED_STEP,
                                       0.3,0.5,0.0,0.0);
   normalCorrupt = fuzzyLogic.addFunction(FUZZY_FUNCTION_TRAPEZOIDAL,
                                        0.3,0.5,0.6,0.8);
   muchCorrupt = fuzzyLogic.addFunction(FUZZY_FUNCTION_STEP,0.6,0.8,0.0,0.0);

   /* Add Rules */

    /* Much Time -> take */
   tmpRule = fuzzyLogic.addRule(1);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchTime);

    /* Much Corrupt AND ! much time -> not take */
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(notTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchCorrupt);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, muchTime);

    /* Corrupt AND Not few time -> maybe take*/
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(maybeTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalCorrupt);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, fewTime);

    /* Normal Corrupt AND few Time -> not take*/
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(notTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, normalCorrupt);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewTime);

    /* Few Corrupt AND much Money -> maybe take*/
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(maybeTake);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewCorrupt);
   tmpRule->addMember(FUZZY_OPERATOR_AND, muchMoney);

    /* Few Corrupt AND NOT much Money -> take */
   tmpRule = fuzzyLogic.addRule(2);
   tmpRule->addVariable(take);
   tmpRule->addMember(FUZZY_OPERATOR_AND, fewCorrupt);
   tmpRule->addMember(FUZZY_OPERATOR_AND_NOT, muchMoney);

   /* Set Fixed Values of Fixed Functions */
   muchCorrupt->setCrispValue(corrupt / (float)MAX_CORRUPT);
   normalCorrupt->setCrispValue(corrupt / (float)MAX_CORRUPT);
   fewCorrupt->setCrispValue(corrupt / (float)MAX_CORRUPT);
}

/*****************************************************************
 *                        getPostion                             *
 *****************************************************************/
void pf::getPosition(GLfloat& x, GLfloat& z)
{
   if(state->getActualState() == STATE_PATROL)
   {
      patAg->getPosition(x, z);
   }
   else
   {
      potAg->getPosition(x, z);
   }
}

/*****************************************************************
 *                         defineStepSize                        *
 *****************************************************************/
void pf::defineStepSize(GLfloat size)
{
   potAg->defineStepSize(size);
   patAg->defineStepSize(size);
}

/*****************************************************************
 *                         defineSight                           *
 *****************************************************************/
void pf::defineSight(GLfloat sightDist, GLfloat sightAng)
{
   potAg->defineSight(sightDist, sightAng);
   patAg->defineSight(sightDist, sightAng);
}

/*****************************************************************
 *                         getSight                              *
 *****************************************************************/
void pf::getSight(GLfloat& sightDist, GLfloat& sightAng)
{
   if(state->getActualState() == STATE_PATROL)
   {
      patAg->getSight(sightDist, sightAng);
   }
   else
   {
      potAg->getSight(sightDist, sightAng);
   }
}

/*****************************************************************
 *                         defineDestiny                         *
 *****************************************************************/
void pf::defineDestiny(GLfloat x, GLfloat z)
{
   patAg->defineDestiny(x,z);
   potAg->defineDestiny(x,z);
}

/*****************************************************************
 *                        definePosition                         *
 *****************************************************************/
void pf::definePosition(GLfloat x, GLfloat z)
{
   potAg->definePosition(x,z);
   patAg->definePosition(x,z);
}

/*****************************************************************
 *                      actualizeMachineAndPosition              *
 *****************************************************************/
void pf::actualizeMachineAndPosition()
{
   /* PATROL */
   if(state->getActualState() == STATE_PATROL)
   {
      patAg->actualize();
      if(target != NULL)
      {
         GLfloat actualX, actualZ;
         state->nextState(SDL_GetTicks());
         patAg->getPosition(actualX, actualZ);
         potAg->definePosition(actualX, actualZ);

      }
   }

   /* CATCH */
   else if(state->getActualState() == STATE_CATCH)
   {
      GLfloat x,z;
      GLfloat actualX, actualZ;
      target->getPosition(x,z);
      potAg->defineDestiny(x,z);
      potAg->actualize();
      potAg->getPosition(actualX, actualZ);
      GLfloat dist = sqrt( (x - actualX)*(x - actualX) +
                           (z - actualZ)*(z - actualZ));

      if(target->currentBriefCase() == NULL)
      {
         target = NULL;
         state->setState(STATE_GO_PATROL, SDL_GetTicks());
      }
      else if(dist < 10)
      {
         //Got the Politic!
         state->nextState(SDL_GetTicks());
         target->setState(STATE_BUSTED);
      }
   }

   /* SEND_PRISON */
   else if(state->getActualState() == STATE_SEND_PRISON)
   {
      GLfloat x,z;
      potAg->defineDestiny(federalX, federalZ);
      potAg->actualize();
      potAg->getPosition(x,z);
      target->definePosition(x,z);
      target->currentBriefCase()->x = x;
      target->currentBriefCase()->z = z;
      if( (x >= federalX-32) && ( x <= federalX+32) &&
          (z >= federalZ-32) && ( z <= federalZ+32))
      {
         lastPrison = SDL_GetTicks();
         state->nextState(SDL_GetTicks());
         target->busted = false;
         target = NULL;
      }
   }

   /* GO_PATROL */
   else if(state->getActualState() == STATE_GO_PATROL)
   {
      wayPoint* wp;
      GLfloat x,z;
      wp = patAg->getActualWayPoint();
      potAg->defineDestiny(wp->x, wp->z);
      potAg->actualize();
      potAg->getPosition(x,z);
      if( (x >= wp->x-10) && (x <= wp->x+10) && 
          (z >= wp->z-10) && (z <= wp->z+10))
      {
         state->nextState(SDL_GetTicks());
         patAg->definePosition(wp->x, wp->z);
      }
   }
}

/*****************************************************************
 *                         getTimePercent                        *
 *****************************************************************/
float pf::getTimePercent()
{
   float result = (float) ((float)(SDL_GetTicks() - lastPrison) 
                          / (float)MAX_VALUE);

   if(result > 1)
   {
      result = 1.0;
   }
   return(result);
}

float pf::getCorruptPercent()
{
   return(corrupt / (float)MAX_CORRUPT);
}


/*****************************************************************
 *                            setTarget                          *
 *****************************************************************/
bool pf::setTarget(politic* tg)
{
   
   if(tg)
   {
      /* Fuzzy Logic to verify if will take the target */
            /* Verify if will take in Fuzzy Logic */
      float percValue = (float) ((float)tg->currentBriefCase()->value / 
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
      
      if(result >= 0.65)
      {
         /* Take the Target */
         target = tg;
         target->busted = true;
         return(true);
      }
      else
      {
        return(false); 
      }
   }
   return(false);
}

/*****************************************************************
 *                            getTarget                          *
 *****************************************************************/
politic* pf::getTarget()
{
   return(target);
}

/*****************************************************************
 *                           setFederal                          *
 *****************************************************************/
void pf::setFederal(GLfloat x, GLfloat z)
{
   federalX = x;
   federalZ = z;
}

/*****************************************************************
 *                           getState                            *
 *****************************************************************/
int pf::getState()
{
   return(state->getActualState());
}


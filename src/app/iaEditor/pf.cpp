#include "pf.h"
#include <math.h>

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
 *                            setTarget                          *
 *****************************************************************/
void pf::setTarget(politic* tg)
{
   target = tg;
   if(target)
   {
      target->busted = true;
   }
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


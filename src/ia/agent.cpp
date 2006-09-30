#include "agent.h"


/********************************************************************
 *                         Constructor                              *
 ********************************************************************/
agent::agent(bool orientation)
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
   withOrientation = orientation;
}

/********************************************************************
 *                          Destructor                              *
 ********************************************************************/
agent::~agent()
{
   stepSize = 0;
   destinyX = -1;
   destinyZ = -1;
   actualX = -1;
   actualZ = -1;
}

/********************************************************************
 *                     Define Step Size                             *
 ********************************************************************/
void agent::defineStepSize(GLfloat size)
{
   stepSize = size;
}

/********************************************************************
 *                          Define Destiny                          *
 ********************************************************************/
void agent::defineDestiny(GLfloat x, GLfloat z)
{
   destinyX = x;
   destinyZ = z;
}

/********************************************************************
 *                          Define Position                         *
 ********************************************************************/
void agent::definePosition(GLfloat x, GLfloat z)
{
   actualX = x;
   actualZ = z;
}

/********************************************************************
 *                           Get Position                           *
 ********************************************************************/
void agent::getPosition(GLfloat& x, GLfloat& z)
{
   x = actualX;
   z = actualZ;
}

/********************************************************************
 *                           define Sight                           *
 ********************************************************************/
void agent::defineSight(GLfloat sightDist, GLfloat sightAng)
{
   sightDistance = sightDist;
   sightAngle = sightAngle;
}


/********************************************************************
 *                            Actualize                             *
 ********************************************************************/
void agent::actualize()
{
   //TODO get Obstacles in Range
   knowObstacles = 0;

   defineNextPosition();
}




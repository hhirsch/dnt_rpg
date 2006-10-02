#ifndef _pattAgent_h
#define _pattAgent_h

#include "agent.h"

#define AGENT_PATT_HALF_X 5.0
#define AGENT_PATT_HALF_Z 5.0
#define AGENT_PATT_HIGHT  6.0

class wayPoint
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */

      wayPoint* next;      /**< Next Way Point on list */
      wayPoint* previous;  /**< Previous Way point on list */
};

class pattAgent: public agent
{
   public:
      pattAgent(bool oriented);
      ~pattAgent();

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      bool defineNextPosition();

      /*! Add WayPoint to be the "last" on the Agent */
      void addWayPoint(GLfloat x, GLfloat z);

      pattAgent* next;              /**< Next Agent on List */
      
   protected:
      wayPoint* wayPoints;          /**< List of Way Points */
      wayPoint* actualWayPoint;     /**< Current Way Point Objective */

      int totalWayPoints;           /**< Total WayPoints on List */

      int loops;
      int actualLoop;
      GLfloat xInc;
      GLfloat zInc;

      /*! Define that the agent will go to next Way Point */
      void changeToNextWayPoint();    
};
                   


#endif


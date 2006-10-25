#ifndef _pattAgent_h
#define _pattAgent_h

#include "agent.h"

/*! Way Point Definition */
class wayPoint
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */

      wayPoint* next;      /**< Next Way Point on list */
      wayPoint* previous;  /**< Previous Way point on list */
};

/*! Pattern Agent */
class pattAgent: public agent
{
   public:
      /*! Constructor */
      pattAgent(bool oriented);
      /*! Destructor */
      ~pattAgent();

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      bool defineNextPosition();

      /*! Add WayPoint to be the "last" on the Agent */
      void addWayPoint(GLfloat x, GLfloat z);

      /*! Draw Waypoints, conneting them */
      void drawWayPoints();

      /*! Return the number of Way Points  */
      int getTotalWayPoints(){return(totalWayPoints);};

      /*! Get WayPoints List. Only for save on editor. */
      wayPoint* getWayPoints(){return(wayPoints);};

      wayPoint* getActualWayPoint(){return(actualWayPoint);};

      pattAgent* next;              /**< Next Agent on List */
      
   protected:
      wayPoint* wayPoints;          /**< List of Way Points */
      wayPoint* actualWayPoint;     /**< Current Way Point Objective */

      int totalWayPoints;           /**< Total WayPoints on List */

      GLfloat xInc;                 /**< How many units X coordinate uptade */
      GLfloat zInc;                 /**< How many units Z coordinate update */

      /*! Define that the agent will go to next Way Point */
      void changeToNextWayPoint();    
};
                   


#endif


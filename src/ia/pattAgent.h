#ifndef _pattAgent_h
#define _pattAgent_h

#include "agent.h"

/*! Way Point Definition */
class wayPoint
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */

      GLfloat angle;       /**< Angle for the wayPoint to previous one */

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

      /*! Add WayPoint to be the "first" on the Agent */
      void addWayPointFirst(GLfloat x, GLfloat z);

      /*! Remove waypoint from the path */
      void removeWayPoint(wayPoint* way);
      /*! Remove all linear way points (they are redundant) */
      void removeLinearWayPoints();


      /*! Draw Waypoints, conneting them */
      void drawWayPoints();
      void drawWayPointsLinear();


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

      /*! Calculate the angle value for the waypoint
       * \param way-> the waypoint to calculate angle
       * \param previous -> previous wayPoint */
      void calculateAngle(wayPoint* way, wayPoint* previous);

};
                   


#endif


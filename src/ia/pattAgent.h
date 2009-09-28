/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_pattAgent_h
#define _dnt_pattAgent_h

#include "../etc/list.h"

#include "agent.h"

/*! Way Point Definition */
class wayPoint: public dntListElement
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */

      GLfloat angle;       /**< Angle for the wayPoint to previous one */
};

/*! List of way points */
class wayPointList: public dntList
{
   public:
      /*! Constructor */
      wayPointList();
      /*! Destructor */
      ~wayPointList();
   protected:
      /*! Free wayPoint memory
       * \param opj -> wayPoint to delete */
       void freeElement(dntListElement* obj);
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

      /*! Set the pattern origin
       * \param x -> origin X position
       * \param z -> origin Z position */
      void setOrigin(GLfloat x, GLfloat z);

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

      /*! Get total wayPoints defined to the pattAgent
       * \return -> number of wayPoints */
      int getTotalWayPoints();

      /*! Return the number of units current walked by the agent
       * \return current (meters) walked by the agent */
      float getTotalWalked();

      /*! Get current wayPoint of the pattern agent
       * \return pointer to the wayPoint where the pattAgent is going to. */
      wayPoint* getActualWayPoint();

   protected:
      wayPointList list;            /**< List of wayPoints */
      wayPoint* actualWayPoint;     /**< Current Way Point Objective */

      int totalWayPoints;           /**< Total WayPoints on List */

      GLfloat totalWalked;          /**< Total units current walked */

      GLfloat origX,                /**< Original X position */
              origZ;                /**< Original Z position */
      
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


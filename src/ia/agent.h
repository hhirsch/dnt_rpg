/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_agent_h
#define _dnt_agent_h

#include <SDL/SDL_opengl.h>
#include "../engine/util.h"

#define MAX_OBSTACLES 30
#define MAX_ROTATION  TURN_VALUE



/*! Obstacle Percepted */
class obstacle
{
   public:
      GLfloat x;       /**< x center coordinate */
      GLfloat z;       /**< z center coordinate */

      GLfloat x1,      /**< x1 bounding box coordinate */
              z1,      /**< y1 bounding box coordinate */
              x2,      /**< x2 bounding box coordinate */
              z2;      /**< y2 bounding box coordinate */
};


/*! This Class Defines the Generic AI Agent */
class agent
{
   public:
      /*! Constructor */
      agent(bool oriented);
      /*! Destructor */
      virtual ~agent();

      /*! Actualize Agent */
      void actualize();

      /*! Define Destiny 
       * \param x -> x destiny coordinate
       * \param z -> z destiny coordinate */
      void defineDestiny(GLfloat x, GLfloat z);

      /*! Define Step Size 
       * \param size -> step size */
      void defineStepSize(GLfloat size);

      /*! Define Actual Agent Position 
       * \param x -> x position coordinate
       * \param z -> z position coordinate */
      void definePosition(GLfloat x, GLfloat z);

      /*! Get Actual Agent Position 
       * \param x -> will have X agent coordinate
       * \param z -> will have Z agent coordinate */
      void getPosition(GLfloat& x, GLfloat& z);

      /*! Define Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void defineSight(GLfloat sightDist, GLfloat sightAng);

      /*! Gets Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void getSight(GLfloat& sightDist, GLfloat& sightAng);

      void defineOrientation(GLfloat ori){orientation = ori;};

      void addObstacle(GLfloat x, GLfloat z, GLfloat x1, GLfloat z1,
                       GLfloat x2, GLfloat z2);

      void clearObstacles();

      bool oriented(){return(withOrientation);};
      GLfloat orientationValue(){return(orientation);};
      GLfloat getStepSize(){return(stepSize);};

      bool doAngle();

      bool addIfVisible(agent* testAg);

      void defineBoundingBox(GLfloat xa, GLfloat za, GLfloat xb, GLfloat zb);
      void getBoundingBox(GLfloat &xa, GLfloat &za, GLfloat &xb, GLfloat &zb);


      /* Virtual Methods */

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      virtual bool defineNextPosition()=0;

   protected:

         /* DESTINY */
      GLfloat destinyX;      /**< Destiny X  Coordinate */
      GLfloat destinyZ;      /**< Destiny Z Coordinate */

         /* POSITION */
      GLfloat actualX;       /**< Agent Actual X Coordinate */
      GLfloat actualZ;       /**< Agent Actual Z Coordinate */
        
         /* BOUNDING BOX */
      GLfloat x1;            /**< X1 Bounding Box */
      GLfloat z1;            /**< Z1 Bounding Box */
      GLfloat x2;            /**< X2 Bounding Box */
      GLfloat z2;            /**< Z2 Bounding Box */

         /* STEP */
      GLfloat stepSize;      /**< Agent Step Size per Actualization */

         /* ORIENTATION */
      bool withOrientation;  /**< If agent has orientation or not */
      GLfloat orientation;   /**< Agent orientation angle, if has */
      GLfloat desiredAngle;  /**< The desired angle to the agent.
                                  Only start walk after orientation 
                                  is in this angle. */

         /* SIGHT */
      GLfloat sightDistance; /**< The distance that agent can percept */
      GLfloat sightAngle;    /**< The angle that agent can percept, 
                                  if orientation is enabled */

         /* PERCEPTION */
      obstacle obstacles[MAX_OBSTACLES];   /**< Know obstacles */
      int knowObstacles;                   /**< Number of know obstacles */
};

#endif


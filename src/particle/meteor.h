#ifndef _meteor_h
#define _meteor_h

#include "part2.h"

#define METEOR_STATIC_LIVING  50   /**< Static living time of the meteor */

/*! The meteor class. It is just a fire particles, 
 * that have their orign translated, and when reach
 * target, stop the motion, for MAX_STATIC_LIVING iterations */
class meteor
{
   public:
      /*!
       ****************************************************************
       * Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position
       * \param vX -> variation on X
       * \param vY -> variation on Y
       * \param vZ -> variation on Z
       * \param tX -> target X
       * \param tY -> target Y
       * \param tZ -> target Z
       * \param fileName -> file name of the file to load 
       ****************************************************************/
      meteor(float cX, float cY, float cZ, float vX, float vY, float vZ,
             float tX, float tY, float tZ, string fileName);

      /*! Destructor */
      ~meteor();

      /*!
       ***************************************************************
       * Do things before render (like glBegin)
       ***************************************************************/  
      void InitRender();
      /*!
       ***************************************************************
       * Do things after render (like glEnd)
       ***************************************************************/ 
      void EndRender();

      /*!
       ****************************************************************
       * Verify if the Meteor is Living (aka, moving or 
       * in the static period) 
       * \return -> true if is living, false if it can be deleted 
       ****************************************************************/
      bool isLiving();

      /*!
       ***************************************************************
       * Do Next Step on System, rendering and actualizing
       * \param matriz -> View Frustum Matrix
       ***************************************************************/
      void NextStep(GLfloat matriz[6][4]);

      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();
   private:
      int actualLiving;     /**< Living Time after dead */
      GLfloat curPosX,      /**< Current Meteor X Position */
              curPosY,      /**< Current Meteor Y Position */
              curPosZ;      /**< Current Meteor Z Position */
      GLfloat varX,         /**< X variation */ 
              varY,         /**< Y variation */
              varZ;         /**< Z variation */
      GLfloat targX,        /**< Target X Coordinate */
              targY,        /**< Target Y Coordinate */
              targZ;        /**< Target Z Coordinate */
      bool dead;            /**< When reach target, is dead */
      part2* intFire;       /**< Internal Fire Particle */
};

#endif


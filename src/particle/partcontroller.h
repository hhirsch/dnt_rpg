/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _partController_h
#define _partController_h

#include "particle.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"
#include "part5.h"
#include "part6.h"
#include "part7.h"
#include "grass.h"
#include "meteor.h"
#include "partlist.h"
#include <string>
using namespace std;

#define PART_WATERFALL     1  /**< Waterfall ID */
#define PART_FIRE          2  /**< Fire ID */
#define PART_WATER_SURFACE 3  /**< Water Surface ID */
#define PART_SMOKE         4  /**< Smoke ID */
#define PART_BLOOD         5  /**< Blood ID */
#define PART_LIGHTNING     6  /**< Lightnig ID */
#define PART_SNOW          7  /**< Snow ID */
#define PART_GRASS         8  /**< Grass ID */
#define PART_METEOR        9  /**< Meteor ID */

/*! The partController class controls all particles systens, 
    make their updates, renders, deaths, etc. */
class partController
{
   public:
      /*! Init the use of the partController */
      void init();
      /*! Finish the use of the partController  */
      void finish();

      /*! Update and Render all Active Systems
       * \param PCposX -> position X of the PC.
       * \param PCposY -> position Y of the PC
       * \param PCposZ -> position Z of the PC.
       * \param matriz -> View Frustum Matrix.
       * \param enableGrass -> true if grass is enable. */
      void updateAll(float PCposX, float PCposY, float PCposZ, 
                     GLfloat** matriz, bool enableGrass);

      /*! Add a Particle System to the System
       * \param type -> the type of the System (PART_BLOOD, PART_FIRE)
       * \param X -> X coordinate
       * \param Y -> Y coordinate
       * \param Z -> Z coordinate
       * \param fileName -> name of the file containing the definition
       * \return particle pointer; */
      particleSystem* addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                                  string fileName );
      particleSystem* addParticle(int type, GLfloat x1, GLfloat z1,
                                  GLfloat x2, GLfloat z2, int total,
                                  GLfloat scale, string fileName);
      meteor* addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                          GLfloat varX, GLfloat varY, GLfloat varZ,
                          GLfloat targX, GLfloat targY, GLfloat targZ,
                          string fileName);

      /*! Remove an particle system from the system
       * \param type -> the type of the System (PART_BLOOD, PART_FIRE)
       * \param part -> address of the Particle */
      void removeParticle(int type, void* part);

      /*! Gets the total numbers of particles
       * \return Total Number of particles on system, from all systems */
      int numParticles();

      /*! Put all particle system on a stable state. */
      void stabilizeAll();

      /*! Load Particles Systems from file.
       * \param fileName -> name of the file to load. */
      void loadFromFile(string fileName);

      /*! Save Particles System To file.
       * \param fileName -> name of the file to be saved. */
      void saveToFile(string fileName);

      /*! Remove all particle systens from the system */
      void deleteAll();

      /*! Define the opened map
       * \param acMap -> pointer to the opened map
       * \param col -> pointer to the collision system */
      void setActualMap(void* acMap, collision* col);


   private:
      static particleList* waterfall;        /**< Waterfall Particles */
      static particleList* fire;             /**< Fire Particles */
      static particleList* waterSurface;     /**< WaterSurfaces Particles */
      static particleList* smoke;            /**< Smoke Particles */
      static particleList* blood;            /**< Blood Particles */
      static particleList* lightning;        /**< Lightning Particles */
      static particleList* snow;             /**< Snow Particles */
      static particleList* grassParticles;   /**< Grass Particles */
      static particleList* meteorParticles;  /**< Meteor Particles */
      static collision* colDetect;           /**< Collision System */

};


#endif

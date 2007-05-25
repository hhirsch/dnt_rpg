/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _partSystem_h
#define _partSystem_h

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


#define MAX_WATERFALL     2  /**< Max Number of Waterfalls per Map */
#define MAX_FIRE          5  /**< Max number of Fire per map */
#define MAX_SMOKE         5  /**< Max Number of Smoke per map */
#define MAX_WATER_SURFACE 3  /**< Max number of waters surfaces per map */
#define MAX_BLOOD         10 /**< Max number of blood per map */
#define MAX_SNOW          1  /**< Max number of snow per map */
#define MAX_LIGHTNING     3  /**< Max number of simultaniously lightinings */
#define MAX_GRASS         20 /**< Max Number of Grass */
#define MAX_METEOR        4  /**< Max Number of Meteors */

/*! The patSytem class controls all particles systens, 
    make their atualizations, renderizations, etc. */
class partSystem
{
   public:
      /*! Constructor */
      partSystem();
      /*! Destructor */
      ~partSystem();

      /*!
       *************************************************************** 
       * Actualize and Render all Active Systems
       * \param PCposX -> position X of the PC.
       * \param PCposY -> position Y of the PC
       * \param PCposZ -> position Z of the PC.
       * \param matriz -> View Frustum Matrix.
       * \param enableGrass -> true if grass is enable.
       ***************************************************************/
      void actualizeAll(float PCposX, float PCposY, float PCposZ, 
                        GLfloat matriz[6][4], bool enableGrass);

      /*!
       *************************************************************** 
       * Add a Particle System to the System
       * \param type -> the type of the System (PART_BLOOD, PART_FIRE)
       * \param X -> X coordinate
       * \param Y -> Y coordinate
       * \param Z -> Z coordinate
       * \param fileName -> name of the file containing the definition
       * \return particle pointer;
       ***************************************************************/
      particleSystem* addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                                  string fileName );
      particleSystem* addParticle(int type, GLfloat x1, GLfloat z1,
                                  GLfloat x2, GLfloat z2, int total,
                                  string fileName);
      meteor* addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                          GLfloat varX, GLfloat varY, GLfloat varZ,
                          GLfloat targX, GLfloat targY, GLfloat targZ,
                          string fileName);

      /*!
       *************************************************************** 
       * Remove an particle system from the system
       * \param type -> the type of the System (PART_BLOOD, PART_FIRE)
       * \param part -> address of the Particle
       ***************************************************************/
      void removeParticle(int type, void* part);

      /*!
       *************************************************************** 
       * Gets the total numbers of particles
       * \return Total Number of particles on system, from all systems
       ***************************************************************/
      int numParticles();

      /*!
       *************************************************************** 
       * Puts all particle system on a stable state.
       ***************************************************************/
      void stabilizeAll();

      /*!
       *************************************************************** 
       * Load Particles Systens from file.
       * \param fileName -> name of the file to load.
       ***************************************************************/
      void loadFromFile(string fileName);

      /*!
       *************************************************************** 
       * Save Particles System To file.
       * \param fileName -> name of the file to be saved.
       ***************************************************************/
      void saveToFile(string fileName);

      /*!
       *************************************************************** 
       * Remove all particle systens from the system
       ***************************************************************/
      void deleteAll();

      /*!
       ***************************************************************
       * Define the opened map
       * \param map -> pointer to the opened map
       * \param col -> pointer to the collision system
       ***************************************************************/
      void setActualMap(void* map, collision* col);


   private:
      part1* waterfall[MAX_WATERFALL];        /**< Waterfall Particles */
      part2* fire[MAX_FIRE];                  /**< Fire Particles */
      part3* waterSurface[MAX_WATER_SURFACE]; /**< WaterSurfaces Particles */
      part4* smoke[MAX_SMOKE];                /**< Smoke Particles */
      part5* blood[MAX_BLOOD];                /**< Blood Particles */
      part6* lightning[MAX_LIGHTNING];        /**< Lightning Particles */
      part7* snow[MAX_SNOW];                  /**< Snow Particles */
      grass* grassParticles[MAX_GRASS];       /**< Grass Particles */
      meteor* meteorParticles[MAX_METEOR];    /**< Meteor Particles */
      collision* colDetect;                   /**< Collision System */

};


#endif

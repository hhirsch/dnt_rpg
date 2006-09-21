#ifndef _part1_h
#define _part1_h

#include "particle.h"

/*!
 ******************************************
 *            Max Planes Number           *
 ******************************************/
#define PART1_MAX_PLANES 10

#define PLANE_NO_INCLINATION 0 /**< No Inclination on Plane */
#define PLANE_INCLINATION_X  1 /**< Inclination on X axis */
#define PLANE_INCLINATION_Z  2 /**< Inclination on Z axis */

/*! Intersection Plane */
typedef struct 
{
   float x1,        /**< X coordinate of plane */
         x2;        /**< X coordinate of plane */
   float y1,        /**< Y coordinate of plane */
         y2;        /**< Y coordinate of plane */
   float z1,        /**< Z coordinate of plane */
         z2;        /**< Z coordinate of plane */
   float dX,        /**< Variation X of water on plane */
         dZ;        /**< Variation Z of water on plane */
   int inclination; /**< Inclination Type of plane */
}interPlane; 

/*! WaterFalls Particle */
class part1: public particleSystem
{
   public:
      /*! Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position 
       * \param fileName -> file name of the file to load */
      part1(float cX,float cY,float cZ, string fileName); 
      /*! Destructor */
      ~part1();

      /*!
       ***************************************************************
       * Render one particle on screen
       * \param part -> particle to render
       ***************************************************************/
      void Render(particle* part);
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
       ***************************************************************
       * actualize particles attributes (with global independent
       *         forces and influentions).
       * \param part -> particle to actualize
       ***************************************************************/
      void actualize(particle* part);
      /*!
       ***************************************************************
       * Verifies if the particle continue live or not.
       * \param part -> particle to verify
       * \return true if particle continue live, false, otherwise.
       ***************************************************************/
      bool continueLive(particle* part);
      /*!
       ***************************************************************
       * Total Particles needed to create on this step
       * \return Number of particles to create
       ***************************************************************/
      int needCreate();
      /*!
       ***************************************************************
       * Create a particle (its position, color, etc);
       * \param part -> particle struct that will have the new created;
       ***************************************************************/
      void createParticle(particle* part);
      /*!
       ***************************************************************
       * Do a step on system after sec seconds
       * \param matriz -> View Frustum Matrix
       ***************************************************************/
      void NextStep(GLfloat matriz[6][4]);
      /*!
       ***************************************************************
       * Gets the number of actual active particles
       * \return number of actual living particles. 
       ***************************************************************/
      int numParticles();
      /*!
       ***************************************************************
       * Add one plane to internal Planes
       * \param x1 -> initial x plane coordinate 
       * \param y1 -> initial y plane coordinate
       * \param z1 -> initial z plane coordinate
       * \param x2 -> final x plane coordinate 
       * \param y2 -> final y plane coordinate
       * \param z2 -> final z plane coordinate
       * \param dX -> particle X variation when plane contact
       * \param dZ -> particle Z variation when plane contact
       * \param inclination -> plane inclination type
       ***************************************************************/
      void addPlane(float x1, float y1, float z1, 
                    float x2, float y2, float z2, 
                    float dX, float dZ, int inclination);
      /*!
       ***************************************************************
       * Removes The Character's Planes
       ***************************************************************/
      void removeCharacterPlanes();
      /*!
       ***************************************************************
       * Verify if Particle intersects one internal Plane
       * \param part -> particle struct;
       * \param dX -> Variation on X particle axys when intersects
       * \param dZ -> Variation on Z particle axys when intersects
       * \return true if intersects.
       ***************************************************************/
      bool intersectPlanes(particle* part, float* dX, float* dZ); 

   private:
      float seconds;         /**< Actual Time on particle */
      GLuint partTexture;    /**< Current particle texture */
      interPlane intersections[PART1_MAX_PLANES]; /**< The internal Planes */
      int actualPlanes;      /**< actual number of planes */

      /*!
       **************************************************************
       * Load the particle texture file
       * \param fileName -> texture file name;
       ***************************************************************/
      GLuint LoadTexture(char* fileName);
};


#endif


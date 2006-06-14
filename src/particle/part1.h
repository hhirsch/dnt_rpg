#ifndef _part1_h
#define _part1_h

#include "particle.h"

/******************************************
 *            Max Planes Number           *
 ******************************************/
#define PART1_MAX_PLANES 10

/******************************************
 *        Plane Inclination Types         *
 ******************************************/
#define PLANE_NO_INCLINATION 0
#define PLANE_INCLINATION_X  1
#define PLANE_INCLINATION_Z  2

typedef struct 
{
   float x1,x2;     /* X's coordinates of plane */
   float y1,y2;     /* Y's coordinates of plane */
   float z1,z2;     /* Z's coordinates of plane */
   float dX, dZ;    /* Variation of water on plane */
   int inclination; /* Inclination Type of plane */
}interPlane; /* Intersection Plane */

class part1: public particleSystem
{
   public:
      part1(float cX,float cY,float cZ, string fileName);
      ~part1();

      /***************************************************************
       * Reason: Render one particle on screen
       * Param: 
       *         part -> particle to render
       ***************************************************************/
      void Render(particle* part);
      /***************************************************************
       * Reason: Do things before render (like glBegin)
       * Param: 
       ***************************************************************/ 
      void InitRender();
      /***************************************************************
       * Reason: Do things after render (like glEnd)
       * Param: 
       ***************************************************************/ 
      void EndRender();
      /***************************************************************
       * Reason: actualize particles attributes (with global independent
       *         forces and influentions).
       * Param: 
       *         part -> particle to actualize
       ***************************************************************/
      void actualize(particle* part);
      /***************************************************************
       * Reason: true if particle continue live, false, otherwise.
       * Param: 
       *         part -> particle to verify
       ***************************************************************/
      bool continueLive(particle* part);
      /***************************************************************
       * Reason: Total Particles needed to create on this step
       * Param: 
       ***************************************************************/
      int needCreate();
      /***************************************************************
       * Reason: create a particle (its position, color, etc);
       * Param: 
       *         part -> particle struct that will have the new created;
       ***************************************************************/
      void createParticle(particle* part);
      /***************************************************************
       * Reason: Do a step on system after sec seconds
       * Param: 
       *        sec -> time (on seconds);
       ***************************************************************/
      void NextStep(float sec);
      /***************************************************************
       * Reason: Return the number of actual active particles
       * Param: 
       ***************************************************************/
      int numParticles();
      /***************************************************************
       * Reason: Add one plane to internal Planes
       * Param: 
       *         x1,y1,z1 -> initial plane coordinates
       *         x2,y2,z2 -> final plane coordinates
       *         dX, dZ   -> particle axys variation when plane contact
       *         inclination -> plane inclination type
       ***************************************************************/
      void addPlane(float x1, float y1, float z1, 
                    float x2, float y2, float z2, 
                    float dX, float dZ, int inclination);
      /***************************************************************
       * Reason: Remove The Character's Planes
       * Param: 
       ***************************************************************/
      void removeCharacterPlanes();
      /***************************************************************
       * Reason: Verify if Particle intersects one internal Plane
       * Param: 
       *         part -> particle struct;
       *         dX -> Variation on X particle axys when intersects
       *         dZ -> Variation on Z particle axys when intersects
       ***************************************************************/
      bool intersectPlanes(particle* part, float* dX, float* dZ); 

   private:
      float seconds;         /* Actual Time on particle */
      GLuint partTexture;    /* Current particle texture */
      interPlane intersections[PART1_MAX_PLANES]; /* The internal Planes */
      int actualPlanes;      /* actual number of planes */

      /***************************************************************
       * Reason: Load the particle texture file
       * Param: 
       *        fileName -> texture file name;
       ***************************************************************/
      GLuint LoadTexture(char* fileName);
};


#endif


#ifndef _part1_h
#define _part1_h

#include "particle.h"

#define PART1_MAX_PLANES 10

#define PLANE_NO_INCLINATION 0
#define PLANE_INCLINATION_X  1
#define PLANE_INCLINATION_Z  2

typedef struct 
{
   float x1,x2;  /* X's coordinates of plane */
   float y1,y2;  /* Y's coordinates of plane */
   float z1,z2;  /* Z's coordinates of plane */
   float dX, dZ; /* Variation of water on plane */
   int inclination;
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

      void NextStep(float sec);

      int numParticles();
 
      void addPlane(float x1, float y1, float z1, 
                    float x2, float y2, float z2, 
                    float dX, float dZ, int inclination);

      void removePlane(float x1, float y1, float z1, 
                       float x2, float y2, float z2);

      bool intersectPlanes(particle* part, float* dX, float* dZ); 

   private:
      float seconds; 
      GLuint partTexture;
      GLuint LoadTexture(char* fileName);
      interPlane intersections[PART1_MAX_PLANES];
      int actualPlanes;

   
};


#endif


#ifndef _part3_h
#define _part3_h

#include "particle.h"

class part3: particleSystem
{
   public:
      part3(float cX,float cY,float cZ);
      ~part3();

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

   private:
      float centerX,centerY, centerZ;
      float gravity;

      float alpha;
      float seconds; 
   
};


#endif


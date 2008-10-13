#ifndef _part3_h
#define _part3_h

#include "particle.h"

/*! Water Surface Particle */
class part3: public particleSystem
{
   public:
      /*! Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position */
      part3(float cX,float cY,float cZ);
      /*! Destructor */
      ~part3();

      /*!
       ***************************************************************
       * Render one particle on screen
       * \param part -> particle to render
       ***************************************************************/
      void render(particle* part);
      /*!
       ***************************************************************
       * Do things before render (like glBegin)
       ***************************************************************/  
      void initRender();
      /*!
       ***************************************************************
       * Do things after render (like glEnd)
       ***************************************************************/ 
      void endRender();
      /*!
       ****************************************************************
       * Update particles attributes (with global independent
       *         forces and influentions).
       * \param part -> particle to actualize
       ***************************************************************/
      void update(particle* part);
      /*!
       ***************************************************************
       * Verifies if a particle continue live
       * \param part -> particle to verify
       * \return true if particle continue live, false, otherwise.
       ***************************************************************/
      bool continueLive(particle* part);
      /*!
       ***************************************************************
       * Total Particles needed to create on this step
       * \return number of particles to create. 
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
       * Do Next Step on System, rendering and actualizing
       * \param matriz -> view Frustum Matrix 
       ***************************************************************/
      void nextStep(GLfloat** matriz);
      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();
   
   private:
      float otherX,  /**< X auxiliar coordinate */
            otherY,  /**< Y auxiliar coordinate */
            otherZ;  /**< Z auxiliar coordinate */
      int time;      /**< Time passed (on frames) from particle creation */
      float seconds; /**< \todo Seconds, not used anymore.  */
      int xCoord, yCoord; /* Texture coordinate for actual particle */
};


#endif


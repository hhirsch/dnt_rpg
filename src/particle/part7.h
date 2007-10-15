#ifndef _part7_h
#define _part7_h

#include "particle.h"

/*! Snow Particle */
class part7: public particleSystem
{
   public:
      /*! Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position 
       * \param fileName -> file name of the file to load */
      part7(float cX,float cY,float cZ, string fileName);
      /*! Destructor */
      ~part7();

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
       ****************************************************************
       * Actualize particles attributes (with global independent
       *         forces and influentions).
       * \param part -> particle to actualize
       ***************************************************************/
      void actualize(particle* part);
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
       * \param matriz -> viewfrustum matrix
       ***************************************************************/
      void NextStep(GLfloat matriz[6][4]);
      /*!
       ***************************************************************
       * Gets total living particles.
       * \return number of living particles
       ***************************************************************/
      int numParticles();
      /*!
       **************************************************************
       * Load the particle texture file
       * \param fileName -> texture file name;
       ***************************************************************/
      GLuint LoadTexture(string fileName);


   private:
      float seconds;     /**< Seconds, not used anymore. */
      GLuint partTexture;/**< GL Particle Texture */
};


#endif


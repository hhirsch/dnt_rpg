#ifndef _part5_h
#define _part5_h

#include "particle.h"

#define MAX_BLOOD_LIVING  1200 /**< Max Iterations that the blood continues */
#define BLOOD_REMOVING     200 /**< Time for removing blood particles */
#define MAX_BLOOD_REMOVING (MAX_BLOOD_LIVING - BLOOD_REMOVING)

/*! Blood Particle */
class part5: public particleSystem
{
   public:
      /*! Contructor
       * \param cX -> center X position
       * \param cY -> center Y position
       * \param cZ -> center Z position 
       * \param fileName -> file name of the file to load */
      part5(float cX,float cY,float cZ, string fileName);
      ~part5();

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
       * \param matriz -> view frustum Matrix
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

      GLuint getLivingTime();

   private:
      float seconds;      /**< Seconds, not used anymore. */
      GLuint partTexture; /**< Particle Texture */
      GLuint livingTime;  /**< Living Time */
      bool doneCreation;  /**< Mark when finished the creation period */
};


#endif


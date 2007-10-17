/* grass particle */

#ifndef _grass_h
#define _grass_h

#include "particle.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

class grass:public particleSystem
{
public:
      /*! Contructor
       * \param cX1 -> center X1 position
       * \param cZ1 -> center Z1 position
       * \param cX2 -> center X2 position
       * \param cZ2 -> center Z2 position
       * \param total -> total number of particles
       * \param scale -> scale of the grass
       * \param fileName -> file name of the file to load */
      grass(float cX1,float cZ1, float cX2, float cZ2, int total,
            float scale, string fileName);
      ~grass();

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
       ***************************************************************
       * Define the opened map
       * \param actualMap -> pointer to the opened map
       ***************************************************************/
      void defineMap(void* actualMap);

      /*!
       ***************************************************************
       * Get The Area of the grass
       * \param cX1 -> x1 coordinate
       * \param cZ1 -> z1 coordinate
       * \param cX2 -> x2 coordinate
       * \param cZ2 -> z2 coordinate
       ***************************************************************/
      void getPosition(GLfloat& cX1, GLfloat& cZ1, GLfloat& cX2, GLfloat& cZ2 );

      /*!
       ***************************************************************
       * Get the filename of the Grass texture used
       * \return name of the grass texture file
       ***************************************************************/
      string getGrassFileName();

      /*!
       ***************************************************************
       * Get the scale factor of the grass
       * \return the scale factor of the grass
       ***************************************************************/
      GLfloat getScaleFactor();

      /*!
       ***************************************************************
       * Set the scale factor of the grass
       * \param scale -> the scale factor of the grass
       ***************************************************************/
      void setScaleFactor(GLfloat scale);

      /*!
       ***************************************************************
       * Set the total number of grass
       * \param total -> the total number of grass
       ***************************************************************/
      void setTotal(int total);

   private:
      /*!
       ***************************************************************
       * Set the size and Y position of the grass particle
       * \param part -> the particle to set
       ***************************************************************/
      void defineSize(particle* part);

      GLfloat seconds;      /**< Seconds, not used anymore. */
      GLfloat centerX1,     /**< Grass Area X1 */
            centerX2,       /**< Grass Area X2 */
            centerZ1,       /**< Grass Area Z1 */
            centerZ2;       /**< Grass Area Z2 */
      string grassFileName; /**< Name of the grass file texture */
      GLuint grassTexture;  /**< Load Texture used to grass */
      GLfloat scaleFactor;  /**< Scale factor of the grass */
      void* usedMap;        /**< Actual Openned Map */
};


#endif


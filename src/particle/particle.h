/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_particle_h
#define _dnt_particle_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#include "../etc/dirs.h"
#include "../etc/extensions.h"
#include "../etc/list.h"

#define PARTICLE_STATUS_DEAD   0  /**< Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE  1  /**< Say that a particle is alive */
#define PARTICLE_STATUS_STATIC 2  /**< Says a particle don't change anymore */

#define PARTICLE_DRAW_GROUPS     0 /**< Draw particles as vertex group */
#define PARTICLE_DRAW_INDIVIDUAL 1 /**< Draw each particle individually */

/*! The particle system types */
enum
{
    DNT_PARTICLE_TYPE_NONE=0,
    DNT_PARTICLE_TYPE_WATERFALL,
    DNT_PARTICLE_TYPE_FIRE,
    DNT_PARTICLE_TYPE_WATER_SURFACE,
    DNT_PARTICLE_TYPE_SMOKE,
    DNT_PARTICLE_TYPE_BLOOD,
    DNT_PARTICLE_TYPE_LIGHTNING,
    DNT_PARTICLE_TYPE_SNOW,
    DNT_PARTICLE_TYPE_GRASS,
    DNT_PARTICLE_TYPE_METEOR
};

/*! Number of actualizations to stabilize a system */
#define PART_STABILIZE_LOOP 600 

/*! A Particle Alone */
class particle
{
   public:
      float posX, posY, posZ;    /**< Position */
      float prvX, prvY, prvZ;    /**< Previous Position */
      float velX, velY, velZ;    /**< Velocity */
      float size;                /**< Size */
      float R,G,B;               /**< Color */
      float prvR, prvG, prvB;    /**< Previous Color */
      int age;                   /**< Age */
      int status;                /**< Actual Status */
      int internalNumber;        /**< Internal Number */
};

/*! The Particle System */
class particleSystem: public dntListElement
{
   public:
      /*!
       ***************************************************************
       * Construct internal structs                                  *
       * \param total -> max particles to create                     *
       * \param mode -> draw mode                                    *
       ***************************************************************/
      particleSystem(int total, int mode);
      /*!
       ***************************************************************
       * Construct internal structs with default values              *
       ***************************************************************/
      particleSystem();
      /*!
       ***************************************************************
       * Construct internal structs                                  *
       * \param fileName -> particle FileName to load                *
       * \param mode -> draw mode                                    *
       ***************************************************************/
      particleSystem(string fileName, int mode);
      /*!
       ***************************************************************
       * Destroy internal structs                                    *
       ***************************************************************/
      virtual ~particleSystem();

      /*!
       ***************************************************************
       * Init the System                                             
       * \param total -> total particles 
       * \param mode -> draw mode
       ***************************************************************/
      void init(int total, int mode);
      
       /*!
       ***************************************************************
       * Finish the System                                             
       ***************************************************************/
      void finish();
      
      /*!
       ***************************************************************
       * Save the system to a file
       * \param fileName -> name of file to save
       * \return -> true if can save the particle
       ***************************************************************/
      bool save(string fileName);

      /*!
       ***************************************************************
       * Do a step on system after sec seconds
       * \param matriz -> View Frustum Matrix
       ***************************************************************/
      virtual void nextStep(GLfloat** matriz)=0;
      /*!
       ***************************************************************
       * Render one particle on screen     
       *  \param part -> particle to render
       ***************************************************************/
      virtual void render(particle* part)=0;
      /*!
       ***************************************************************
       * Do things before render (like glBegin)    
       ***************************************************************/ 
      virtual void initRender()=0;
      /*!
       ***************************************************************
       * Do things after render (like glEnd)  
       ***************************************************************/ 
      virtual void endRender()=0;
      /*!
       ***************************************************************
       * update particles attributes (with global independent
       *         forces and influentions). 
       * \param part -> particle to actualize
       ***************************************************************/
      virtual void update(particle* part)=0;
      /*!
       ***************************************************************
       * Vertifies if a particle continue live or not
       * \param part -> particle to verify  
       * \return  true if particle continue live, false, otherwise.
       ***************************************************************/
      virtual bool continueLive(particle* part)=0;
      /*!
       ***************************************************************
       * Total Particles needed to create on this step
       ***************************************************************/
      virtual int needCreate()=0;
      /*!
       ***************************************************************
       * Create a particle (its position, color, etc); 
       * \param part -> particle struct that will have the new created;
       ***************************************************************/
      virtual void createParticle(particle* part)=0;
      /*!
       ***************************************************************
       * Actualize and render all particles  
       * \param matriz -> viewFrustum Matrix
       ***************************************************************/
      void doStep(GLfloat** matriz); 
      /*!
       ***************************************************************
       * Define XZ position
       * \param cX -> X position
       * \param cZ -> Z position
       ***************************************************************/
      void definePosition(float cX, float cZ);
      /*!
       ***************************************************************
       * Define XZ position
       * \param cX -> X position
       * \param cY -> Y position
       * \param cZ -> Z position
       ***************************************************************/
      void definePosition(float cX, float cY, float cZ);

      /*!
       *************************************************************** 
       * Get the FileName of the particleSystem
       * \return -> particleSystem fileName 
       ***************************************************************/
      string getFileName();

      /*!
       *************************************************************** 
       * Get the current position of the particleSystem
       * \param x -> current X position
       * \param y -> current Y position
       * \param z -> current Z position  
       ***************************************************************/      
      void getPosition(GLfloat& x, GLfloat &y, GLfloat& z);
      
      /*!
       *************************************************************** 
       * Get the max number of particles on the system
       * \return -> max number of particles on the system 
       ***************************************************************/
      int getMaxParticles();

      /*!
       ***************************************************************
       * Get the number of actual active particles
       * \return number of actual living particles. 
       ***************************************************************/
      int numParticles();

      /*!
       *************************************************************** 
       * Set if the particleSystem will follow the PC or not
       * \param follow -> pointer to the character to follow
       * \param isPC -> true if follow character is a PC
       ***************************************************************/
      void setFollowCharacter(void* follow, bool isPC);

      /*!
       *************************************************************** 
       * Set the duration time to the system be deleted 
       * from controller
       * \param time -> duration time in ms. 0 for infinity duration  
       ***************************************************************/
      void setDurationTime(int time);

      friend class partController;  /**< The controller is our friend */
      friend class particleList;    /**< The list is our friend too */

   protected:

      int type;                  /**< The particle type constant */

      int systemInitialLiveTime; /**< Time the Particle start live  */
      int systemMaxLiveTime;     /**< Max living time, in ms. 0 is infinity */

      int actualParticles;     /**< Number of actual alive particles */
      int drawMode;            /**< DrawMode of the system */
      void* followCharacter;   /**< If the orign of System Follows a charactr */
      bool followIsPC;         /**< If the follow character is a PC */
      bool windAffect;         /**< If Wind Affects the System */

      string strFileName;      /**< Name of the File */
      int maxLive;             /**< Max live of a particle  */
      int maxParticles;        /**< Max number of particles */
      float* vertexArray;      /**< Vertex array to draw */
      float* colorArray;       /**< Color array to draw */
      float centerX,           /**< X initial position */
            centerY,           /**< Y initial position */
            centerZ;           /**< Z initial position */
      float gravity;           /**< Force of Gravity */
      float initR,             /**< Initial Red Color */ 
            initG,             /**< Initial Green Color */
            initB;             /**< Initial Blue Color */
      float finalR,            /**< Final Red Color */
            finalG,            /**< Final Green Color */
            finalB;            /**< Final Blue Color */
      float alpha;             /**< Global Alpha Value */

      float initVelX,          /**< Initial Velocity at X */
            initVelY,          /**< Initial Velocity at Y */
            initVelZ;          /**< Initial Velocity at Z */

      float boundX1;           /**< Bounding Box */
      float boundX2;           /**< Bounding Box */
      float boundY1;           /**< Bounding Box */
      float boundY2;           /**< Bounding Box */
      float boundZ1;           /**< Bounding Box */
      float boundZ2;           /**< Bounding Box */

      /* Probability Values */
 
      float dMultCenter[3];    /**< Multiply values to Center Position */
      float dSumCenter[3];     /**< Sum Values to Center Position */

      float dMultPos[3];       /**< Multiply values to Actual Position */
      float dSumPos[3];        /**< Sum Values to Actual Position */

      float dMultColor[3];     /**< Multiply values to inital Color */
      float dSumColor[3];      /**< Sum Values to initial Color */

      float dMultVel[3];       /**< Multiply values to Actual Velocity */
      float dSumVel[3];        /**< Sum Values to Actual Velocity */

      particle* particles;     /**< Internal Particles Vector */

      GLuint partTexture;      /**< Current particle texture */
      
      extensions ext;          /**< The OpenGL Extensions */


      /*!
       ***************************************************************
       * Reset The Bounding Box
       ***************************************************************/
      void resetBoundingBox();

      /*!
       **************************************************************
       * Load the particle texture file
       * \param fileName -> texture file name;
       ***************************************************************/
      GLuint loadTexture(string fileName);

};

#endif


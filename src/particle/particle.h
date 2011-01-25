/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "partelement.h"
#include "partorigin.h"

#include "../etc/dirs.h"
#include "../etc/extensions.h"
#include "../etc/list.h"

#define PARTICLE_STATUS_DEAD    0  /**< Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE   1  /**< Say that a particle is alive */
#define PARTICLE_STATUS_STATIC  2  /**< Says a particle don't change anymore */

#define DNT_PARTICLE_DRAW_GROUP      0 /**< Draw particles as vertex group */
#define DNT_PARTICLE_DRAW_INDIVIDUAL 1 /**< Draw each particle individually */

#define DNT_PARTICLE_RENDER_DEFAULT  0  /**< Default renderer */
#define DNT_PARTICLE_RENDER_GLOW     1  /**< Glow renderer (for fire) */

#define DNT_PARTICLE_SYSTEM_FOLLOW_NONE   0
#define DNT_PARTICLE_SYSTEM_FOLLOW_HEAD   1
#define DNT_PARTICLE_SYSTEM_FOLLOW_PC     2

/*! The particle system types */
enum
{
    DNT_PARTICLE_SYSTEM_TYPE_DEFAULT=0,
    DNT_PARTICLE_SYSTEM_TYPE_WATERFALL
};

/*! Number of actualizations to stabilize a system */
#define PART_STABILIZE_LOOP 600 

/*! A Particle Alone */
class particle
{
   public:
      float posX, posY, posZ;    /**< Position */
      float velX, velY, velZ;    /**< Velocity */
      float size;                /**< Size */
      float R,G,B,A;             /**< Color */
      int age;                   /**< Age */
      int status;                /**< Actual Status */
      int internalNumber;        /**< Internal Number */
};

/*! The Particle System */
class particleSystem: public dntListElement
{
   public:
      /*! Construct internal structs with default values */
      particleSystem();
      /*! Destructor */
      ~particleSystem();

      /*! Load the partSystem from a file
       * \param fileName -> fileName of the particle system to load 
       * \return true if loaded. */
      bool load(string fileName);
      
      /*! Save the system to a file
       * \param fileName -> name of file to save
       * \return -> true if can save the particle */
      bool save(string fileName);

      /*! Update and render all particles  
       * \param matriz -> the viewFrustum Matrix */
      void doStep(GLfloat** matriz); 
      
      /*! Define XZ position of the syste origin
       * \param cX -> X position
       * \param cZ -> Z position */
      void definePosition(float cX, float cZ);
      /*! Define XYZ position of the system origin
       * \param cX -> X position
       * \param cY -> Y position
       * \param cZ -> Z position */
      void definePosition(float cX, float cY, float cZ);

      /*! Get the FileName of the particleSystem
       * \return -> particleSystem fileName  */
      string getFileName();

      /*! Get the current position of the particleSystem
       * \param x -> current X position
       * \param y -> current Y position
       * \param z -> current Z position   */
      void getPosition(GLfloat& x, GLfloat &y, GLfloat& z);
      
      /*! Get the max number of particles on the system
       * \return -> max number of particles on the system */
      int getMaxParticles();

      /*! Get the number of actual active particles
       * \return number of actual living particles. */
      int numParticles();

      /*! Set if the particleSystem will follow the PC or not
       * \param follow -> pointer to the character to follow
       * \param t -> floow type constant */
      void setFollowCharacter(void* follow, int type);

      /*! Define the opened map
       * \param actualMap -> pointer to the opened map */
      void defineMap(void* actualMap);

      /*! Set the duration time to the system be deleted 
       * from controller
       * \param time -> duration time in ms. 0 for infinity duration */ 
      void setDurationTime(int time);

      /*! Get max life time of a single particle
       * \return max life time of a single particle*/
      int getMaxParticleLifeTime();

      friend class partController;  /**< The controller is our friend */
      friend class particleList;    /**< The list is our friend too */

   protected:

      /* Internal things */
      int type;               /**< The particle type constant */
      int maxParticles;       /**< Max number of particles */
      int actualParticles;    /**< Number of actual alive particles */
      void* followCharacter;  /**< If the orign of System Follows a character */
      int followType;         /**< If the follow character is a PC */
      bool windAffect;        /**< If Wind Affects the System */
      bool floorCollision;    /**< If collide with map's floor or not */
      string strFileName;     /**< Name of the File */
      particle* particles;    /**< Internal Particles Vector */
      string textureFileName; /**< Filename of the texture */
      GLuint partTexture;     /**< Current particle texture */
      extensions ext;         /**< The OpenGL Extensions */
      bool doneCreation;      /**< true when is no more to create particles */
      void* curMap;           /**< the current map used */

      /* Life related */
      int initialLifeTime;       /**< Time the Particle start to live  */
      int maxLifeTime;           /**< Max life time, in ms. 0 is infinity */
      int maxParticleLifeTime;   /**< Max live of a particle  */

      /* Render related things */
      int drawMode;            /**< DrawMode of the system */
      int renderMode;          /**< Types of render */
      float* vertexArray;      /**< Vertex array to draw */
      float* colorArray;       /**< Color array to draw */
      int pointSize;           /**< Render Point Size */
     
      /* System specific elements */
      dntPartOrigin origin;       /**< Particle System Origin */
      float gravity;              /**< Force of Gravity */
      int numParticlesToCreate;         /**< total particles to create */
      dntPartElement particlesToCreate; /**< particles to create elemtent */
      dntPartElement color[4];    /**< Color element controller */
      dntPartElement velocity[3]; /**< Velocity element controller */
      dntPartElement position[3]; /**< Position variation element */
      dntPartElement scale;       /**< Scale variation element */
      
      /* System Bounding Box */
      float boundX1;           /**< Bounding Box */
      float boundX2;           /**< Bounding Box */
      float boundY1;           /**< Bounding Box */
      float boundY2;           /**< Bounding Box */
      float boundZ1;           /**< Bounding Box */
      float boundZ2;           /**< Bounding Box */

      /*! Init the System after all values defined */
      void init();

      /*! Reset The Bounding Box */
      void resetBoundingBox();

      /*! Load the particle texture file */
      void loadTexture();

      /*! Total Particles needed to create on this step */
      int needCreate();
      
      /*! Create a particle (its position, color, etc); 
       * \param part -> particle struct that will have the new created; */
      void createParticle(particle* part);

      /*! Verify if a particle continue alive or not
       * \param part- > particle to verify
       * \reuturn true if is time to kill the particle. */
      bool continueLive(particle& part);

      /*! Render one particle on screen     
       *  \param part -> particle to render */
      void render(particle* part);
      /*! Do things before render (like glBegin) */
      void initRender();
      /*! Do things after render (like glEnd)  */
      void endRender();
      /*! update particles attributes (with global independent
       *         forces and influentions). 
       * \param part -> particle to update */
      void update(particle* part);

      /*! update by the character position (if follow is defined) */
      void updateByCharacter();


};

#endif


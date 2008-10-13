#ifndef _particle_h
#define _particle_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#include "../etc/extensions.h"
#include "../etc/dirs.h"

#define PARTICLE_STATUS_DEAD   0  /**< Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE  1  /**< Say that a particle is alive */
#define PARTICLE_STATUS_STATIC 2  /**< Says a particle don't change anymore */

#define PARTICLE_DRAW_GROUPS     0 /**< Draw particles as vertex group */
#define PARTICLE_DRAW_INDIVIDUAL 1 /**< Draw each particle individually */

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
class particleSystem
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
      bool save( string fileName);


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
       * Set if the particleSystem will follow the PC or not
       * \param follow -> true to follow pc, false to not follow  
       ***************************************************************/
      void setFollowPC(bool follow);

      /*!
       *************************************************************** 
       * Set the duration time to the system be deleted 
       * from controller
       * \param time -> duration time in ms. 0 for infinity duration  
       ***************************************************************/
      void setDurationTime(int time);


      particleSystem* next;     /**< Next Particle System on the List */
      particleSystem* previous; /**< Previous Particle System on the list */

      friend class partController;  /**< The controller is our friend */
      friend class particleList;    /**< The list is our friend too */

   protected:

      int systemInitialLiveTime; /**< Time the Particle start live  */
      int systemMaxLiveTime;     /**< Max living time, in ms. 0 is infinity */

      int actualParticles;     /**< Number of actual alive particles */
      int drawMode;            /**< DrawMode of the system */
      bool followPC;           /**< If the orign of System Follows PC */
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


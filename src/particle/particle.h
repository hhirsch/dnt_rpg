#ifndef _particle_h
#define _particle_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#define PARTICLE_STATUS_DEAD   0  /**< Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE  1  /**< Say that a particle is alive */
#define PARTICLE_STATUS_STATIC 2  /**< Says a particle don't change anymore */

#define PARTICLE_DRAW_GROUPS     0 /**< Draw particles as vertex group */
#define PARTICLE_DRAW_INDIVIDUAL 1 /**< Draw each particle individually */

/*! A Particle Alone */
typedef struct part_
{
   float posX, posY, posZ;    /**< Position */
   float prvX, prvY, prvZ;    /**< Previous Position */
   float velX, velY, velZ;    /**< Velocity */
   float size;                /**< Size */
   float R,G,B;               /**< Color */
   float prvR, prvG, prvB;    /**< Previous Color */
   int age;                   /**< Age */
   int status;                /**< Actual Status */
   int internalNumber;        /**< Internal Number */
}particle;

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
       * Save the system to a file
       * \param fileName -> name of file to save
       ***************************************************************/
      void Save( string fileName);


      /*!
       ***************************************************************
       * Render one particle on screen     
       *  \param part -> particle to render
       ***************************************************************/
      virtual void Render(particle* part)=0;
      /*!
       ***************************************************************
       * Do things before render (like glBegin)    
       ***************************************************************/ 
      virtual void InitRender()=0;
      /*!
       ***************************************************************
       * Do things after render (like glEnd)  
       ***************************************************************/ 
      virtual void EndRender()=0;
      /*!
       ***************************************************************
       * actualize particles attributes (with global independent
       *         forces and influentions). 
       * \param part -> particle to actualize
       ***************************************************************/
      virtual void actualize(particle* part)=0;
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
      void DoStep(GLfloat matriz[6][4]); 
      /*!
       ***************************************************************
       * Define XZ position
       * \param cX -> X position
       * \param cZ -> Z position
       ***************************************************************/
      void definePosition(float cX, float cZ);
      void definePosition(float cX, float cY, float cZ);

      string getFileName(){return(strFileName);};
      void getPosition(GLfloat& x, GLfloat &y, GLfloat& z){x=centerX;y=centerY;z=centerZ;};

      int actualParticles;   /**< Number of actual alive particles */
      int drawMode;          /**< DrawMode of the system */
      bool followPC;         /**< If the orign of System Follows PC */
      bool windAffect;       /**< If Wind Affects the System */

      int getMaxParticles(){return(maxParticles);};
      

   PFNGLPOINTPARAMETERFARBPROC PointParameterf; /**< OpenGL extended function*/  
   PFNGLPOINTPARAMETERFVARBPROC PointParameterfv;/**< OpenGL extended function*/

   protected:

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


      /*!
       ***************************************************************
       * Reset The Bounding Box
       ***************************************************************/
      void resetBoundingBox();

};

#endif


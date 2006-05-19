#ifndef _particle_h
#define _particle_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#define PARTICLE_STATUS_DEAD   0  /* Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE  1  /* Say that a particle is alive */
#define PARTICLE_STATUS_STATIC 2  /* Say that a particle don't change anymore */

#define PARTICLE_DRAW_GROUPS     0 /* Draw particles as vertex group */
#define PARTICLE_DRAW_INDIVIDUAL 1 /* Draw each particle individually */

/* A Particle Alone */
typedef struct part_
{
   float posX, posY, posZ;    /* Position */
   float prvX, prvY, prvZ;    /* Previous Position */
   float velX, velY, velZ;    /* Velocity */
   float size;                /* Size */
   float R,G,B;               /* Color */
   float prvR, prvG, prvB;    /* Previous Color */
   int age;                   /* Age */
   int status;                /* Actual Status */
   int internalNumber;        /* Internal Number */
}particle;

/* The Particle System */
class particleSystem
{
   public:
      /***************************************************************
       * Reason: construct internal structs                          *
       * Param:                                                      *
       *         total -> max particles to create                    *
       *         mode -> draw mode                                   *
       ***************************************************************/
      particleSystem(int total, int mode);
      /***************************************************************
       * Reason: construct internal structs                          *
       * Param:                                                      *
       *         fileName -> particle FileName to load               *
       *         mode -> draw mode                                   *
       ***************************************************************/
      particleSystem(string fileName, int mode);
      /***************************************************************
       * Reason: destroy internal structs                            *
       * Param:                                                      *
       ***************************************************************/
      virtual ~particleSystem();

      void init(int total, int mode);
      void Save( string fileName);


      /***************************************************************
       * Reason: Render one particle on screen                       *
       * Param:                                                      *
       *         part -> particle to render                          *
       ***************************************************************/
      virtual void Render(particle* part)=0;
      /***************************************************************
       * Reason: Do things before render (like glBegin)              *
       * Param:                                                      *
       ***************************************************************/ 
      virtual void InitRender()=0;
      /***************************************************************
       * Reason: Do things after render (like glEnd)                 *
       * Param:                                                      *
       ***************************************************************/ 
      virtual void EndRender()=0;
      /***************************************************************
       * Reason: actualize particles attributes (with global indepen-*
       *         dent forces and influentions).                      *
       * Param:                                                      *
       *         part -> particle to actualize                       *
       ***************************************************************/
      virtual void actualize(particle* part)=0;
      /***************************************************************
       * Reason: true if particle continue live, false, otherwise.   *
       * Param:                                                      *
       *         part -> particle to verify                          *
       ***************************************************************/
      virtual bool continueLive(particle* part)=0;
      /***************************************************************
       * Reason: Total Particles needed to create on this step       *
       * Param:                                                      *
       ***************************************************************/
      virtual int needCreate()=0;
      /***************************************************************
       * Reason: create a particle (its position, color, etc);       *
       * Param:                                                      *
       *      part -> particle struct that will have the new created;*
       ***************************************************************/
      virtual void createParticle(particle* part)=0;
      /***************************************************************
       * Reason: Actualize and render all particles                  *
       * Param:                                                      *
       ***************************************************************/
      void DoStep(); 

      /* Public Variables */
      int actualParticles;   /* Number of actual alive particles */
      int drawMode;          /* DrawMode of the system */
      

      /* The OpenGL extended point functions */
      PFNGLPOINTPARAMETERFARBPROC PointParameterf;   
      PFNGLPOINTPARAMETERFVARBPROC PointParameterfv;

   protected:

      int maxLive;             /* Max live of a particle  */
      int maxParticles;        /* Max number of particles */
      float* vertexArray;      /* Vertex array to draw */
      float* colorArray;       /* Color array to draw */
      float centerX,           /* X initial position */
            centerY,           /* Y initial position */
            centerZ;           /* Z initial position */
      float gravity;           /* Force of Gravity */
      float initR,             /* Initial Red Color */ 
            initG,             /* Initial Green Color */
            initB;             /* Initial Blue Color */
      float finalR,            /* Final Red Color */
            finalG,            /* Final Green Color */
            finalB;            /* Final Blue Color */
      float alpha;             /* Global Alpha Value */

      /* Probability Values */
 
      float dMultCenter[3];    /* Multiply values to Center Position */
      float dSumCenter[3];     /* Sum Values to Center Position */

      float dMultPos[3];       /* Multiply values to Actual Position */
      float dSumPos[3];        /* Sum Values to Actual Position */

      float dMultColor[3];     /* Multiply values to inital Color */
      float dSumColor[3];      /* Sum Values to initial Color */

      float dMultVel[3];       /* Multiply values to Actual Velocity */
      float dSumVel[3];        /* Sum Values to Actual Velocity */

      particle* particles;     /* Internal Particles Vector */

};

#endif


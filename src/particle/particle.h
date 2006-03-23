#ifndef _particle_h
#define _particle_h

#define PARTICLE_STATUS_DEAD   0  /* Say that a particle is dead */
#define PARTICLE_STATUS_ALIVE  1  /* Say that a particle is alive */
#define PARTICLE_STATUS_STATIC 2  /* Say that a particle don't change anymore */

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
       * Reason: construct internal structs
       * Param: 
       *         total -> max particles to create
       ***************************************************************/
      particleSystem(int total);
      /***************************************************************
       * Reason: destruct internal structs
       * Param: 
       ***************************************************************/
      virtual ~particleSystem();


      /***************************************************************
       * Reason: Render one particle on screen
       * Param: 
       *         part -> particle to render
       ***************************************************************/
      virtual void Render(particle* part)=0;
      /***************************************************************
       * Reason: Do things before render (like glBegin)
       * Param: 
       ***************************************************************/ 
      virtual void InitRender()=0;
      /***************************************************************
       * Reason: Do things after render (like glEnd)
       * Param: 
       ***************************************************************/ 
      virtual void EndRender()=0;
      /***************************************************************
       * Reason: actualize particles attributes (with global independent
       *         forces and influentions).
       * Param: 
       *         part -> particle to actualize
       ***************************************************************/
      virtual void actualize(particle* part)=0;
      /***************************************************************
       * Reason: true if particle continue live, false, otherwise.
       * Param: 
       *         part -> particle to verify
       ***************************************************************/
      virtual bool continueLive(particle* part)=0;
      /***************************************************************
       * Reason: Total Particles needed to create on this step
       * Param: 
       ***************************************************************/
      virtual int needCreate()=0;
      /***************************************************************
       * Reason: create a particle (its position, color, etc);
       * Param: 
       *         part -> particle struct that will have the new created;
       ***************************************************************/
      virtual void createParticle(particle* part)=0;

   
      /***************************************************************
       * Reason: Actualize and render all particles
       * Param: 
       ***************************************************************/
      void DoStep(); 

      int actualParticles; 
      

   protected:
      particle* particles;
      int       maxParticles;
};

#endif


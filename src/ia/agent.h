#ifndef _agent_h
#define _agent_h

#include <SDL/SDL_opengl.h>

#define MAX_OBSTACLES 30

/*! Obstacle Percepted */
class obstacle
{
   public:
      GLfloat x;       /**< x coordinate */
      GLfloat z;       /**< z coordinate */
};


/*! This Class Defines the Generic AI Agent */
class agent
{
   public:
      /*! Constructor */
      agent(bool orientation);
      /*! Destructor */
      virtual ~agent();

      /*! Actualize Agent */
      void actualize();

      /*! Define Destiny 
       * \param x -> x destiny coordinate
       * \param z -> z destiny coordinate */
      void defineDestiny(GLfloat x, GLfloat z);

      /*! Define Step Size 
       * \param size -> step size */
      void defineStepSize(GLfloat size);

      /*! Define Actual Agent Position 
       * \param x -> x position coordinate
       * \param z -> z position coordinate */
      void definePosition(GLfloat x, GLfloat z);

      /*! Get Actual Agent Position 
       * \param x -> will have X agent coordinate
       * \param z -> will have Z agent coordinate */
      void getPosition(GLfloat& x, GLfloat& z);

      /*! Define Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void defineSight(GLfloat sightDist, GLfloat sightAng);

      /*! Gets Character Sight 
       * \param sightDist -> sight Distance 
       * \param sightAng -> sight Angle */
      void getSight(GLfloat& sightDist, GLfloat& sightAng);

      void addObstacle(GLfloat x, GLfloat z);

      void clearObstacles();


      /* Virtual Methods */

      /*! Defines Next Agent position. Virtual. 
       * \return true if position was defined */
      virtual bool defineNextPosition()=0;

   protected:

         /* DESTINY */
      GLfloat destinyX;      /**< Destiny X  Coordinate */
      GLfloat destinyZ;      /**< Destiny Z Coordinate */

         /* POSITION */
      GLfloat actualX;       /**< Agent Actual X Coordinate */
      GLfloat actualZ;       /**< Agent Actual Z Coordinate */

         /* STEP */
      GLfloat stepSize;      /**< Agent Step Size per Actualization */

         /* ORIENTATION */
      bool withOrientation;  /**< If agent has orientation or not */
      GLfloat orientation;   /**< Agent orientation angle, if has */

         /* SIGHT */
      GLfloat sightDistance; /**< The distance that agent can percept */
      GLfloat sightAngle;    /**< The angle that agent can percept, 
                                  if orientation is enabled */

         /* PERCEPTION */
      obstacle obstacles[MAX_OBSTACLES];   /**< Know obstacles */
      int knowObstacles;                   /**< Number of know obstacles */
};

#endif


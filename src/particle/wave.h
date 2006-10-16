#ifndef _wave_h
#define _wave_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


#define WAVE_DIRECTION_NONE      0
#define WAVE_DIRECTION_UP        2
#define WAVE_DIRECTION_DOWN      4
#define WAVE_DIRECTION_LEFT      8
#define WAVE_DIRECTION_RIGHT    16


class position
{
   public:
      GLfloat x,  y,  z;
      GLfloat px, py, pz;
};

/*! A Liquid Surface Wave */
class wave
{
   public:
      int initialX;           /**< The X particle position on surface */
      int initialZ;           /**< The Z particle position on surface */
      GLfloat amplitude;      /**< Wave amplitude */
      GLfloat deltaAmplitude; /**< Delta Amplitude (how much increase each 
                                   lifetime) */
      int actualLifeTime;     /**< Actual Wave Life Time */
      int generationTime;     /**< Total Time on Generating the Wave */
      int attrition;          /**< The Surface Attrition */

      int direction;          /**< The Wave Direction */
      
      int nearLoop;           /**< The first alive loop */
      int farLoop;            /**< The last alive loop */

      bool dead;              /**< If Wave is Dead or Not */

      wave* next;             /**< Next Wave on List */
      wave* previous;         /**< Previous Wave on List */
};

/*! The Waves Class represents waves on a liquid surface */
class waves
{
   public:
      waves(string waveFile, GLfloat X, GLfloat Y, GLfloat Z, int nX, int nZ);
      ~waves();

      void removeDeadWaves();
      int getTotalWaves();
      void doStep();
      void insertWave(int initialX, int initialZ, 
                      GLfloat amplitude, GLfloat deltaAmplitude, 
                      int generationTime, int attrition,
                      int direction);

      void draw();


   private:

      GLfloat initialX;           /**< The X particle position on surface */
      GLfloat initialZ;           /**< The Z particle position on surface */
      GLfloat initialY;           /**< The Y static position of surface */

      
      void removeWave(wave* remWave);

      wave* actualWaves;

      int totalWaves;

      position** surface;
      int surfX;
      int surfZ;

      
};

#endif


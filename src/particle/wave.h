#ifndef _wave_h
#define _wave_h

#include <string>
using namespace std;

/*! A Liquid Surface Wave */
class wave
{
   public:
      int initialX;           /**< The X particle position on surface */
      int initialZ;           /**< The Z particle position on surface */
      int initialAmplitude;   /**< Initial wave amplitude */
      int deltaAmplitude;     /**< Delta Amplitude (how much increase each 
                                   lifetime) */
      int actualLifeTime;     /**< Actual Wave Life Time */
      int generationTime;     /**< Total Time on Generating the Wave */
      int attrition;          /**< The Surface Attrition */
      
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
      waves(string waveFile);
      ~waves();

      void removeDeadWaves();
      int getTotalWaves();

   private:
      void insertWave(int initialX, int initialZ, int initialAmplitude,
                      int deltaAmplitude, int generationTime, int attrition);

      void removeWave(wave* remWave);

      wave* actualWaves;

      int totalWaves;

      
};

#endif


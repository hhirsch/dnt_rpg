#ifndef _sound_h
#define _sound_h

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>

#include <string>
using namespace std;

#define SOUND_NONE    0  /**< No sound */
#define SOUND_WALK    1  /**< Walk Sound */
#define SOUND_ACTION1 2  /**< Action 1 sound */
#define SOUND_ACTION2 3  /**< Action 2 sound */
#define SOUND_ACTION3 4  /**< Action 3 sound */

/*! The sound definitions */
class sound
{
   public:
      /*! Constructor */
      sound();
      /*! Destructor */
      ~sound();
 
      /*! Load the music from file and start playing
       * \param file -> file name of the music to load
       * \return Mix_Music pointer to the music loaded. */
      Mix_Music* LoadMusic(string file);
      /*! Stop the music
       * \param music -> Mix_Music pointer of the music to stop. */
      void StopMusic(Mix_Music* music);

      /*! Load Sample File to memory. 
       * \param smp ->  sample reference ID. 
       * \param file -> filename of sample to load */
      void LoadSample(int smp, string file);
      /*! Play Sample
       * \param smp ->  sample reference ID.
       */
      void PlaySample(int smp, int cnt=0);
      /*! Stop playing Sample
       * \param smp ->  sample reference ID.
       * */
      void StopSample(int smp);
     
      /*!
       * Change volume of the music and of the samples 
       * \param music -> music volume
       * \param sndfx -> samples volume */
      void ChangeVolume(int music, int sndfx);

   private:
     int         musicVolume; /**< Music Volume */
     int         sndfxVolume; /**< Samples Volume */
     Mix_Chunk*  walk;        /**< Walk Effect */
     Mix_Chunk*  action1;     /**< Action 1 effect */
     Mix_Chunk*  action2;     /**< Action 2 effect */
     Mix_Chunk*  action3;     /**< Action 3 effect */
     int channel[4];          /**< Channel Used By th Sound */
};

#endif


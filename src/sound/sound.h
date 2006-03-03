#ifndef _sound_h
#define _sound_h

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>

#include <string>
using namespace std;

#define SOUND_NONE    0
#define SOUND_WALK    1
#define SOUND_ACTION1 2
#define SOUND_ACTION2 3
#define SOUND_ACTION3 4

class sound
{
   public:
      sound();
      ~sound();
 
      Mix_Music* LoadMusic(string file);
      void StopMusic(Mix_Music* music);

      void LoadSample(int smp, string file);
      void PlaySample(int smp);
      void StopSample(int smp);

      int volume;

   private:
     Mix_Chunk* walk;
     Mix_Chunk* action1;
     Mix_Chunk* action2;
     Mix_Chunk* action3;
};

#endif


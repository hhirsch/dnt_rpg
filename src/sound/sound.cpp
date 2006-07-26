#include "sound.h"

#define CHANNELS         2    /* Stereo Mode */
#define SAMPLE_CHANNELS  4    /* Number of channels to mix */
#define BUFFER           1024 /* Buffer size for sounds */

sound::sound()
{
   if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,CHANNELS,BUFFER)<0)
   {
      printf("Error while opening SDL_MIX\n");
      return;
   }
   Mix_AllocateChannels(SAMPLE_CHANNELS);
   walk = NULL;
   action1 = NULL;
   action2 = NULL;
   action3 = NULL;
   musicVolume = SDL_MIX_MAXVOLUME - 20;
   sndfxVolume = SDL_MIX_MAXVOLUME;

   int i;
   for(i = 0; i<SAMPLE_CHANNELS;i++)
   {
      channel[i] = -1;
   }
   
}

sound::~sound()
{
   Mix_CloseAudio();
   if(walk)
   {
      Mix_FreeChunk(walk);
      walk = NULL;
   }
   if(action1)
   {
      Mix_FreeChunk(action1);
      action1 = NULL;
   }
   if(action2)
   {
      Mix_FreeChunk(action2);
      action2 = NULL;
   }
   if(action3)
   {
      Mix_FreeChunk(action3);
      action1 = NULL;
   }
}

Mix_Music* sound::LoadMusic(string file)
{
   Mix_Music* music;
   music = Mix_LoadMUS( file.c_str() );
   if(!music)
   {
      printf("Error while opening music file: %s\n",file.c_str());
   }
   Mix_PlayMusic(music, -1);
   Mix_VolumeMusic(musicVolume);
   return(music);
}

void sound::StopMusic(Mix_Music* music)
{
   if(music) 
   {
      Mix_HaltMusic();
      Mix_FreeMusic(music);
   }
   return;
}

void sound::LoadSample(int smp, string file)
{
   switch(smp)
   {
      case SOUND_WALK:
      {
         if(walk)
         {
             Mix_FreeChunk(walk);
         }
         walk = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(),"rb"), 0);
         break;
      }
      case SOUND_ACTION1:
      {
         if(action1)
         {
             Mix_FreeChunk(action1);
         }
         action1 = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(),"rb"), 0);
         break;
      }
      case SOUND_ACTION2:
      {
         if(action2)
         {
             Mix_FreeChunk(action2);
         }
         action2 = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(),"rb"), 0);
         break;
      }
      case SOUND_ACTION3:
      {
         if(action3)
         {
             Mix_FreeChunk(action3);
         }
         action3 = Mix_LoadWAV_RW(SDL_RWFromFile(file.c_str(),"rb"), 0);
         break;
      }
   }
}

void sound::PlaySample(int smp, int cnt)
{
   switch(smp)
   {
      case SOUND_WALK:
      {
         if(walk)
         {
            if((channel[0] == -1) || (!Mix_Playing(channel[0])))
            {
               channel[0] = Mix_PlayChannel(-1, walk, cnt);
            }
         }
         break;
      }
      case SOUND_ACTION1:
      {
         if(action1)
         { 
            if( (channel[1] == -1) || (!Mix_Playing(channel[1])))
            {
               channel[1] = Mix_PlayChannel(-1, action1, cnt);
            }
         }
         break;
      }
      case SOUND_ACTION2:
      {
         if(action2)
         {
            if((channel[2] == -1) || (!Mix_Playing(channel[2])))
            {
               channel[2] = Mix_PlayChannel(-1, action2, cnt);
            }
         }
         break;
      }
      case SOUND_ACTION3:
      {
         if(action3)
         {
            if((channel[3] ==-1) || (!Mix_Playing(channel[3])))
            {
               channel[3] = Mix_PlayChannel(-1, action3, cnt);
            }
         }
         break;
      }
   }
}

void sound::StopSample(int smp)
{
   if(channel[smp-1] != -1)
   {
      Mix_HaltChannel(channel[smp-1]);
      channel[smp-1] = -1;
   }
  /* switch(smp)
   {
      case SOUND_WALK:
      {
         Mix_HaltChannel(channel[0]);
         break;
      }
      case SOUND_ACTION1:
      {
         Mix_HaltChannel(2);
         break;
      }
      case SOUND_ACTION2:
      {
         Mix_HaltChannel(3);
         break;
      }
      case SOUND_ACTION3:
      {
         Mix_HaltChannel(4);
         break;
      }
   }*/
}


void sound::ChangeVolume(int music, int sndfx)
{
   musicVolume = music;
   sndfxVolume = sndfx;

   Mix_Volume(-1, sndfxVolume);
   Mix_VolumeMusic(musicVolume);
}


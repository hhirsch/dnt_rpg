#include "sound.h"
#include <math.h>
#include <SDL/SDL.h>
#include "../engine/util.h"

sound* actualSound;

/*************************************************************************
 *                          runParalelSound                              *
 *************************************************************************/
int runParalelSound(void* param)
{
   while(true)
   {
      actualSound->lock();
      actualSound->flush();
      actualSound->unLock();
      SDL_Delay(100);
   }
   return(1);
}

/*************************************************************************
 *                                 lock                                  *
 *************************************************************************/
void sound::lock()
{
   if(SDL_mutexP(soundMutex) != 0)
   {
      printf("Error while Sound Mutex Lock\n");
   }
}

/*************************************************************************
 *                                unLock                                 *
 *************************************************************************/
void sound::unLock()
{
   if(SDL_mutexV(soundMutex) != 0)
   {
      printf("Error while Sound Mutex UnLock\n");
   }
}

/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sound::sound()
{
   // Initialize Open AL
   device = alcOpenDevice(NULL); 
   enabled = false;
   
   if (device != NULL) 
   {
      context=alcCreateContext(device,NULL); 
      if (context != NULL) 
      {
         alcMakeContextCurrent(context);
         enabled = true;
      }
      else
      {
         printf("Can't Create OpenAL Context\n");
      }

   }
   else
   {
      printf("No OpenAL Device Avaible!\n");
   }
   
   /* None current Opened Music */
   backMusic = NULL;

   /* Initialize the Sound Effects Double Linked List with head Node */
   sndfxList.next = &sndfxList;
   sndfxList.previous = &sndfxList;
   totalSndfx = 0;

   musicVolume = 128;
   sndfxVolume = 128;

   actualSound = this;
   if(enabled)
   {
      soundMutex = SDL_CreateMutex();
      soundThread  = SDL_CreateThread((&runParalelSound), NULL);   
   }
}

/*************************************************************************
 *                              Destructor                               *
 *************************************************************************/
sound::~sound()
{
   sndfx* snd, *tmp;
   
   if(enabled)
   {
      /* Clear the Opened Music */
      if(backMusic)
      {
         backMusic->release();
         delete(backMusic);
         backMusic = NULL;
      }

      /* Clear all opened Sound Effects */
      snd = sndfxList.next;
      while(totalSndfx > 0)
      {
         tmp = snd;
         snd = snd->previous;
         removeSoundEffect(tmp);
         snd= snd->next;
      }
   
      /* Destroy Thread and Mutex */
      SDL_KillThread(soundThread);
      SDL_DestroyMutex(soundMutex);
   
      /* Clear OpenAL Context and Device */
      alcDestroyContext(context);
      alcCloseDevice(device);
   }
}

/*************************************************************************
 *                          setListenerPosition                          *
 *************************************************************************/
void sound::setListenerPosition(ALfloat centerX, ALfloat centerY, 
                                ALfloat centerZ, ALfloat theta, ALfloat phi,
                                ALfloat d, ALfloat deltaY)
{
   if(enabled)
   {
      lock();
         ALfloat directionvect[6]; /* Direction Vector of Listener */
         alListener3f(AL_POSITION, centerX, centerY, centerZ);
   
         directionvect[0] = (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
         directionvect[1] = deltaY + (float) d * sin(deg2Rad(theta));
         directionvect[2] = (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));
         directionvect[3] = 0;
         directionvect[4] = 1;
         directionvect[5] = 0;
         alListenerfv(AL_ORIENTATION, directionvect);
      unLock();
   }
}

/*************************************************************************
 *                              loadMusic                                *
 *************************************************************************/
bool sound::loadMusic(string fileName)
{
   if(!enabled)
   {
      return(false);
   }
   
   lock();
   if(backMusic)
   {
      /* Delete Active Music, if one is */
      backMusic->release();
      delete(backMusic);
      backMusic = NULL;
   }

   /* Load The File and Set The active Music */
   backMusic = new(ogg_stream);

   backMusic->open(fileName);
   backMusic->changeVolume(musicVolume);

   if(!backMusic->playback())
   {
      printf("Can't Play Ogg File: %s\n", fileName.c_str());
      delete(backMusic);
      backMusic = NULL;
      return(false);
   }

   backMusic->setLoop(true);
   backMusic->defineAsMusic();

   unLock();
   return(true);
}

/*************************************************************************
 *                                flush                                  *
 *************************************************************************/
void sound::flush()
{
   sndfx* snd;

   if(!enabled)
   {
      return;
   }
   
   /* Music Update */
   if(backMusic)
   {
      if(!backMusic->update()) 
      {
         //backMusic->rewind();
         printf("error while playing music\n");
      }
   }

   /* Sound Effects Update */
   snd = sndfxList.next;
   while(snd != &sndfxList)
   {
      if(!snd->update())
      {
         /* Remove Sound */
         sndfx* tmp = snd;
         snd = snd->previous;
         removeSoundEffect(tmp);
      }
      snd = snd->next;
   }
}

/*************************************************************************
 *                            addSoundEffect                             *
 *************************************************************************/
sndfx* sound::addSoundEffect(ALfloat x, ALfloat y, ALfloat z, bool loop,
                             string fileName)
{
   if(enabled)
   {
     lock();
      sndfx* snd = new sndfx(x,y,z,loop, fileName);
      snd->changeVolume(sndfxVolume);
      snd->next = sndfxList.next;
      snd->previous = &sndfxList;
      snd->next->previous = snd;
      snd->previous->next = snd;
      totalSndfx++;
     unLock();
     return(snd);
   }
   return(NULL);
}

/*************************************************************************
 *                            addSoundEffect                             *
 *************************************************************************/
sndfx* sound::addSoundEffect(bool loop, string fileName)
{
   if(enabled)
   {
    lock();
      sndfx* snd = new sndfx(loop, fileName);
      snd->changeVolume(sndfxVolume);
      snd->next = sndfxList.next;
      snd->previous = &sndfxList;
      snd->next->previous = snd;
      snd->previous->next = snd;
      totalSndfx++;
    unLock();
    return(snd);
   }
   return(NULL);
}


/*************************************************************************
 *                          removeSoundEffect                            *
 *************************************************************************/
void sound::removeSoundEffect(sndfx* snd)
{
   if(enabled)
   {
    lock();
      if( (snd) && (snd != &sndfxList))
      {
         snd->previous->next = snd->next;
         snd->next->previous = snd->previous;
         delete(snd);
         totalSndfx--;
      }
    unLock();
   }
}



/*************************************************************************
 *                              changeVolume                             *
 *************************************************************************/
void sound::changeVolume(int music, int sndV)
{
   sndfx* snd;
   
   if(enabled)
   {
    lock();
      musicVolume = music;
      sndfxVolume = sndV;
      if(backMusic)
      {
         backMusic->changeVolume(musicVolume);
      }

      snd = sndfxList.next;
      while(snd != &sndfxList)
      {
         snd->changeVolume(sndfxVolume);
         snd = snd->next;
      }

    unLock();
   }
}



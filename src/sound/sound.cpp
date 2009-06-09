/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sound.h"
#include <math.h>
#include <SDL/SDL.h>
#include "../engine/util.h"

sound* actualSound;
bool running = true;

/*************************************************************************
 *                          runParalelSound                              *
 *************************************************************************/
int runParalelSound(void* param)
{
   while(running)
   {
      actualSound->lock();
      actualSound->flush();
      actualSound->unLock();
      SDL_Delay(50);
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
}

/*************************************************************************
 *                                Init                                   *
 *************************************************************************/
void sound::init()
{
   // Initialize Open AL
   enabled = false;
   device = alcOpenDevice(NULL); 
   
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
}

/*************************************************************************
 *                               Finish                                  *
 *************************************************************************/
void sound::finish()
{
   if(enabled)
   {
      /* End the Thread */
      lock();
      running = false;
      unLock();
      SDL_WaitThread(soundThread, NULL);

      /* Clear the Opened Music */
      if(backMusic)
      {
         backMusic->release();
         delete(backMusic);
         backMusic = NULL;
      }

      /* Clear all opened Sound Effects */
      removeAllSoundEffects();

      /* Destroy the Mutex */
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
      printf("Can't Play Ogg File: %s\n",fileName.c_str());
      delete(backMusic);
      backMusic = NULL;
      return(false);
   }

   backMusic->setLoop(SOUND_AUTO_LOOP);
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
sndfx* sound::addSoundEffect(ALfloat x, ALfloat y, ALfloat z, int loop,
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
sndfx* sound::addSoundEffect(int loop, string fileName)
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
 *                        removeAllSoundEffects                          *
 *************************************************************************/
void sound::removeAllSoundEffects()
{
   /* Clear all opened Sound Effects */
   sndfx* snd = sndfxList.next;
   sndfx* tmp;
   while(totalSndfx > 0)
   {
      tmp = snd;
      snd = snd->previous;
      removeSoundEffect(tmp);
      snd= snd->next;
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

/*************************************************************************
 *                             static members                            *
 *************************************************************************/
ALCdevice* sound::device;         /**< Active AL device */
ALCcontext* sound::context;       /**< Active AL context */
ogg_stream* sound::backMusic;     /**< Active BackGround Music */

bool sound::enabled;              /**< If Sound is Enabled or Not */

SDL_Thread* sound::soundThread;   /**< The Sound Paralel Thread */
SDL_mutex* sound::soundMutex;     /**< The Sound Mutex */

sndfx sound::sndfxList;           /**< Head Node of sndFx List */
int sound::totalSndfx;            /**< Total Sound Effects on List */

int sound::musicVolume;           /**< The Music volume */
int sound::sndfxVolume;           /**< The SndFxVolume */


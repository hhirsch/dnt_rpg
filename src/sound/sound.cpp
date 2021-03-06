/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sound.h"
#include <math.h>
#include <SDL/SDL.h>
#include "../etc/quaternion.h"
#include "../engine/util.h"

#include <iostream>
using namespace std;

sound* actualSound;
bool running = true;

/*! Init and run the paralel sound */
int runParalelSound(void* param);

/*************************************************************************
 *                          runParalelSound                              *
 *************************************************************************/
int runParalelSound(void* param)
{
   if(!actualSound->initOpenAL())
   {
      return(0);
   }

   while(running)
   {
      actualSound->lock();
      actualSound->flush();
      actualSound->unLock();
      SDL_Delay(50);
   }
   actualSound->finishOpenAL();

   return(1);
}

/*************************************************************************
 *                                 lock                                  *
 *************************************************************************/
void sound::lock()
{
   if(SDL_mutexP(soundMutex) != 0)
   {
      cerr << "Error while Sound Mutex Lock" << endl;
   }
}

/*************************************************************************
 *                                unLock                                 *
 *************************************************************************/
void sound::unLock()
{
   if(SDL_mutexV(soundMutex) != 0)
   {
      cerr << "Error while Sound Mutex UnLock\n" << endl;
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
   enabled = true;
   soundMutex = SDL_CreateMutex();
      
   /* None current Opened Music */
   backMusic = NULL;

   musicVolume = 128;
   sndfxVolume = 128;

   /* Create the sound thread */
   actualSound = this;
   soundThread  = SDL_CreateThread((&runParalelSound), NULL);   
}

/*************************************************************************
 *                             initOpenAL                                *
 *************************************************************************/
bool sound::initOpenAL()
{
   /* Initialize Open AL */
   device = alcOpenDevice(NULL); 
   
   if (device != NULL) 
   {
      context=alcCreateContext(device,NULL); 
      if (context != NULL) 
      {
         alcMakeContextCurrent(context);
         enabled = true;
         
         /* set attenuation model */
         alDistanceModel(AL_EXPONENT_DISTANCE);
         return(true);
      }
      else
      {
         cerr << "Couldn't create OpenAL context" << endl;
      }

   }
   else
   {
      cerr << "No OpenAL device available!" << endl;
   }
   enabled = false;
   return(false);
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
   }

   /* Destroy the Mutex */
   SDL_DestroyMutex(soundMutex);
}

/*************************************************************************
 *                            finishOpenAL                               *
 *************************************************************************/
void sound::finishOpenAL()
{
   /* Clear the Opened Music */
   if(backMusic)
   {
      backMusic->release();
      delete(backMusic);
      backMusic = NULL;
   }

   /* Clear all opened Sound Effects */
   removeAllSoundEffects();

   /* Clear OpenAL Context and Device */
   alcDestroyContext(context);
   alcCloseDevice(device);
}

/*************************************************************************
 *                          setListenerPosition                          *
 *************************************************************************/
void sound::setListenerPosition(ALfloat centerX, ALfloat centerY, 
                                ALfloat centerZ, ALfloat theta, ALfloat phi,
                                ALfloat d, ALfloat deltaY)
{
   vec3_t pos;               /* Listener Position */
   ALfloat directionvect[6]; /* Direction Vector of Listener */
   if(enabled)
   {
      lock();
      
         pos.x = centerX + (float) d * cos(deg2Rad(theta)) * sin(deg2Rad(phi));
         pos.y = centerY + deltaY + (float) d * sin(deg2Rad(theta));
         pos.z = centerZ + (float) d * cos(deg2Rad(theta)) * cos(deg2Rad(phi));

         alListener3f(AL_POSITION, pos.x, pos.y, pos.z);

         directionvect[0] = centerX-pos.x;
         directionvect[1] = centerY-pos.y;
         directionvect[2] = centerZ-pos.z;
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
      if(backMusic->getFileName() == fileName)
      {
         /* Keep playing as the file is the same */
         unLock();
         return(true);
      }
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
      cerr << "Couldn't play ogg file: " << fileName << endl;
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
   sndfx* snd, *tmp;
   int i, total;

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
         cerr << "Error while playing music" << endl;
      }
   }

   /* Sound Effects Update */
   total = sndList.getTotal();
   snd = (sndfx*)sndList.getFirst();
   for(i=0; i < total; i++)
   {
      if(!snd->update())
      {
         /* Remove Sound */
         tmp = snd;
         snd = (sndfx*)snd->getNext();
         removeSoundEffect(tmp);
      }
      else
      {
         snd = (sndfx*)snd->getNext();
      }
   }
}

/*************************************************************************
 *                            addSoundEffect                             *
 *************************************************************************/
sndfx* sound::addSoundEffect(ALfloat x, ALfloat y, ALfloat z, int loop,
                             string fileName)
{
   sndfx* snd = NULL;

   if(enabled)
   {
      /* Create it */
      snd = new sndfx(x,y,z,loop, fileName);
      snd->changeVolume(sndfxVolume);

      /* Insert on the list */
      lock();
         sndList.insert(snd);
      unLock();
   }

   return(snd);
}

/*************************************************************************
 *                            addSoundEffect                             *
 *************************************************************************/
sndfx* sound::addSoundEffect(int loop, string fileName)
{
   sndfx* snd = NULL;

   if(enabled)
   {
      /* Create it */
      snd = new sndfx(loop, fileName);
      snd->changeVolume(sndfxVolume);

      /* Insert on the list */
      lock();
         sndList.insert(snd);
      unLock();
   }

   return(snd);
}


/*************************************************************************
 *                          removeSoundEffect                            *
 *************************************************************************/
void sound::removeSoundEffect(sndfx* snd)
{
   if( (enabled) && (snd != NULL) )
   {
      lock();
         sndList.remove(snd);
      unLock();
   }
}

/*************************************************************************
 *                        removeAllSoundEffects                          *
 *************************************************************************/
void sound::removeAllSoundEffects()
{
   /* Clear all opened Sound Effects */
   lock();
      sndList.clearList();
   unLock();
}


/*************************************************************************
 *                              changeVolume                             *
 *************************************************************************/
void sound::changeVolume(int music, int sndV)
{
   sndfx* snd;
   int i;
   
   if(enabled)
   {
      lock();
         /* Updata values */
         musicVolume = music;
         sndfxVolume = sndV;

         /* Update backmusic */
         if(backMusic)
         {
            backMusic->changeVolume(musicVolume);
         }

         /* Update all current sounds */
         snd = (sndfx*)sndList.getFirst();
         for(i=0; i < sndList.getTotal(); i++)
         {
            snd->changeVolume(sndfxVolume);
            snd = (sndfx*)snd->getNext();
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

sndfxList sound::sndList;         /**< sndFx List */

int sound::musicVolume;           /**< The Music volume */
int sound::sndfxVolume;           /**< The SndFxVolume */


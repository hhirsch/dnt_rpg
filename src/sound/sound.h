/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_sound_h
#define _dnt_sound_h

#include <SDL/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
using namespace std;

#include "ogg_stream.h"
#include "sndfx.h"

#define SOUND_NO_LOOP   -1
#define SOUND_AUTO_LOOP  0

/*! The sound Class definitions */
class sound
{
   public:
      /*! Constructor */
      sound();
      /*! Destructor */
      ~sound();

      /*! Init the sound system to use (must be called at program's init) */
      void init();

      /*! Finish the use of sound system (must be called at program's end) */
      void finish();

      /*! Define the Listener 3D Position (usually the Camera Position)
       *  \param centerX -> X position of the listener
       *  \param centerY -> Y position of the listener 
       *  \param centerZ -> Z position of the listener
       *  \param theta -> Theta Orientation Angle of the listener 
       *  \param phi -> Phi Orientation Angle of the listener 
       *  \param d -> D Param of the Listener
       *  \param deltaY -> Y Delta Variation of Listener */
      void setListenerPosition(ALfloat centerX, ALfloat centerY, 
                               ALfloat centerZ, ALfloat theta, ALfloat phi,
                               ALfloat d, ALfloat deltaY);


      /*! Flush All Buffers to the Sound Device, actualizing the played sounds
       *  and music (usually called every frame, near GLflush() )*/
      void flush();

      /*! Load and Start to Play OGG music file.
       * \param fileName -> name of the ogg file with the desired music. */
      bool loadMusic(string fileName);
   
      /*! Add sound effect to the list
       *  \param x -> X position
       *  \param y -> Y position
       *  \param z -> Z position
       *  \param loop -> sound loop interval ( < 0 won't loop) 
       *  \param fileName -> name of the ogg file to open
       *  \return pointer to the added sound */
      sndfx* addSoundEffect(ALfloat x, ALfloat y, ALfloat z, int loop,
                            string fileName);
      
      /*! Add sound effect without position to the list
       *  \param loop -> if sound will loop at end or not (see sndFx and
       *                                                   ogg_stream)
       *  \param fileName -> name of the ogg file to open
       *  \return pointer to the added sound */
      sndfx* addSoundEffect(int loop, string fileName);

      /*! Remove sound effect from list
       *  \param snd -> pointer to sound effect to remove */
      void removeSoundEffect(sndfx* snd);

      /*! Remove All sound Effects from list */
      void removeAllSoundEffects();
      
      /*! Lock the sound mutex */
      void lock();

      /*! Unlock the sound mutex */
      void unLock();

      /*! Change Overall Volume.
       *  \param music -> volume of the music
       *  \param sndV -> sound effects volume */
      void changeVolume(int music, int sndV); 

      /*! init the OpenAL device
       * \return true if successfull */
      bool initOpenAL();

      /*! finish the openAL device and related sounds */
      void finishOpenAL();

   private:

      static ALCdevice* device;         /**< Active AL device */
      static ALCcontext* context;       /**< Active AL context */
      static ogg_stream* backMusic;     /**< Active BackGround Music */

      static bool enabled;              /**< If Sound is Enabled or Not */

      static SDL_Thread* soundThread;   /**< The Sound Paralel Thread */
      static SDL_mutex* soundMutex;     /**< The Sound Mutex */

      static sndfxList sndList;         /**< Head Node of sndFx List */

      static int musicVolume;           /**< The Music volume */
      static int sndfxVolume;           /**< The SndFxVolume */
};

#endif


#ifndef _sound_h
#define _sound_h

#include <SDL/SDL.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <string>
using namespace std;

#include "ogg_stream.h"
#include "sndfx.h"

/*! The sound Class definitions */
class sound
{
   public:
      /*! Constructor */
      sound();
      /*! Destructor */
      ~sound();

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
       *  \param loop -> if sound will loop at end or not
       *  \param fileName -> name of the ogg file to open
       *  \return pointer to the added sound */
      sndfx* addSoundEffect(ALfloat x, ALfloat y, ALfloat z, bool loop,
                            string fileName);
      
      /*! Add sound effect without position to the list
       *  \param loop -> if sound will loop at end or not
       *  \param fileName -> name of the ogg file to open
       *  \return pointer to the added sound */
      sndfx* addSoundEffect(bool loop, string fileName);

      /*! Remove sound effect from list
       *  \param snd -> pointer to sound effect to remove */
      void removeSoundEffect(sndfx* snd);
      
      void lock();

      void unLock();

      /*! \todo Change Overall Volume.
       *  \param music -> volume of the music
       *  \param sndV -> sound effects volume */
      void changeVolume(int music, int sndV); 

   private:
      ALCdevice* device;            /**< Active AL device */
      ALCcontext* context;          /**< Active AL context */
      ogg_stream* backMusic;        /**< Active BackGround Music */

      SDL_Thread* soundThread;      /**< The Sound Paralel Thread */
      SDL_mutex* soundMutex;        /**< The Sound Mutex */

      sndfx sndfxList;              /**< Head Node of sndFx List */
      int totalSndfx;               /**< Total Sound Effects on List */

      int musicVolume;              /**< The Music volume */
      int sndfxVolume;              /**< The SndFxVolume */
};

#endif


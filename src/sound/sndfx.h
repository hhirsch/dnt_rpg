#ifndef _sndfx_h
#define _snd_fx_h

#include <string>
using namespace std;
#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "ogg_stream.h"

/*! Sound Effect Manipulation and Definitions */
class sndfx
{
   public:
      /*! Constructor of NULL class (for head node) */
      sndfx();

      /*! Constructor of Non positional Source 
       * \param lp -> true if is to looping the source
       * \param fileName -> name of the Ogg File to Open */
      sndfx(bool lp, string fileName);
      
      /*! Constructor of the Class.
       * \param centerX -> X position of the source
       * \param centerY -> Y position of the source
       * \param centerZ -> Z position of the source
       * \param lp -> true if is to looping the source
       * \param fileName -> name of the Ogg File to Open */
      sndfx(ALfloat centerX, ALfloat centerY, ALfloat centerZ, bool lp,
            string fileName);
      /*! Destructor */
      ~sndfx();

      /*! Redefine Position of the Source
       * \param centerX -> X position of the source
       * \param centerY -> Y position of the source
       * \param centerZ -> Z position of the source */
      void redefinePosition(ALfloat centerX, ALfloat centerY, ALfloat centerZ);

      /*! Sets if the Source will Loop at end or not
       * \param lp -> true if is to looping the source */
      void setLoop(bool lp);

      /*! Set the Velocity of the Source (if have one)
       * \param velX -> X velocity value
       * \param velY -> Y velocity value
       * \param velZ -> Z velocity value */
      void setVelocity(ALfloat velX, ALfloat velY, ALfloat velZ);

      /*! Set if the source Position is relative to the listener or not.
       *  Relative position is usually used to set a sndfx source to
       *  something that is IN the listener (like a flute, clock, etc) 
       *  \param relative -> true to set SourceRelative, false otherwise */
      void setRelative(bool relative);

      /*! Set the source to be directional.
       * \param direcX -> X direction value 
       * \param direcY -> Y direction value
       * \param direcZ -> Z direction value
       * \param innerAngle -> Cone Inner Angle
       * \param outerAngle -> Cone Outer Angle */
      void setDirectionCone(ALfloat direcX, ALfloat direcY, ALfloat direcZ,
                            ALfloat innerAngle, ALfloat outerAngle);

      /*! Update the Execution of the Source
       * \return false when execution is over */
      bool update();

      /*! Change the stream overall volume 
       * \param volume -> volume value [0 - 128]*/
      void changeVolume(int volume);


      sndfx* next;            /**< Pointer to next element on list */
      sndfx* previous;        /**< Pointer to previous element on list */

   private:
      ogg_stream* oggSndFx;   /**< The Ogg stream of the Sound */
      string oggName;         /**< File Name of the Ogg Used */
};

#endif


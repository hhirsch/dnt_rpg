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

#ifndef _dnt_sndfx_h
#define _dnt_sndfx_h

#include <string>
using namespace std;

#ifdef __APPLE__
   #include <OpenAL/al.h>
#else
   #include <AL/al.h>
#endif
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "ogg_stream.h"
#include "../etc/list.h"

/*! Sound Effect Manipulation and Definitions */
class sndfx: public dntListElement
{
   public:
      /*! Constructor of NULL class (for head node) */
      sndfx();

      /*! Constructor of Non positional Source 
       * \param lp -> loop interval (<0 won't loop, =0 loop 
       *              just after the end, >0 wait lp seconds to loop)
       * \param fileName -> name of the Ogg File to Open */
      sndfx(int lp, string fileName);
      
      /*! Constructor of the Class.
       * \param centerX -> X position of the source
       * \param centerY -> Y position of the source
       * \param centerZ -> Z position of the source
       * \param lp -> loop interval (see setLoop)
       * \param fileName -> name of the Ogg File to Open */
      sndfx(ALfloat centerX, ALfloat centerY, ALfloat centerZ, int lp,
            string fileName);
      /*! Destructor */
      ~sndfx();

      /*! Redefine Position of the Source
       * \param centerX -> X position of the source
       * \param centerY -> Y position of the source
       * \param centerZ -> Z position of the source */
      void redefinePosition(ALfloat centerX, ALfloat centerY, ALfloat centerZ);

      /*! Sets if the Source will Loop at end or not
       * \param lp -> loop interval (<0 won't loop, =0 loop 
       *              just after the end, >0 wait lp seconds to loop) */
      void setLoop(int lp);

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

   private:
      int loopInterval;       /**< The sound loop interval */
      ogg_stream* oggSndFx;   /**< The Ogg stream of the Sound */
      string oggName;         /**< File Name of the Ogg Used */
};

/*! List of sound effects */
class sndfxList: public dntList
{
   public:
      /*! Constructor */
      sndfxList();
      /*! Destructor */
      ~sndfxList();

   protected:

      /*! Free an element (deleting it)
       * \param obj -> object to free */
      void freeElement(dntListElement* obj);

};

#endif


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

#ifndef _dnt_ogg_stream_h
#define _dnt_ogg_stream_h

#include <string>
#include <iostream>
using namespace std;
 
#include <SDL/SDL.h>
#include <AL/al.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h> 

/*! The OGG Input Stream Class, based on the tutorial from devmasters. */
class ogg_stream
{
   public:
      /*! Open Ogg file to use
       * \param path -> file name of the ogg file to use
       * \return -> true if successfully opened */
      bool open(string path);
      
      /*! Release all buffers and sources relative to the file */
      void release();         
      
      /*! playback the ogg stream
       * \param rewind -> if true, rewind the ogg file
       * \return false if is not playing */
      bool playback(bool rewind=false);        
      
      /*! Verify if the source is playing
       * \return false if is not playing */
      bool playing();         
      
      /*! Update the stream
       * \return false if stream is over */
      bool update();          
      
      /*! Define the stream as Music (No position and no atenuation) */
      void defineAsMusic();

      /*! Get The Source
       * \return AL source of the ogg */
      ALuint getSource();

      /*! Change the stream overall volume 
       * \param volume -> volume value [0 - 128]*/
      void changeVolume(int volume);

      /*! Set if will Loop at EOF or not
       * \param lp -> loop interval (< 0 won't loop, =0 loop just at 
       *              the EOF, >0 wait lp seconds before loop) */
      void setLoop(int lp);

 
   protected:
      /*! Reloads a buffer
       * \param buffer -> buffer to reload 
       * \param rewind -> if true, rewind the ogg file
       * \return false if error occurs */
      bool stream(ALuint buffer, bool rewind=false);   

      /*! Empty the queue */
      void empty();      

      /*! Check OpenAl errors
       * \param where -> string with information about 
       *                 where the check occurs */
      void check(string where); 

      /*! Error code
       * \param code -> numer of error
       * \return string relative to the error */
      string errorString(int code); 
      
   private:
        FILE* oggFile;                 /**< file handle */
        OggVorbis_File oggStream;      /**< stream handle */
        vorbis_info* vorbisInfo;       /**< some formatting data */
        vorbis_comment* vorbisComment; /**< user comments */

        int loopInterval;              /**< if loop the source at EOF */ 
        Uint32 timeEnded;              /**< SDL_Ticks when EOF found */

        ALuint buffers[2]; /**< front and back buffers */
        ALuint source;     /**< audio source */
        ALenum format;     /**< internal format */
};


#endif



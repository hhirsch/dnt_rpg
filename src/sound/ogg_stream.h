#ifndef _ogg_stream_h
#define _ogg_stream_h

#include <string>
#include <iostream>
using namespace std;
 
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
       * \param path -> file name of the ogg file to use */
      void open(string path);
      
      /*! Release all buffers and sources relative to the file */
      void release();         
      
      /*! playback the ogg stream 
       * \return false if is not playing */
      bool playback();        
      
      /*! Verify if the source is playing
       * \return false if is not playing */
      bool playing();         
      
      /*! Update the stream
       * \return false if stream is over */
      bool update();          
      
      /*! Rewind the source stream */
      void rewind();          

      /*! Define the stream as Music (No position and no atenuation) */
      void defineAsMusic();

      /*! Get The Source
       * \return AL source of the ogg */
      ALuint getSource();
 
   protected:
      /*! Reloads a buffer
       * \param buffer -> buffer to reload 
       * \return false if error occurs */
      bool stream(ALuint buffer);   

      /*! Empty the queue */
      void empty();      

      /*! Check OpenAl errors */
      void check();          

      /*! Error code
       * \param code -> numer of error
       * \return string relative to the error */
      string errorString(int code); 
      
   private:
        FILE* oggFile;                 /**< file handle */
        OggVorbis_File oggStream;      /**< stream handle */
        vorbis_info* vorbisInfo;       /**< some formatting data */
        vorbis_comment* vorbisComment; /**< user comments */

        ALuint buffers[2]; /**< front and back buffers */
        ALuint source;     /**< audio source */
        ALenum format;     /**< internal format */
};


#endif



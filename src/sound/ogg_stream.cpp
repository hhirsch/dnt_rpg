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

#include "ogg_stream.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

#ifdef __APPLE__

#define BUFFER_SIZE (4096 * 64) /**< Size of the Ogg Buffer */

#else

#define BUFFER_SIZE (4096 * 16) /**< Size of the Ogg Buffer */

#endif

/*************************************************************************
 *                                 open                                  *
 *************************************************************************/
bool ogg_stream::open(string path)
{
   dirs dir;
   int result;

   timeEnded = 0;
   fileName = path;
    
   if(!(oggFile = fopen(dir.getRealFile(path).c_str(), "rb")))
   {
       cerr << "Could not open Ogg file: " <<  dir.getRealFile(path) << endl;
       return(false);
   }
   
   #if defined(_MSC_VER)
      result = ov_open_callbacks(oggFile, &oggStream, 
                                 NULL, 0, OV_CALLBACKS_DEFAULT);
   #else
      result = ov_open(oggFile, &oggStream, NULL, 0);
   #endif
 
   if(result < 0)
   {
       fclose(oggFile);
       cerr << "Could not open Ogg stream: " << errorString(result) << endl;
       return(false);
   }

   vorbisInfo = ov_info(&oggStream, -1);
   vorbisComment = ov_comment(&oggStream, -1);
 
   if(vorbisInfo->channels == 1)
   {
       format = AL_FORMAT_MONO16;
   }
   else
   {
       format = AL_FORMAT_STEREO16;
   }

   loopInterval = -1;

   alGenBuffers(2, buffers);
   check("::open() -> alGenBuffers");
   alGenSources(1, &source);
   check("::open() -> alGenSouces");

   return(true);
}

/*************************************************************************
 *                             defineAsMusic                             *
 *************************************************************************/
void ogg_stream::defineAsMusic()
{
   alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
   alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
   alSource3f(source, AL_DIRECTION, 0.0, 0.0, 0.0);
   alSourcef(source, AL_ROLLOFF_FACTOR, 0.0);
   alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
}

/*************************************************************************
 *                              getSource                                *
 *************************************************************************/
ALuint ogg_stream::getSource()
{
   return(source);
}

/*************************************************************************
 *                                release                                *
 *************************************************************************/
void ogg_stream::release()
{
   if(vorbisInfo)
   {
      /* Stop Source (if already not stoped) */
      if(timeEnded == 0)
      {
         alSourceStop(source);
         check("::release() alSourceStop");
      }

      /* Empty the remaining buffers */
      empty();

      /* Delete Sources And Buffers */
      alDeleteSources(1, &source);
      check("::release() alDeleteSources");
      alDeleteBuffers(2, &buffers[0]);
      check("::release() alDeleteBuffers");

      /* Finally, clear the ogg stream */
      ov_clear(&oggStream);
      vorbisInfo = NULL;
   }
}

/*************************************************************************
 *                              playBack                                 *
 *************************************************************************/
bool ogg_stream::playback(bool rewind)
{
   if(playing())
   {
      return(true);
   }
        
   if(!stream(buffers[0], rewind))
   {
      return(false);
   }
 
   if(!stream(buffers[1]))
   {
      if(!rewind)
      {
         /* File must be too small for double buffer.
          * Play as a single buffer. */
         alSourceQueueBuffers(source, 1, buffers);
         alSourcePlay(source);
         return(true);
      }
      /* With rewind enabled and couldn't read second buffer? Fail! */
      return(false);
   }
    
   alSourceQueueBuffers(source, 2, buffers);
   alSourcePlay(source);
    
   return(true);
}

/*************************************************************************
 *                               playing                                 *
 *************************************************************************/
bool ogg_stream::playing()
{
   ALenum state;

   alGetSourcei(source, AL_SOURCE_STATE, &state);

   return (state == AL_PLAYING);
}

/*************************************************************************
 *                               update                                  *
 *************************************************************************/
bool ogg_stream::update()
{
    int processed;
    bool active = true;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    /* Must verify if sound is pending for loop */
    if( (processed == 0) && (timeEnded > 0) )
    {
       /* Verify Waiting to loop again time */
       if((loopInterval > 0) && (timeEnded != 0))
       {
          int interval = (SDL_GetTicks() - timeEnded) / 1000;
          if(interval >= loopInterval)
          {
             /* Reinit the play, rewinding the file */
             active = playback(true);
          }
       }
    }

    /* Process the processed buffers */
    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
        check("::update() alSourceUnqueueBuffers");

        /* Only re-stream with ogg_stream is active (sometimes the previous
         * buffer inactive the stream, so must end to avoid uneeded replay) */
        if(active)
        {
           active = stream(buffer);

           if( (active) && (timeEnded == 0))
           {
              /* Only Queue if stream is active, and not waiting */
              alSourceQueueBuffers(source, 1, &buffer);
              check("::update() alSourceQueueBuffers");
           }
        }
    }
 
    return(active);
}

/*************************************************************************
 *                                stream                                 *
 *************************************************************************/
bool ogg_stream::stream(ALuint buffer, bool rewind)
{
   char data[BUFFER_SIZE] = { 0 };
   int  size = 0;
   int  section;
   long  result = -1;
   int bufferSize = BUFFER_SIZE;
   
#if (defined(__APPLE__) && (SDL_BYTEORDER != SDL_BIG_ENDIAN))
   /* ogg_ov_read seems to be buggy on Intel Macs... so, doing by hand */
   float** channels;
   int i,c,j;
   int val;
   
   if(format == AL_FORMAT_MONO8)
   {
      bufferSize = BUFFER_SIZE / 2;
   }
   else if(format == AL_FORMAT_STEREO16)
   {
      bufferSize = BUFFER_SIZE / 4;
   }
#endif
   
   if(rewind)
   {
      /* Must restart the stream */
      timeEnded = 0;
      /* Rewind the file */
      if(ov_raw_seek(&oggStream,0) != 0)
      {
         cerr << "Ogg Rewind Error" << endl;
      }
   }
   else if(timeEnded > 0)
   {
      /* Must only wait */
      return(true);
   }
   
   while( (size < bufferSize) && (result != 0))
   {
#if (defined(__APPLE__) && (SDL_BYTEORDER != SDL_BIG_ENDIAN))
      /* Get as ov_read_float, as ov_read is buggy on Intel OSX */
      result = ov_read_float(&oggStream, &channels, bufferSize-size, &section);
      /* Convert float values to the buffer... ugly and unefficient! */
      if(result > 0)
      {

         if(format == AL_FORMAT_MONO16)
         {
            j=size;
            for(i=0; i<result; i++)
            {
               val = channels[0][i]*32767;
               
               if(val > 32767) val = 32767;
               else if(val < -32767) val = -32767;
               
               data[j] = (val&0xff);
               data[j+1] = (val>>8);
               j += 2;
            }
            result *= 2;
         }
         else if(format == AL_FORMAT_STEREO16)
         {
            j = size;
            for(i=0; i<result; i++)
            {
               for(c=0; c < 2; c++)
               {
                  val = channels[c][i]*32768;
                  
                  if(val > 32767) val = 32767;
                  else if(val < -32767) val = -32767;
                  
                  data[j] = (val&0xff);
                  data[j+1] = (val>>8);
                  j += 2;
               }
            }
            result *= 4;
         }
      }
#else
      result = ov_read(&oggStream,data+size,bufferSize-size,
                       (SDL_BYTEORDER == SDL_BIG_ENDIAN),2,1,&section);
#endif
      
      if(result > 0)
      {
         size += result;
      }
      else if(result < 0)
      {
         cerr << "Ogg Buffer Error: " << errorString((int)result) << endl;
         return(false);
      }
      else
      {
         if(loopInterval == 0)
         {
            /* rewind file */
            result = 1;
            if(ov_raw_seek(&oggStream,0) != 0)
            {
               cerr << "Ogg Rewind Error" << endl;
            }
         }
         else if(loopInterval > 0)
         {
            timeEnded = SDL_GetTicks();
         }
      }
   }
   
   if(size == 0)
   {
      /*empty();*/
      alSourceStop(source);
      
      /* Only return false if will no more play */
      return(timeEnded > 0);
   }
   else
   {
      alBufferData(buffer, format, data, size, (ALsizei)vorbisInfo->rate);
      check("::stream() alBufferData");
   }
   
   return(true);
}

/*************************************************************************
 *                                 empty                                 *
 *************************************************************************/
void ogg_stream::empty()
{
    int queued;

    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    check("::empty() AL_BUFFERS_QUEUED");

    while(queued--)
    {
       ALuint buffer;

       alSourceUnqueueBuffers(source, 1, &buffer);
       check("::empty() alSourceUnqueueBuffers");
    }
}

/*************************************************************************
 *                                 check                                 *
 *************************************************************************/
void ogg_stream::check(string where)
{
    int error = alGetError();
 
    if(error != AL_NO_ERROR)
    {
        cerr << "OpenAL error was raised: " << error << endl;
        switch(error)
        {
           case AL_INVALID_NAME: 
              cerr << "Invalid name parameter";
           break;
           case AL_INVALID_ENUM: 
              cerr << "Invalid parameter";
           break;
           case AL_INVALID_VALUE: 
              cerr << "Invalid enum parameter value";
           break;
           case AL_INVALID_OPERATION: 
              cerr << "Illegal call";
           break;
           case AL_OUT_OF_MEMORY: 
              cerr << "Unable to allocate memory";
           break;
        }
        cerr << " at " << where << endl;
    }
}

/*************************************************************************
 *                                errorString                            *
 *************************************************************************/
string ogg_stream::errorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return string("Read from media.");
        case OV_ENOTVORBIS:
            return string("Not Vorbis data.");
        case OV_EVERSION:
            return string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return string("Invalid Vorbis header.");
        case OV_EFAULT:
            return string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return string("Unknown Ogg error.");
    }
}

/*************************************************************************
 *                            changeVolume                               *
 *************************************************************************/
void ogg_stream::changeVolume(int volume)
{
   alSourcef(source, AL_GAIN, (float)(volume / 128.0));
}

/*************************************************************************
 *                               setLoop                                 *
 *************************************************************************/
void ogg_stream::setLoop(int lp)
{
   loopInterval = lp;
}


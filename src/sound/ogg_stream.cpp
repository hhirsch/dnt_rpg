#include "ogg_stream.h"

#define BUFFER_SIZE (4096 * 8) /**< Size of the Ogg Buffer */

/*************************************************************************
 *                                 open                                  *
 *************************************************************************/
void ogg_stream::open(string path)
{
   int result;
    
   if(!(oggFile = fopen(path.c_str(), "rb")))
   {
       printf("Could not open Ogg file: %s\n",path.c_str());
       return;
   }
 
   if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
   {
       fclose(oggFile);
       printf("Could not open Ogg stream: %s\n", errorString(result).c_str());
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

   alGenBuffers(2, buffers);
   check();
   alGenSources(1, &source);
   check();

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
    alSourceStop(source);
    empty();
    alDeleteSources(1, &source);
    check();
    alDeleteBuffers(2, &buffers[0]);
    check();
 
    ov_clear(&oggStream);
}

/*************************************************************************
 *                              playBack                                 *
 *************************************************************************/
bool ogg_stream::playback()
{
   if(playing())
   {
      return(true);
   }
        
   if(!stream(buffers[0]))
   {
      return(false);
   }
 
   if(!stream(buffers[1]))
   {
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

    if(!playing())
    {
       //If not playing, resume the play!
       alSourcePlay(source);
    }

    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
 
        active = stream(buffer);
 
        alSourceQueueBuffers(source, 1, &buffer);
        check();
    }
 
    return active;
}

/*************************************************************************
 *                                rewind                                 *
 *************************************************************************/
void ogg_stream::rewind()
{
   int result = ov_raw_seek(&oggStream,0);
   if( result != 0)
   {
      printf("Ogg Rewind Error: %d\n",result);
   }
}

/*************************************************************************
 *                                stream                                 *
 *************************************************************************/
bool ogg_stream::stream(ALuint buffer)
{
    char data[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result = -1;
 
    while( (size < BUFFER_SIZE) && (result != 0))
    {
        result = ov_read(&oggStream,data+size,BUFFER_SIZE-size,0,2,1,&section);
    
        if(result > 0)
        {
           size += result;
        }
        else if(result < 0)
        {
           printf("Ogg Buffer Error: %s\n", errorString(result).c_str());
           return(false);
        }
    }
    
    if(size == 0)
    {
       return(false);
    }
 
    if(result != 0)
    {
       /*printf("bufferdata: Buffer: %d Format: %d size: %d\n", buffer, 
                                                 format, size);*/
       alBufferData(buffer, format, data, size, vorbisInfo->rate);
       check();
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
    
    while(queued--)
    {
        ALuint buffer;
    
        alSourceUnqueueBuffers(source, 1, &buffer);
        check();
    }
}

/*************************************************************************
 *                                 check                                 *
 *************************************************************************/
void ogg_stream::check()
{
    /*int error = alGetError();
 
    if(error != AL_NO_ERROR)
    {
        printf("OpenAL error was raised: %d .\n", error);
        switch(error)
        {
           case AL_INVALID_NAME: printf("Invalid name parameter\n");
           break;
           case AL_INVALID_ENUM: printf("Invalid parameter\n");
           break;
           case AL_INVALID_VALUE: printf("Invalid enum parameter value\n");
           break;
           case AL_INVALID_OPERATION: printf("Illegal call\n");
           break;
           case AL_OUT_OF_MEMORY: printf("Unable to allocate memory\n");
           break;
        }
    }*/
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


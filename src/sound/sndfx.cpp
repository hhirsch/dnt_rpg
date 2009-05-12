#include "sndfx.h"
#include "../etc/dirs.h"

/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sndfx::sndfx()
{
   oggSndFx = NULL;
   next = NULL;
   previous = NULL;
}

/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sndfx::sndfx(ALfloat centerX, ALfloat centerY, ALfloat centerZ, int lp,
             string fileName)
{
   /* Create the Ogg Stream */ 
   oggSndFx = new(ogg_stream);
   if(oggSndFx->open(fileName))
   {

      /* Define Position */
      alSource3f(oggSndFx->getSource(), AL_POSITION, centerX, centerY, centerZ);
      alSourcef(oggSndFx->getSource(), AL_REFERENCE_DISTANCE, 160);

      if(!oggSndFx->playback())
      {
         printf("Can't Play Sound Effect: %s\n",fileName.c_str());
      }

      oggName = fileName;
      setLoop(lp);
   }
   else
   {
      delete(oggSndFx);
      oggSndFx = NULL;
   }
   next = NULL;
   previous = NULL;
}

/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sndfx::sndfx(int lp, string fileName)
{
   /* Create the Ogg Stream */ 
   oggSndFx = new(ogg_stream);
   
   if(oggSndFx->open(fileName))
   {
      /* Define Position */
      oggSndFx->defineAsMusic();

      if(!oggSndFx->playback())
      {
         printf("Can't Play Sound Effect: %s\n",fileName.c_str());
      }

      oggName = fileName;
      setLoop(lp);
   }
   else
   {
      delete(oggSndFx);
      oggSndFx = NULL;
   }
   next = NULL;
   previous = NULL;
}

/*************************************************************************
 *                              Destructor                               *
 *************************************************************************/
sndfx::~sndfx()
{
   if(oggSndFx)
   {
      oggSndFx->release();
      delete(oggSndFx);
   }
}

/*************************************************************************
 *                                setLoop                                *
 *************************************************************************/
void sndfx::setLoop(int lp)
{
   if(oggSndFx)
   {
      oggSndFx->setLoop(lp);
   }
}

/*************************************************************************
 *                             redefinePosition                          *
 *************************************************************************/
void sndfx::redefinePosition(ALfloat centerX, ALfloat centerY, ALfloat centerZ)
{
   if(oggSndFx != NULL)
   {
      alSource3f(oggSndFx->getSource(), AL_POSITION, centerX, centerY, centerZ);
   }
}

/*************************************************************************
 *                             setVelocity                               *
 *************************************************************************/
void sndfx::setVelocity(ALfloat velX, ALfloat velY, ALfloat velZ)
{
   if(oggSndFx != NULL)
   {
      alSource3f(oggSndFx->getSource(), AL_VELOCITY, velX, velY, velZ);
   }
}

/*************************************************************************
 *                             setRelative                               *
 *************************************************************************/
void sndfx::setRelative(bool relative)
{
   if(oggSndFx != NULL)
   {
      if(relative)
      {
         alSourcei(oggSndFx->getSource(), AL_SOURCE_RELATIVE, AL_TRUE);
      }
      else
      {
         alSourcei(oggSndFx->getSource(), AL_SOURCE_RELATIVE, AL_FALSE);
      }
   }
}

/*************************************************************************
 *                          setDirectionCone                             *
 *************************************************************************/
void sndfx::setDirectionCone(ALfloat direcX, ALfloat direcY, ALfloat direcZ,
                             ALfloat innerAngle, ALfloat outerAngle)
{
   if(oggSndFx != NULL)
   {
      alSource3f(oggSndFx->getSource(), AL_DIRECTION, direcX, direcY, direcZ);
      alSourcef(oggSndFx->getSource(), AL_CONE_INNER_ANGLE, innerAngle);
      alSourcef(oggSndFx->getSource(), AL_CONE_OUTER_ANGLE, outerAngle);
   }
}

/*************************************************************************
 *                               update                                  *
 *************************************************************************/
bool sndfx::update()
{
   if(oggSndFx != NULL)
   {
      if(oggSndFx->update()) 
      {
         return(true);
      }
      else 
      {
         return(false);
      }
   }
   return(false);
}

/*************************************************************************
 *                            changeVolume                               *
 *************************************************************************/
void sndfx::changeVolume(int volume)
{
   if(oggSndFx)
   {
      oggSndFx->changeVolume(volume);
   }
}



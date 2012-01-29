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

#include "sndfx.h"
#include "../etc/dirs.h"

#include <iostream>
using namespace std;

/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sndfx::sndfx()
{
   oggSndFx = NULL;
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
      setLoop(lp);

      if(!oggSndFx->playback())
      {
         cerr << "Couldn't play sound effect: " << fileName << endl;
      }

      oggName = fileName;
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
      setLoop(lp);

      if(!oggSndFx->playback())
      {
         cerr << "Couldn't play sound effect: " << fileName << endl;
      }

      oggName = fileName;
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


/*************************************************************************
 *                             Constructor                               *
 *************************************************************************/
sndfxList::sndfxList():dntList()
{
}

/*************************************************************************
 *                              Destructor                               *
 *************************************************************************/
sndfxList::~sndfxList()
{
   clearList();
}

/*************************************************************************
 *                             freeElement                               *
 *************************************************************************/
void sndfxList::freeElement(dntListElement* obj)
{
   sndfx* snd = (sndfx*)obj;
   delete(snd);
}


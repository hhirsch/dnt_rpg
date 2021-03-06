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

#include "door.h"
#include "../sound/sound.h"

/********************************************************************
 *                            Constructor                           *
 ********************************************************************/
door::door()
{
   status = DOOR_STATUS_CLOSED;
   delta = 0.0f;
}

/********************************************************************
 *                             Destructor                           *
 ********************************************************************/
door::~door()
{
}

/********************************************************************
 *                            update()                              *
 ********************************************************************/
void door::update()
{
   /* Do the "animation" */
   float oY = obj->scNode->getAngleY();
   oY -= delta;

   if( (status == DOOR_STATUS_OPENED) && (delta < 90))
   {
      delta += 3;
   }
   else if( (status == DOOR_STATUS_CLOSED) && (delta > 0) )
   {
      delta -= 6;
   }
   obj->scNode->setAngle(0.0f, oY+delta, 0.0f);
}

/********************************************************************
 *                            door flip                             *
 ********************************************************************/
void door::flip()
{
   sound snd;
   if(status == DOOR_STATUS_CLOSED)
   {
      /* Will open */
      status = DOOR_STATUS_OPENED;
      /* Call opening sound */
      snd.addSoundEffect(obj->scNode->getPosX(), 0.0, 
            obj->scNode->getPosZ(), SOUND_NO_LOOP,
            "sndfx/objects/door_open.ogg");
   }
   else
   {
      /* Will close */
      status = DOOR_STATUS_CLOSED;
      /* Call closing sound */
      snd.addSoundEffect(obj->scNode->getPosX(), 0.0,
            obj->scNode->getPosZ(), SOUND_NO_LOOP,
            "sndfx/objects/door_close.ogg");
   }
}


/********************************************************************
 *                            Constructor                           *
 ********************************************************************/
doorList::doorList()
{
}

/********************************************************************
 *                             Destructor                           *
 ********************************************************************/
doorList::~doorList()
{
   clearList();
}

/********************************************************************
 *                            freeElement                           *
 ********************************************************************/
void doorList::freeElement(dntListElement* obj)
{
   door* d = (door*)obj;
   delete(d);
}


/********************************************************************
 *                            update()                              *
 ********************************************************************/
void doorList::update()
{
   int i;
   door* d = (door*)first;

   for(i=0; i < total; i++)
   {
      d->update();
      d = (door*)d->getNext();
   }
}


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

#include "mapsound.h"
#include "../etc/defparser.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
mapSound::mapSound()
{
   sounds = NULL;
   totalSounds = 0;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
mapSound::~mapSound()
{
   int i;
   soundInfo* aux;

   /* Delete all soundInfos on the list */
   for(i = 0; i < totalSounds; i++)
   {
      aux = sounds;
      sounds = sounds->next;
      delete(aux);
   }
}

/***********************************************************************
 *                               newSound                              *
 ***********************************************************************/
soundInfo* mapSound::newSound()
{
   /* Create the structure */
   soundInfo* snd = new soundInfo();

   /* Nullify it */
   snd->fileName = "";
   snd->x = 0;
   snd->y = 0;
   snd->z = 0;
   snd->loopInterval = 0;

   /* Add to the list */
   addSound(snd);

   return(snd);
}

/***********************************************************************
 *                               addSound                              *
 ***********************************************************************/
void mapSound::addSound(soundInfo* snd)
{
   if(sounds == NULL)
   {
      /* The only on the list */
      snd->next = snd;
      snd->previous = snd;
   }
   else
   {
      /* set it pointers */
      snd->next = sounds;
      snd->previous = sounds->previous;
      snd->next->previous = snd;
      snd->previous->next = snd;
   }

   /* Put at the first */
   sounds = snd;
   totalSounds++;
}

/***********************************************************************
 *                                 load                                *
 ***********************************************************************/
bool mapSound::load(string fileName)
{
   defParser def;
   string key, value;
   soundInfo* snd = NULL;

   if(def.load(fileName))
   {
      while(def.getNextTuple(key, value))
      {
         if(key == "sound")
         {
            /* Define a new sound */
            snd = newSound();
            snd->fileName = value;
         }
         else if(snd != NULL)
         {
            if(key == "posX")
            {
               sscanf(value.c_str(),"%f", &snd->x);
            }
            else if(key == "posY")
            {
               sscanf(value.c_str(),"%f", &snd->y);
            }
            else if(key == "posZ")
            {
               sscanf(value.c_str(),"%f", &snd->z);
            }
            else if(key == "loopInterval")
            {
               sscanf(value.c_str(),"%d", &snd->loopInterval);
            }
         }
         else
         {
            cerr << "Trying to define: " << key << " before sound at "
                 << fileName << endl;
         }
      }
      return(true);
   }

   return(false);
}

/***********************************************************************
 *                                 save                                *
 ***********************************************************************/
bool mapSound::save(string fileName)
{
   int i;
   ofstream file;

   /* Open the file to save */
   file.open(fileName.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error Opening File: " << fileName << endl;
      return(false);
   }

   /* Save all sounds to the file */
   soundInfo* s = sounds;
   for(i = 0; i < totalSounds; i++)
   {
      /* Save sound info */
      file << "sound = " << s->fileName << endl;
      file << "posX = " << s->x << endl;
      file << "posY = " << s->y << endl;
      file << "posZ = " << s->z << endl;
      file << "loopInterval = " << s->loopInterval << endl;

      /* Next sound */
      s = s->next;
   }

   return(true);
}

/***********************************************************************
 *                                 flush                               *
 ***********************************************************************/
void mapSound::flush()
{
   sound snd;
   int i;

   /* Add all sounds to the sound controller */
   soundInfo* s = sounds;
   for(i = 0; i < totalSounds; i++)
   {
      snd.addSoundEffect(s->x, s->y, s->z, s->loopInterval, s->fileName);
      s = s->next;
   }
}


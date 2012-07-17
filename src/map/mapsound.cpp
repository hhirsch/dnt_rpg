/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "mapsound.h"
#include "../etc/defparser.h"
using namespace std;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
mapSound::mapSound():dntList()
{
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
mapSound::~mapSound()
{
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
   insert(snd);

   return(snd);
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
            else if(key == "rollOff")
            {
               sscanf(value.c_str(),"%f", &snd->rollOff);
            }
            else if(key == "refDistance")
            {
               sscanf(value.c_str(),"%f", &snd->refDistance);
            }
            else if(key == "volume")
            {
               sscanf(value.c_str(),"%d", &snd->volume);
            }
            else if(key == "loopInterval")
            {
               sscanf(value.c_str(),"%d", &snd->loopInterval);
            }
            else
            {
               std::cerr << "Unknown key '" << key << "' "
                  << "at sound definitions file " << fileName << std::endl; 
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
   soundInfo* s = (soundInfo*)first;
   for(i = 0; i < total; i++)
   {
      /* Save sound info */
      file << "sound = " << s->fileName << endl;
      file << "posX = " << s->x << endl;
      file << "posY = " << s->y << endl;
      file << "posZ = " << s->z << endl;
      file << "rollOff = " << s->rollOff << endl;
      file << "refDistance = " << s->refDistance << endl;
      file << "volume = " << s->volume << endl;
      file << "loopInterval = " << s->loopInterval << endl;

      /* Next sound */
      s = (soundInfo*)s->getNext();
   }

   return(true);
}

/***********************************************************************
 *                            freeElement                              *
 ***********************************************************************/
void mapSound::freeElement(dntListElement* obj)
{
   soundInfo* s = (soundInfo*)obj;
   delete(s);
}

/***********************************************************************
 *                                 flush                               *
 ***********************************************************************/
void mapSound::flush()
{
   sound snd;
   sndfx* sfx;
   int i;

   /* Add all sounds to the sound controller */
   soundInfo* s = (soundInfo*)first;
   for(i = 0; i < total; i++)
   {
      sfx = snd.addSoundEffect(s->x, s->y, s->z, s->loopInterval, s->fileName);
      sfx->changeVolume(s->volume);
      sfx->setReferenceDistance(s->refDistance);
      sfx->setRollOff(s->rollOff);
      s = (soundInfo*)s->getNext();
   }
}

/***********************************************************************
 *                             getSound                                *
 ***********************************************************************/
soundInfo* mapSound::getSound(float x, float y, float z)
{
   sound snd;
   int i;

   soundInfo* s = (soundInfo*)first;
   for(i = 0; i < total; i++)
   {
      if( (s->x == x) && 
          (s->y == y) &&
          (s->z == z) )
      {
         return(s);
      }
      s = (soundInfo*)s->getNext();
   }
   return(NULL);
}


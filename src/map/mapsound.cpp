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
            snd = new soundInfo();
            snd->fileName = value;
            snd->x = 0;
            snd->y = 0;
            snd->z = 0;
            snd->loopInterval = 0;
            /* Add it to the list */
            addSound(snd);
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


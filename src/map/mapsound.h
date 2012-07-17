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

#ifndef _dnt_map_sound_h
#define _dnt_map_sound_h

#include "../etc/list.h"
#include "../sound/sound.h"

/*! The soundInfo class keeps informations about a sound on a map */
class soundInfo: public dntListElement
{
   public:
      std::string fileName;   /**< Ogg File Name */
      ALfloat x,         /**< Sound X Position */
              y,         /**< Sound Y Position */
              z;         /**< Sound Z Position */
      int loopInterval;  /**< Sound Loop Interval */
};

/*! The mapSound class contains information of all sounds related to the 
 * current map (position, ogg file, etc.) */
class mapSound: public dntList
{
   public:
      /*! Constructor */
      mapSound();

      /*! Destructor */
      ~mapSound();

      /*! Load mapSound definition file
       * \param fileName -> file name of the definition to load
       * \return -> true if load was successfull, falst otherwise */
      bool load(std::string fileName);

      /*! Save mapSound definition file with the current sound list
       * \param fileName -> name of the file to save
       * \return -> true if save was successfull */
      bool save(std::string fileName);

      /*! Flush Sounds, adding it to the OpenAL sound controller */
      void flush();

      /*! Add a new undefined sound to the definition.
       * \note -> used only at mapEditor
       * \return -> pointer to the added soundInfo structure */
      soundInfo* newSound();

      /*! Get sound defined at the position x,y,z 
       * \note -> used only at mapEditor
       * \return -> pointer to the sound, if any, or NULL */
      soundInfo* getSound(float x, float y, float z);

   protected:
      /*! Free a sound (deleting it)
       * \param obj -> sound to delete */
      void freeElement(dntListElement* obj);
};

#endif


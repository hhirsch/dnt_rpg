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

#ifndef _dnt_modeffect_h
#define _dnt_modeffect_h

#include <SDL/SDL.h>
#include "../etc/list.h"

#include "modifier.h"

/*! Status modifier effect */
class modEffect: public dntListElement
{
   public:
      int mod;             /**< The modifier value */
      factor cause;        /**< Thing to modify */
      Uint16 init;         /**< Time the effect begins  */
      int time;            /**< Max Time the effect remains (0 for infinity) */
      int periodicTime;    /**< Periodicity of the effect (with saves), 
                                0 for non-periodic ones. */
};

/*! List of modEffects */
class modEffectList: public dntList
{
   public:
      /*! Constructor */
      modEffectList();

      /*! Destructor */
      ~modEffectList();

      /*! Set the character owner of the modEffect list
       * \param actor -> pointer to the character owner of the list */
      void setOwner(void* actor);

   protected:
      /*! Free modEffect used memory
       * \param obj -> modEffect to delete */
      void freeElement(dntListElement* obj);

      void* owner;       /**< Pointer to the character owner of the list */
};


#endif


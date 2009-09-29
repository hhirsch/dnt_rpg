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

      /*! Constructor
       * \param mod -> modifier value
       * \param time -> time to expire (0 to not expire)
       * \param periodicTime -> time to apply the modEffect again 
       *                        (0 to not apply)
       * \param factorId -> id of the target factor (see: factor)
       * \param factorType -> type of the target factor (see: factor) */
      modEffect(int mod, int time, int periodicTime, 
            string factorId, string factorType);
      /*! Constructor 
       * \param obj -> modEffect to use as base */
      modEffect(modEffect* obj);
      /*! Destructor */
      ~modEffect();

      /*! Apply the modEffect to the character
       * \param actor -> pointer to the character to apply the modEffect */
      void apply(void* actor);

      /*! Unapply the modEffect to the character
       * \param actor -> pointer to the character to deapply the modEffect */
      void unApply(void* actor);

      friend class modEffectList;

   protected:

      int mod;             /**< The modifier value */
      factor cause;        /**< Thing to modify */
      Uint16 init;         /**< Time the effect begins  */      
      int time;            /**< Max Time the effect remains (0 for infinity) */
      int periodicTime;    /**< Periodicity of the effect (with saves), 
                                0 for non-periodic ones. */
      Uint16 lastApply;    /**< Time when last applied the modEffect */
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

      /*! Insert the modEffect to the list
       * \param obj -> modEffect to insert
       * \return true on success */
      bool insert(modEffect* obj);

      /*! Insert a copy of the modEffect on the list 
       * (applying it to the owner character)
       * \param obj -> pointer to modEffect to insert a copy
       * \return true on success */
      bool insertCopy(modEffect* obj);

      /*! Remove all expired modEffects from the list (unappling it to 
       * the owner character) and apply all periodic modEffects when needed */
      void doStep();

   protected:
      /*! Free modEffect used memory
       * \param obj -> modEffect to delete */
      void freeElement(dntListElement* obj);

      void* owner;       /**< Pointer to the character owner of the list */
};


#endif


/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

class character;

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
                std::string factorId, std::string factorType);
      /*! Constructor 
       * \param obj -> modEffect to use as base */
      modEffect(modEffect* obj);
      /*! Constructor
       * \param saveTxt -> saveText with modEffect info to load from */
      modEffect(std::string saveTxt);
      /*! Destructor */
      ~modEffect();

      /*! Apply the modEffect to the character
       * \param actor -> pointer to the character to apply the modEffect */
      void apply(character* actor);

      /*! Unapply the modEffect to the character
       * \param actor -> pointer to the character to deapply the modEffect */
      void unApply(character* actor);

      /*! Get a string describing the modEffect
       * \param actor -> pointer to the character "owner" of the effect
       * \return -> representative string of the modEffect */
      std::string toReadableText(character* actor);

      /*! Get a string to save the modEffect
       * \return -> string with modEffect Info */
      std::string toSaveText();

      /*! Load the modEffect from the save string
       * \param txt -> saveString with modEffect info */
      void fromSaveText(std::string txt);

      /*! Get modification value */
      int getValue();

      friend class modEffectList;

   protected:
      /*! Do the apply on actor, with predefined value
       * \param actor -> character to apply the modEffect to
       * \param value -> value to use as modifier (instead of current mod) */
      void doApply(character* actor, int value);

      int mod;             /**< The modifier value */
      factor cause;        /**< Thing to modify */
      Uint16 init;         /**< Time the effect begins  */      
      int time;            /**< Max Time the effect remains (0 for infinity) */
      int periodicTime;    /**< Periodicity of the effect (with saves), 
                                0 for non-periodic ones. */
      Uint16 lastApply;    /**< Time when last applied the modEffect */
      bool applied;        /**< If applied or not */
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
      void setOwner(character* actor);

      /*! Insert the modEffect to the list
       * \param obj -> modEffect to insert
       * \param applyEffect -> true to apply the effect to character
       * \return true on success */
      bool insert(modEffect* obj, bool applyEffect);

      /*! Insert a copy of the modEffect on the list 
       * (applying it to the owner character)
       * \param obj -> pointer to modEffect to insert a copy
       * \param applyEffect -> true to apply the effect to character
       * \return true on success */
      bool insertCopy(modEffect* obj, bool applyEffect);

      /*! Remove all expired modEffects from the list (unappling it to 
       * the owner character) and apply all periodic modEffects when needed */
      void doStep();

      /*! Do the rest to the modEffects list (removing all timed effects) */
      void rest();

      /*! Unapply all effects (but do not remove them from list) */
      void unApply();

      /*! Apply all effects at the list */
      void apply();

   protected:
      /*! Free modEffect used memory
       * \param obj -> modEffect to delete */
      void freeElement(dntListElement* obj);

      character* owner;    /**< Pointer to the character owner of the list */
};


#endif


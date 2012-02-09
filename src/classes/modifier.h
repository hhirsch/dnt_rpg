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

#ifndef _dnt_modifier_h
#define _dnt_modifier_h

#include <string>

#include "defs.h"

#define SIGNAL_DEC 0
#define SIGNAL_SUM 1

#define MOD_TYPE_ATT   "ATTRIBUTE"
#define MOD_TYPE_SKILL "SKILL"
#define MOD_TYPE_THING "THING"

#define MOD_TYPE_CLASS "CLASS"
#define MOD_TYPE_RACE  "RACE"
#define MOD_TYPE_ALIGN "ALIGN"

/**! Actuation Factor */
class factor
{
   public:
      /* Default Constructor */
      factor(){type = "ALL"; id = "ALL";};

      std::string type; /**< Type of the Factor (ex: Skill, Feat, Class) */
      std::string id;   /**< ID of the Factor (ex: Intimidation, etc) */
};

/*! Default Modifier */
class modifier
{
   public:
      int mod;            /**< Value of the Modifier */
      factor cause;       /**< Cause of the Modifier */
      factor target;      /**< Target when modifer is valid */
      std::string description; /**< Modifier Description */
};

/*! Number of Points, based on ATTRIBUTES 
 * the formula is (sum SIGNAL ATTModifier)*mult */
class points
{
   public:
      int sum;          /**< Value of Sum */
      int signal;       /**< Signal Used (- or +) */
      std::string attID;/**< ID of the Attribute to use modifier */
      int mult;         /**< Value of Mult */
};


#endif


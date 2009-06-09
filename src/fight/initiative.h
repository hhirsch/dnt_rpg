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

#ifndef _dnt_initiative_h
#define _dnt_initiative_h

#include <stdlib.h>
#include <string>
using namespace std;
#include "../engine/character.h"
#include "../engine/briefing.h"
#include <libintl.h>

/*! Implementation of the Struct of Initiative */
class initiativeStruct
{
   public:
      /*! Constructor */
      initiativeStruct(){};
      /*! Destructor */
      ~initiativeStruct(){};
 
      character* dude;         /**< Character Pointer */
      initiativeStruct* next;  /**< Next Character on Initiative Pointer */
      int initiativeValue;     /**< Initiative Value of the Character */
};

/*! Initiatives Implementation */
class initiative
{
   public:
     /*! Constructor */
     initiative();
     /*! Destructor */
     ~initiative();

     /*!
     *************************************************************** 
     * Insert Character on Initiative.  
     * \param pers -> Pointer to Character
     ***************************************************************/
     void insertCharacter(character* pers);
     /*!
     *************************************************************** 
     * Actualize the pointers to a new Round.  
     ***************************************************************/
     void newRound();
     /*!
     *************************************************************** 
     * Gets the next Character to Play.  
     * \return pointer to next character to play.
     ***************************************************************/
     character* nextCharacter();
     /*!
     *************************************************************** 
     * Gets the actual Character to Playing.  
     * \return pointer to actual character to playing.
     ***************************************************************/
     character* actualCharacter();
     /*!
     *************************************************************** 
     * Clears The initiative List.  
     ***************************************************************/
     void clear();

   private:
     initiativeStruct* first;  /**< First character on initiative list */
     initiativeStruct* next;   /**< Next character on initiative list */
     initiativeStruct* actual; /**< Actual character on initiative list */
};

#endif


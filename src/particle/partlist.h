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

#ifndef _dnt_partlist_h
#define _dnt_partlist_h

#include "particle.h"

/*! List of particles Systems  */
class particleList
{
   public:
      /*! Constructor  */
      particleList();
      /*! Destructor */
      ~particleList();

      /*! Get the first element on the list
       * \return -> pointer to the first element on the list */
      particleSystem* getFirst();

      /*! Get the total systems on the list
       * \return -> total elements on the list */
      int getTotal();

      /*! Add a particle System to the list
       * \param part -> system to add */
      void addSystem(particleSystem* part);

      /*! Remove a particle System from the list. Note: it don't free 
       * the system's structs, so, delete them after remove!
       * \param part -> system to remove */
      void removeSystem(particleSystem* part);

   protected:
      particleSystem* first;     /**< First Element on the list */
      int total;                 /**< Total Elements on the list */
};

#endif


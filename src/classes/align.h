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

#ifndef _dnt_align_h
#define _dnt_align_h

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
using namespace std;
#include "../etc/dirs.h"

/*! Single Alignment Definition */
class align
{
   public:
      int intID;              /**< Integer Identifier of the alignment */
      string strID;           /**< String Identifier of the alignment */
      SDL_Surface* image;     /**< Image of the Aligment */
      
      string name;            /**< Name of the Aligment */
      string description;     /**< Description of the Aligment */
      
      align* previous;        /**< Previous Aligment on list */
      align* next;            /**< Next Aligment on list */
};

/**! The List of Alignments Class */
class aligns
{
   public:
      /*! Static Constructor */
      void init();
      /*! Static Destructor */
      void finish();

      /*! Get an Alignment from list
       * \param id -> integer ID of the alignment
       * \return aligment found, if found one. */
      align* getAlignByInteger(int id);
      /*! Get an Aligment from list 
       * \param id -> string ID of the alignment
       * \return aligment found, if found one. */
      align* getAlignByString(string id);
      
   private:
      static int totalAlignments;          /**< Total Alignments on List */
      static align* first;                 /**< First Alignment on List */

      /*! Insert an alignment on List
       * \param fileName -> fileName of the alignment to insert 
       * \param idString -> String Identificator of the alignment 
       * \param idInt -> Integer Identificator of the the alignment */
      void insertAlign(string fileName, string idString, int idInt);
};


#endif

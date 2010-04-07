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

#ifndef _dnt_dirs_h
#define _dnt_dirs_h

#include <string>
using namespace std;


/*! The directory abstraction path */
class dirs
{
   public:
      /*! Find Where on the system data current is:
       * (first try to find at ../data, if not found, isn´t running the local
       *  state, so try at the DATADIR, if still not found, we must have an 
       *  error) */
      void findDataDirectories();

      /*! Get the real path and file of an internal file path
       * \param fileName -> internal file path
       * \return -> string with the real path of the file */
      string getRealFile(string fileName);

      /*! Get the relative (internal) file path of a real path file
       * \param fileName -> real file path
       * \return -> string with the internal path of the file */
      string getRelativeFile(string fileName);

   protected:
      static string dataPath; /**< Current Data Path */
};


#endif



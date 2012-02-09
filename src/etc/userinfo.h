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

#ifndef _dnt_userinfo_h
#define _dnt_userinfo_h

#include <string>

/*! The User Info Class define the user name and home dir (where will save
 * all needed files like options, saves, etc) */
class userInfo
{
   public:
      /*! Get all user info from the system.
       * \note -> this function is usually called at 
       *          the beggining of the program */
      void getValuesFromSystem();

      /*! Get The User Login Name */
      std::string getUserName();

      /*! Get the User Home */
      std::string getUserHome();

      /*! Get the Saves Directory */
      std::string getSavesDirectory();

   protected:
      static std::string userName;    /**< The User Name */
      static std::string userHome;    /**< The User Home */
};



#endif


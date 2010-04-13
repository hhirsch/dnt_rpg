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

#include "userinfo.h"

#ifdef _MSC_VER
   #include <windows.h>
#else
   #include <pwd.h>
   #include <unistd.h>
   #include <sys/types.h>
#endif

#include <iostream>
using namespace std;

/****************************************************************
 *                      getValuesFromSystem                     *
 ****************************************************************/
void userInfo::getValuesFromSystem()
{
   #ifdef _MSC_VER
      char buffer[512];
      DWORD bufSize = sizeof(buffer);

      /* Try to define windows user´s name */
      if(GetUserName(&buffer[0], &bufSize))
      {
         userName = buffer;
      }
      else
      {
         userName = "Don Ramon";
      }
      
      /* Try to define windows user´s home */
      if(ExpandEnvironmentStrings("%USERPROFILE%",&buffer[0],512) != 0)
      {
          userHome = buffer;
          userHome += "/.dccnitghtmare/";
      }
      else
      {
         userHome = "./"; 
      }
   #elif defined (__amigaos4__)
      userName = "Don Ramon";
      userHome = "PROGDIR:data/userdata/";
   #else
      /* Get all Current User´s Info (so more clean, isn´t it?) */
      struct passwd *info;
      info = getpwuid(getuid());
      userName = info->pw_name;
      userHome = info->pw_dir;
      userHome += "/.dccnitghtmare/";
   #endif
}

/****************************************************************
 *                        getUserName                           *
 ****************************************************************/
string userInfo::getUserName()
{
   return(userName);
}

/****************************************************************
 *                        getUserHome                           *
 ****************************************************************/
string userInfo::getUserHome()
{
   return(userHome);
}

/****************************************************************
 *                     getSavesDirectory                        *
 ****************************************************************/
string userInfo::getSavesDirectory()
{
   return(userHome + "saves/");
}

//
//
////////Static Vars///////
//
//

string userInfo::userName;
string userInfo::userHome;



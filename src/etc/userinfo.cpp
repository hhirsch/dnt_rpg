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



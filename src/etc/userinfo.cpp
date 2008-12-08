#include "userinfo.h"

#ifdef _MSC_VER
   #include <windows.h>
#else
   #include <pwd.h>
   #include <unistd.h>
   #include <sys/types.h>
#endif

/****************************************************************
 *                      getValuesFromSystem                     *
 ****************************************************************/
void userInfo::getValuesFromSystem()
{
   #ifdef _MSC_VER
      char name[512];
      DWORD bufSize = sizeof(name);
      if(GetUserName(&name[0], &bufSize))
      {
         userName = name;
      }
      else
      {
         userName = "Don Ramon";
      }
      
      userHome = "./";
   #else
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



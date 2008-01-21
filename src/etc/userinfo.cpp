#include "userinfo.h"

#ifndef _MSC_VER
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
      //FIXME use windows functions to get this.
      userName = "don_Ramon";
      userHome = "./";
   #else
      struct passwd *info;
      info = getpwuid(getuid());
      userName = info->pw_name;
      userHome = info->pw_dir;
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

//
//
////////Static Vars///////
//
//

string userInfo::userName;
string userInfo::userHome;



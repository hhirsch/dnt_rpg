#ifndef _userinfo_h
#define _userinfo_h

#include <string>
using namespace std;

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
      string getUserName();

      /*! Get the User Home */
      string getUserHome();

   protected:
      static string userName;    /**< The User Name */
      static string userHome;    /**< The User Home */
};



#endif


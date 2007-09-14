
#include "translate.h"

/************************************************************
 *                     translateDataString                  *
 ************************************************************/
string translateDataString(string dataString)
{
   if( (dataString.length() > 10) && (dataString[0] == 'g') &&
       (dataString[1] == 'e') && (dataString[2] == 't') && 
       (dataString[3] == 't') && (dataString[4] == 'e') &&
       (dataString[5] == 'x') && (dataString[6] == 't') ) 
   {
      /* Is a translatable string */

      /* Verify and remove \n or DOS carriage at the end */
      if( (dataString[dataString.length()-1] == '\n') ||
          (dataString[dataString.length()-1] == 13) )
      {
         dataString.erase(dataString.length()-1);
      }

      /* Remove gettext(" */
      dataString.erase(0,9);
      /* Remove ") */
      dataString.erase(dataString.length()-2);

      return(gettext(dataString.c_str()));
   }

   /* Not a translatable one */
   return(dataString);
}


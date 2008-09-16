#include "savefile.h"
#include "../etc/userinfo.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
saveFile::saveFile()
{
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
saveFile::~saveFile()
{
}

/***********************************************************************
 *                               save                                  *
 ***********************************************************************/
bool saveFile::save(string title, string saveFile, modState* modifState,
                    string curMap)
{
   ofstream file;
   
   /* Define Save Path */
   userInfo uInfo;
   string savePath = uInfo.getUserHome() + saveFile;

   /* Open the File to save */
   file.open(savePath.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error Opening file: " << savePath << endl;
      return(false);
   }
   
   /* Version who created the savefile */
   file << "$dnt$" << VERSION << endl;
   /* Save file title */
   file << "title = " << title << endl;
   /* TODO Save Image */
   //file << "image = " << image << endl;
   /* Current Map */
   file << "currentMap = " << curMap << endl;

   /* TODO Save the Modif State File */

   /* TODO Save the Mission Status File */

   /* TODO Save the Current PC Status File */

   /* Close the file */
   file.close();
   return(true);
}

/***********************************************************************
 *                               load                                  *
 ***********************************************************************/
bool saveFile::load(string fileName, modState* modifState)
{
   //TODO
   return(true);
}


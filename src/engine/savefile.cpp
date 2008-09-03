#include "saveFile.h"

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
   mState = NULL;
   opened = false;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
saveFile::~saveFile()
{
   /* Close the file if it is opened */
   if(opened)
   {
      file.close();
   }
}

/***********************************************************************
 *                               save                                  *
 ***********************************************************************/
bool saveFile::save(string fileName, modState* modifState)
{
   //TODO
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

/***********************************************************************
 *                             saveHeader                              *
 ***********************************************************************/
bool saveFile::saveHeader()
{
   if(opened)
   {
      file << "$dnt$" << VERSION << endl;
      //FIXME put the image
      file << "imagePath = " << endl;
      //FIXME put the current map
      file << "currentMap = " << endl;
      return(true);
   }

   return(false);
}


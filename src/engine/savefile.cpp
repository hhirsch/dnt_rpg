#include "savefile.h"
#include "../etc/userinfo.h"
#include "../etc/defparser.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

#define DNT_SAVE_TOKEN_VERSION    "version"
#define DNT_SAVE_TOKEN_TITLE      "title"
#define DNT_SAVE_TOKEN_IMAGE      "image"
#define DNT_SAVE_TOKEN_MAP        "map"
#define DNT_SAVE_TOKEN_MOD_STATE  "modState"
#define DNT_SAVE_TOKEN_MISSIONS   "missions"
#define DNT_SAVE_TOKEN_PC         "pc"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
saveFile::saveFile()
{
   title = "";
   version = "";
   imageFile = "";
   mapFile = "";
   modStateFile = "";
   missionsFile = "";
   pcFile = "";
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
saveFile::~saveFile()
{
}

/***********************************************************************
 *                               getTitle                              *
 ***********************************************************************/
string saveFile::getTitle()
{
   return(title);
}

/***********************************************************************
 *                             getImageFile                            *
 ***********************************************************************/
string saveFile::getImageFile()
{
   return(imageFile);
}

/***********************************************************************
 *                             getCurrentMap                           *
 ***********************************************************************/
string saveFile::getCurrentMap()
{
   return(mapFile);
}

/***********************************************************************
 *                           getCharacterFile                          *
 ***********************************************************************/
string saveFile::getCharacterFile()
{
   return(pcFile);
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

   /* Define the Save Prefix */
   string prefix = "";
   if(savePath.length() > 4)
   {
      /* Get the prefix: file path without .sav */
      prefix = savePath.substr(0, savePath.length()-4);
   }

   /* Open the Header File to save */
   file.open(savePath.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error Opening file: " << savePath << endl;
      return(false);
   }
   
   /* Version who created the savefile */
   file << DNT_SAVE_TOKEN_VERSION << VERSION << endl;
   /* Save file title */
   file << DNT_SAVE_TOKEN_TITLE << title << endl;
   /* Save Image path */
   file << DNT_SAVE_TOKEN_IMAGE << prefix << ".png" << endl;
   /* Current Map */
   file << DNT_SAVE_TOKEN_MAP << curMap << endl;
   /* Save the Modif State File */
   file << DNT_SAVE_TOKEN_MOD_STATE << prefix << ".mod" << endl;
   /* Save the Mission Status File */
   file << DNT_SAVE_TOKEN_MISSIONS << prefix << ".mis" << endl;
   /* Save the Current PC Status File */
   file << DNT_SAVE_TOKEN_PC << prefix << ".pc" << endl;

   /* Close the file */
   file.close();

   
   //TODO Call save to modState, current missions, screenshot and pc

   return(true);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool saveFile::load(modState* modifState)
{
   //TODO
   return(false);
}

/***********************************************************************
 *                             loadHeader                              *
 ***********************************************************************/
bool saveFile::loadHeader(string fileName)
{
   defParser parser;
   string key, value;

   /* Define Path */
   userInfo uInfo;
   string path = uInfo.getUserHome() + fileName;

   /* Parse It! */
   if(parser.load(path, true))
   {
      while(parser.getNextTuple(key, value))
      {
         if(key == DNT_SAVE_TOKEN_VERSION)
         {
            version = value;
            if(version != VERSION)
            {
               //TODO -> throw error!
               return(false);
            }
         }
         else if(key == DNT_SAVE_TOKEN_IMAGE)
         {
           /* Define the image file */
           imageFile = value;
         }
         else if(key == DNT_SAVE_TOKEN_TITLE)
         {
            /* Define the title */
            title = value;
         }
         else if(key == DNT_SAVE_TOKEN_MOD_STATE)
         {
            /* Define the modState file */
            modStateFile = value;
         }
         else if(key == DNT_SAVE_TOKEN_MISSIONS)
         {
            /* Define the missions file */
            missionsFile = value;
         }
         else if(key == DNT_SAVE_TOKEN_PC)
         {
            /* Define the pc file */
            pcFile = value;
         }
         else if(key == DNT_SAVE_TOKEN_MAP)
         {
            /* Define the current map file */
            mapFile = value;
         }
      }
   }
   
   return(true);
}


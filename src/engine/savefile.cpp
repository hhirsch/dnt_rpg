#include "savefile.h"
#include "../etc/userinfo.h"
#include "../etc/defparser.h"

#include "dccnit.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

#define DNT_SAVE_TOKEN_VERSION       "version"
#define DNT_SAVE_TOKEN_TITLE         "title"
#define DNT_SAVE_TOKEN_IMAGE         "image"
#define DNT_SAVE_TOKEN_MAP           "map"
#define DNT_SAVE_TOKEN_MOD_STATE     "modState"
#define DNT_SAVE_TOKEN_MISSIONS      "missions"
#define DNT_SAVE_TOKEN_PC            "pc"
#define DNT_SAVE_TOKEN_PC_POSITION   "pcPosition"

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
   pcPos[0] = 0;
   pcPos[1] = 0;
   pcPos[2] = 0;
   pcAngle = 0;
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
bool saveFile::save(string saveTitle, string saveFile, void* curEngine)
{
   ofstream file;
   engine* eng = (engine*)curEngine;

   modState mState;

   /* Verify if we got an engine */
   if(eng == NULL)
   {
      return(false);
   }

   /* Flush the npcs inventories to the modState */
   eng->keepNPCInventoryStatus();
   
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

   /* Define variables/files*/
   title = saveTitle;
   version = VERSION;
   imageFile = prefix + ".bmp";
   mapFile = eng->getCurrentMap()->getFileName();
   modStateFile = prefix + ".mod";
   missionsFile = prefix + ".mis";
   pcFile = prefix + ".pc";
   pcPos[0] = eng->PCs->getActiveCharacter()->xPosition;
   pcPos[1] = eng->PCs->getActiveCharacter()->yPosition;
   pcPos[2] = eng->PCs->getActiveCharacter()->zPosition;
   pcAngle = eng->PCs->getActiveCharacter()->orientation;

   /* Save to the header file */
   
   /* Version who created the savefile */
   file << DNT_SAVE_TOKEN_VERSION << " = " << VERSION << endl;
   /* Save file title */
   file << DNT_SAVE_TOKEN_TITLE << " = " << title << endl;
   /* Save Image path */
   file << DNT_SAVE_TOKEN_IMAGE << " = " << imageFile << endl;
   /* Current Map */
   file << DNT_SAVE_TOKEN_MAP << " = " << mapFile << endl;
   /* Save the Modif State File */
   file << DNT_SAVE_TOKEN_MOD_STATE << " = " << modStateFile << endl;
   /* Save the Mission Status File */
   file << DNT_SAVE_TOKEN_MISSIONS << " = " << missionsFile << endl;
   /* Save the Current PC Status File */
   file << DNT_SAVE_TOKEN_PC << " = " << pcFile << endl;
   file << DNT_SAVE_TOKEN_PC_POSITION << " = " << pcPos[0] << ","  
                                               << pcPos[1] << "," 
                                               << pcPos[2] << ":" 
                                               << pcAngle << endl;
   
   /* Close the file */
   file.close();

   /* Save the PC File */
   eng->PCs->getActiveCharacter()->save(pcFile);

   /* Save the ModState File */
   mState.saveState(modStateFile);
   
   //TODO Call save to modState, current missions, screenshot


   return(true);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool saveFile::load(void* curEngine)
{
   engine* eng = (engine*)curEngine;
   modState modif;

   /* Verify if we got a valid engine */
   if(eng == NULL)
   {
      return(false);
   }

   /* Verify load file version */
   if(version != VERSION)
   {
      cerr << "Warning: loading an old version file: " << version << endl;
   }

   /* Load the pc file! */
   if(!pcFile.empty())
   {
      /* Load the Playable character */
      if(!eng->loadPC(pcFile))
      {
         cerr << "Can't load pc of save file!" << endl;
         return(false);
      }
   }
   
   if(!modStateFile.empty())
   {
      /* Load the modState! */
      modif.loadState(modStateFile);
   }

   if(!mapFile.empty())
   {
      /* Load the map! */
      eng->loadMap(mapFile);

      /* Set Character Position */
      character* pc = eng->PCs->getActiveCharacter();      
      if(pc)
      {
         pc->xPosition = pcPos[0];
         pc->yPosition = pcPos[1];
         pc->zPosition = pcPos[2];
         Map* actualMap = eng->getCurrentMap();
         int posX =(int)floor(pc->xPosition / actualMap->squareSize());
         int posZ =(int)floor(pc->zPosition / actualMap->squareSize());
         pc->ocSquare = actualMap->relativeSquare(posX,posZ);
      }
   }

   if(!missionsFile.empty())
   {
      /* Load the missions! */
      // TODO
   }

   return(true);
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
         else if(key == DNT_SAVE_TOKEN_PC_POSITION)
         {
            /* Define the positions */
            sscanf(value.c_str(), "%f,%f,%f %f", &pcPos[0], &pcPos[1], 
                                                 &pcPos[2], &pcAngle);
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


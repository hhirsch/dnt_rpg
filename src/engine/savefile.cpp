/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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


#include "savefile.h"

#include "../etc/userinfo.h"
#include "../etc/defparser.h"

#include "../classes/mission.h"

#include "dccnit.h"
#include "shortcutswindow.h"
#include "util.h"

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
#define DNT_SAVE_TOKEN_PC_INVENTORY  "pcInventory"
#define DNT_SAVE_TOKEN_PC_POSITION   "pcPosition"
#define DNT_SAVE_TOKEN_HOUR          "hour"
#define DNT_SAVE_TOKEN_CAMERA_INFO   "cameraInfo"
#define DNT_SAVE_TOKEN_QUICK_TALENTS "quickTalents"

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
   invFile = "";
   pcPos[0] = 0;
   pcPos[1] = 0;
   pcPos[2] = 0;
   pcAngle = 0;
   hour = 9.0f;
   cameraPhi = 75.75f;
   cameraTheta = 45.0f;
   cameraDeltaY = 0.0f;
   cameraD = 320.0f;
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
 *                         getQuickTalentsFile                         *
 ***********************************************************************/
string saveFile::getQuickTalentsFile()
{
   return(quickTalentsFile);
}

/***********************************************************************
 *                               save                                  *
 ***********************************************************************/
bool saveFile::save(string saveFile, engine* curEngine, 
      SDL_Surface* frontSurface)
{
   ofstream file;
   char buf[256];
   int i;

   modState mState;
   modInventory* inv;
   character* pc;
   missionsController missions;
   shortcutsWindow scWindow;

   /* Verify if we got an curEngineine */
   if(curEngine == NULL)
   {
      return(false);
   }

   /* Flush the npcs inventories to the modState */
   curEngine->keepNPCStatus();
   pc = curEngine->PCs->getActiveCharacter();
   
   /* Define the Save Prefix */
   string prefix = "";
   if(saveFile.length() > 4)
   {
      /* Get the prefix: file path without .sav */
      prefix = saveFile.substr(0, saveFile.length()-4);
   }

   /* Open the Header File to save */
   file.open(saveFile.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error Opening file: " << saveFile << endl;
      return(false);
   }

   /* Define the title */
   title = pc->name + ", " + pc->actualRace->name + ", " +
           pc->actualAlign->name;
   for(i=0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(pc->actualClass[i])
      {
         sprintf(buf,"%s(%d)", pc->actualClass[i]->name.c_str(),
                 pc->classLevels[i]);
         title += ", ";
         title += buf;
      }
   }

   /* Define variables/files */
   version = VERSION;
   imageFile = prefix + ".bmp";
   mapFile = curEngine->getCurrentMap()->getFileName();
   modStateFile = prefix + ".mod";
   missionsFile = prefix + ".mis";
   quickTalentsFile = prefix + ".qck";
   pcFile = prefix + ".pc";
   invFile = prefix + ".inv";
   pcPos[0] = pc->scNode->getPosX();
   pcPos[1] = pc->scNode->getPosY();
   pcPos[2] = pc->scNode->getPosZ();
   pcAngle = pc->scNode->getAngleY();

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
   /* Save the quick talents file  */
   file << DNT_SAVE_TOKEN_QUICK_TALENTS << " = " << quickTalentsFile << endl;
   /* Save the Current PC Status File */
   file << DNT_SAVE_TOKEN_PC << " = " << pcFile << endl;
   file << DNT_SAVE_TOKEN_PC_INVENTORY << " = " << invFile << endl;
   file << DNT_SAVE_TOKEN_PC_POSITION << " = " << pcPos[0] << ","  
                                               << pcPos[1] << "," 
                                               << pcPos[2] << ":" 
                                               << pcAngle << endl;
   file << DNT_SAVE_TOKEN_CAMERA_INFO << " = " 
                                      << curEngine->gameCamera.getPhi() << ":"
                                      << curEngine->gameCamera.getTheta() << ":"
                                      << curEngine->gameCamera.getDeltaY() << ":"
                                      << curEngine->gameCamera.getD() << endl;
   file << DNT_SAVE_TOKEN_HOUR << " = " << curEngine->getHour() << endl;

   /* Close the file */
   file.close();

   /* Save the PC File */
   pc->save(pcFile);

   /* Save the PC Inventory */
   inv = new modInventory(pc->inventories, pc->getCharacterFile(), "");
   ofstream f;
   f.open(invFile.c_str(), ios::out | ios::binary);
   if(f)
   {
      inv->save(&f);
   }
   else
   {
      cerr << "Can't save file: " << invFile << endl;
   }
   f.close();
   delete(inv);

   /* Save the ModState File */
   mState.saveState(modStateFile);
   
   /* Save the Missions */
   missions.save(missionsFile);

   /* Save quick talents */
   scWindow.saveQuickTalents(quickTalentsFile);

   /* Save the Screenshot */
   screenshot(frontSurface, imageFile, true);

   return(true);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool saveFile::load(engine* curEngine)
{
   modState modif;
   modInventory* inv;
   missionsController missions;
   shortcutsWindow scWindow;

   /* Verify if we got a valid curEngineine */
   if(curEngine == NULL)
   {
      return(false);
   }

   /* Set the current hour */
   curEngine->setHour(hour);

   /* Verify load file version */
   if(version != VERSION)
   {
      cerr << "Warning: loading an old version file: " << version << endl;
   }

   /* Load the pc file! */
   if(!pcFile.empty())
   {
      /* Load the Playable character */
      if(!curEngine->loadPC(pcFile))
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
      curEngine->loadMap(mapFile, true);

      /* Set Character Position */
      character* pc = curEngine->PCs->getActiveCharacter();      
      if(pc)
      {
         pc->scNode->set(pcPos[0], pcPos[1], pcPos[2], 0.0f, pcAngle, 0.0f);
         Map* actualMap = curEngine->getCurrentMap();
         int posX =(int)floor(pcPos[0] / actualMap->squareSize());
         int posZ =(int)floor(pcPos[2] / actualMap->squareSize());
         pc->ocSquare = actualMap->relativeSquare(posX,posZ);
         pc->updateHealthBar();
      }
   }

   /* Load the PC Inventory */
   if(!invFile.empty())
   {
      character* pc = curEngine->PCs->getActiveCharacter();

      if(pc)
      {
         inv = new modInventory(NULL, "", "");
         inv->load(invFile);
         inv->flush(curEngine->getCurrentMap(), pc->inventories); 
         delete(inv);
      }
   }

   if(!missionsFile.empty())
   {
      /* Load the missions! */
      missions.load(missionsFile);
   }

   scWindow.setClearedTalents(false);
   if(!quickTalentsFile.empty())
   {
      /* Set quick talents */
      character* pc = curEngine->PCs->getActiveCharacter();

      if(pc)
      {
         scWindow.loadQuickTalents(quickTalentsFile, pc);
      }
   }

   /* Finally, reset the camera */
   curEngine->gameCamera.setPhi(cameraPhi);
   curEngine->gameCamera.setTheta(cameraTheta);
   curEngine->gameCamera.setDeltaY(cameraDeltaY);
   curEngine->gameCamera.setD(cameraD);

   return(true);
}

/***********************************************************************
 *                             loadHeader                              *
 ***********************************************************************/
bool saveFile::loadHeader(string fileName)
{
   defParser parser;
   string key, value;

   /* Parse It! */
   if(!parser.load(fileName, true))
   {
      return(false);
   }

   while(parser.getNextTuple(key, value))
   {
      if(key == DNT_SAVE_TOKEN_VERSION)
      {
         version = value;
         if(version != VERSION)
         {
            cerr << "Warning: loading an old version file: " << version << endl;
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
      else if(key == DNT_SAVE_TOKEN_QUICK_TALENTS)
      {
         /* Define quick talents file */
         quickTalentsFile = value;
      }
      else if(key == DNT_SAVE_TOKEN_PC)
      {
         /* Define the pc file */
         pcFile = value;
      }
      else if(key == DNT_SAVE_TOKEN_PC_INVENTORY)
      {
         /* Define Inventory File */
         invFile = value;
      }
      else if(key == DNT_SAVE_TOKEN_PC_POSITION)
      {
         /* Define the positions */
         sscanf(value.c_str(), "%f,%f,%f:%f", &pcPos[0], &pcPos[1], 
               &pcPos[2], &pcAngle);
      }
      else if(key == DNT_SAVE_TOKEN_MAP)
      {
         /* Define the current map file */
         mapFile = value;
      }
      else if(key == DNT_SAVE_TOKEN_CAMERA_INFO)
      {
         sscanf(value.c_str(), "%f:%f:%f:%f", &cameraPhi, &cameraTheta,
                                              &cameraDeltaY, &cameraD);
      }
      else if(key == DNT_SAVE_TOKEN_HOUR)
      {
         /* Define the current hour */
         sscanf(value.c_str(), "%f", &hour);
      }
   }

   return(true);
}


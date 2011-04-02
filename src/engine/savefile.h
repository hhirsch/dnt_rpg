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

#ifndef _dnt_save_file_h
#define _dnt_save_file_h

#include "modstate.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class engine;

/*! The saveFile class is the access to the save/load game functionallity */
class saveFile
{
   public:
      /*! Constructor */
      saveFile();

      /*! Destructor */
      ~saveFile();

      /*! Load save file files
       * \param curEngine -> pointer to the current engine
       * \return -> true if can load, false otherwise 
       * \note -> you must read the header with loadHeader before call
       *          this function */
      bool load(engine* curEngine);

      /*! Load Save file header, before really load the save
       * \param fileName -> header filename (without path)
       * \return -> true if can load the header, false otherwise */
      bool loadHeader(string fileName);

      /*! Create a save file
       * \param saveFile -> save file name string
       * \param modifState -> the modState to save to the file
       * \param curMap -> current opened map
       * \param curEngine -> pointer to the current engine
       * \param frontSurface -> pointer to the frontSurface (screenshot)
       * \return -> true if can save, false otherwise */
      bool save(string saveFile, engine* curEngine, SDL_Surface* frontSurface);

      /*! Get the save file title
       * \return -> save file title */
      string getTitle();
      /*! Get the image file
       * \return -> save image file name */
      string getImageFile();
      /*! Get the current map filename
       * \return -> current map filename */
      string getCurrentMap();
      /*! Get the current PC filename
       * \return -> saved character filename */
      string getCharacterFile();
      /*! Get quick talents filename
       * \return -> quick talents filename */
      string getQuickTalentsFile();

   protected:

      string title;            /**< The save File title*/
      string version;          /**< The DNT version who created the save */
      string imageFile;        /**< The screenshot file */
      string invFile;          /**< The inventory file */
      string mapFile;          /**< The current map file */
      string modStateFile;     /**< The modState file */
      string missionsFile;     /**< The missions file */
      string quickTalentsFile; /**< Quick talents file */
      string pcFile;           /**< The PC file */
      float pcPos[3];          /**< The PC Position */
      float pcAngle;           /**< The PC angle */
      float hour;              /**< The current game hour */
      float cameraPhi;         /**< Camera Phi angle */
      float cameraTheta;       /**< Camera Theta angle */
      float cameraDeltaY;      /**< Camera DeltaY */
      float cameraD;           /**< Camera Zoom value */

};

#endif


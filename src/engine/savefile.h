#ifndef _dnt_save_file_h
#define _dnt_save_file_h

#include "modstate.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*! The saveFile class is the access to the save/load game functionallity */
class saveFile
{
   public:
      /*! Constructor */
      saveFile();

      /*! Destructor */
      ~saveFile();

      /*! Load save file files
       * \param modifState -> the modState to load from file
       * \return -> true if can load, false otherwise 
       * \note -> you must read the header with loadHeader before call
       *          this function */
      bool load(modState* modifState);

      /*! Load Save file header, before really load the save
       * \param fileName -> header filename (without path)
       * \return -> true if can load the header, false otherwise */
      bool loadHeader(string fileName);

      /*! Create a save file
       * \param saveTitle -> save file title
       * \param saveFile -> save file name string
       * \param modifState -> the modState to save to the file
       * \param curMap -> current opened map
       * \return -> true if can save, false otherwise */
      bool save(string saveTitle, string saveFile, modState* modifState,
                string curMap);

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

   protected:

      string title;          /**< The save File title*/
      string version;        /**< The DNT version who created the save */
      string imageFile;      /**< The screenshot file */
      string mapFile;        /**< The current map file */
      string modStateFile;   /**< The modState file */
      string missionsFile;   /**< The missions file */
      string pcFile;         /**< The PC file */


};

#endif


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

      /*! Load save file file
       * \param fileName -> save file name string
       * \param modifState -> the modState to load from file
       * \return -> true if can load, false otherwise */
      bool load(string fileName, modState* modifState);

      /*! Create a save file
       * \param title -> save file title
       * \param saveFile -> save file name string
       * \param modifState -> the modState to save to the file
       * \param curMap -> current opened map
       * \return -> true if can save, false otherwise */
      bool save(string title, string saveFile, modState* modifState,
                string curMap);


   protected:

};

#endif


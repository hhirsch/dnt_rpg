/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_map_editor_list_window_h
#define _dnt_map_editor_list_window_h

#include "../../gui/farso.h"

/* NPCs */
#define STATE_CHARACTERS   1

/* Objects */
#define STATE_BUILDING    10
#define STATE_CARS        11 
#define STATE_COMMON      12
#define STATE_DOORS       13       
#define STATE_GUNS        14
#define STATE_ICEX        15
#define STATE_NATURE      16
#define STATE_MACABRE     17
#define STATE_BATH        18
#define STATE_STREET      19
#define STATE_JUNK        20

/* Particles */
#define STATE_FIRE        30
#define STATE_GRASS       31
#define STATE_SMOKE       32
#define STATE_SNOW        33
#define STATE_WATERFALL   34

/* Music */
#define STATE_MUSIC       40

/* Texture */
#define STATE_INDOOR_TEXTURE   50
#define STATE_OUTDOOR_TEXTURE  51

/*! Element on the list */
class listElement
{
   public:
      string title;            /**< The Title of the element */
      string fileName;         /**< The name of the file of the element */
};

/*! The list Window, for selecting objects */
class listWindow
{
   public:
      /*! Constructor
       * \param gui -> pointer to the interface used */
      listWindow(guiInterface* gui);
      /*! Destructor */
      ~listWindow();

      /*! Set State
       * \param st -> state value */
      void setState(int st);

      /*! Get State
       * \return -> state value */
      int getState();

      /*! Get the current selected fileName
       * \return file name */
      string getFileName();


      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, guiObject* object);

   private:
      /*! Open the window */
      void open();

      /*! Load list from a file
       * \param filename -> full filename of the list to load */
      void loadFromFile(string fileName);

      /*! Get the filename with title title
       * \param title -> title of the filename 
       * \return -> the fielename */
      string getFileNameWithTitle(string title);

      int state;                 /**< Internal State */
      window* intWindow;         /**< Pointer to the window */
      guiInterface* intGui;      /**< Pointer to the interface */
      listText* list;            /**< Pointer to the listText */
      listElement* listElements; /**< Elements on the list */
      int total;                 /**< Total elements on list */
      string selFileName;        /**< Last Selected fileName */

};

#endif


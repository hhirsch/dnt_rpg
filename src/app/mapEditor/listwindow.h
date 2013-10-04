/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_editor_list_window_h
#define _dnt_map_editor_list_window_h

#include "../../gui/farso.h"
#include "../../etc/list.h"

namespace dntMapEditor
{

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

/*! Tuple (title, file) on the list */
class Tuple: public dntListElement
{
   public:
      std::string title;            /**< The Title of the element */
      std::string fileName;         /**< The name of the file of the element */
};

/*! List of tuples */
class TupleList: public dntList
{
   public:
      /*! Constructor */
      TupleList();
      /*! Destructor */
      ~TupleList();

   protected:
      /*! Free tuple memory */
      void freeElement(dntListElement* obj);
};

/*! The list Window, for selecting objects */
class ListWindow
{
   public:
      /*! Constructor
       * \param gui -> pointer to the interface used */
      ListWindow(Farso::GuiInterface* gui);
      /*! Destructor */
      ~ListWindow();

      /*! Set State
       * \param st -> state value */
      void setState(int st);

      /*! Get State
       * \return -> state value */
      int getState();

      /*! Get the current selected fileName
       * \return file name */
      std::string getFileName();


      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, Farso::GuiObject* object);

   private:
      /*! Open the window */
      void open();

      /*! Load list from a file
       * \param filename -> full filename of the list to load */
      void loadFromFile(std::string fileName);

      /*! Get the filename with title title
       * \param title -> title of the filename 
       * \return -> the fielename */
      std::string getFileNameWithTitle(std::string title);

      int state;                 /**< Internal State */
      Farso::Window* intWindow;         /**< Pointer to the window */
      Farso::GuiInterface* intGui;      /**< Pointer to the interface */
      Farso::ListText* list;            /**< Pointer to the listText */
      TupleList tuples;          /**< Current Tuples */
      std::string selFileName;   /**< Last Selected fileName */

};

}

#endif


/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef dnt_journal_window_h
#define dnt_journal_window_h

#include "../gui/farso.h"
#include "../etc/btree.h"
#include "../etc/list.h"

/*! The journalDesc keeps information about a single mission */
class journalDesc: public dntListElement
{
   public:
      std::string text;  /**< Description Text */
      bool completed;    /**< If completed or not */
};

/*! The journalArea keeps information about
 * all missions on an specific area */
class journalArea : public bTreeCell, public dntList
{
   public:
      /*! Constructor */
      journalArea();

      /*! Destructor */
      ~journalArea();
 
      /*! Compare with another cell
       * \param cell -> pointer to cell to compare this one to
       * \return == 0 if equal, < 0  if lesser or > 0 if greater */
      int compare(bTreeCell* cell);

      /*! Merge the current bTreeCell with another
       * \param cell -> cell to merge the current one with */
      void merge(bTreeCell* cell);

      /*! Insert a mission at the area
       * \param desc -> mission description
       * \param comp -> true if completed */
      void insertMission(std::string desc, bool comp);

      /*! Get the area name
       * \return string with area's name */
      std::string getName();

      /*! Set the area name
       * \param n -> new area's name */
      void setName(std::string n);

   protected:
      /*! Free area description memory
       * \parma obj -> areaDesc to delete */
      void freeElement(dntListElement* obj);


      std::string name;  /**< Area Name */
};

/*! The journalAreas keeps all journalArea informations */
class journalAreas : public bTree
{
   public:
      /*! Constructor */
      journalAreas();
      /*! Destructor */
      ~journalAreas();

      /*! Insert a journalArea at the bTree
       * \param title -> journalArea text 
       * \return -> pointer to the inserted journal area */
      journalArea* insert(std::string title);

      /*! Insert a journalArea at the bTree
       * \param title -> journalArea text 
       * \return -> pointer to the inserted journal area */
      journalArea* search(std::string title);

      /*! Get the journalArea at the num position
       * \param num -> position to get
       * \return -> pointer to the area got */
      journalArea* get(int num);

      /*! Get the journalArea at the num position
       * \param num -> position to get
       * \param curRoot -> current node used as root
       * \param cur -> current node position 
       * \return -> pointer to the area got */
      journalArea* get(int num, journalArea* curRoot, int& cur);

      /*! Dupplicate a cell (aka: alloc a new one with same values)
       * \param cell -> pointer to the cell to dupplicate */
      bTreeCell* dupplicateCell(bTreeCell* cell);

      /*! Delete the actual cell 
       * \param cell -> the cell to delete */
      void freeCell(bTreeCell* cell);
};

/*! The Journal Window definition.
 * This window shows current, finished and aborted missions
 * sorted by game area.
 * \todo -> TODO: the AREA idea implementation. Currently it's only a 
 *                separator at missions definitions. */
class journalWindow
{
   public:
      /*! Constructor */
      journalWindow();
      /*! Destructor */
      ~journalWindow();

      /*! Open, if not already opened, the journal window
       * \param inter -> pointer to the used guiInterface */
      void open(guiInterface* inter);

      /*! Verify if the window is opened
       * \return true if the journalWindow is opened */
      bool isOpen();
      /*! Close, if opened, the journal window */
      void close();

   protected:
      /*! Create the Journal missions lists per area */
      void createLists();

      /*! Show current Area missions at the window */
      void showArea();

      journalAreas* areas;       /**< The missions per areas */
      int curArea;               /**< Current area displayed */

      guiInterface* gui;         /**< The gui used */
      window* internalWindow;    /**< The window pointer */
      button* nextButton;        /**< The Next Area Button */
      button* previousButton;    /**< The Previous Area Button */
      textBox* areaText;         /**< The Area text */
      rolBar* missionsText;      /**< The missons text */
};

#endif


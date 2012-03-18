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

#ifndef _dnt_classwindow_h
#define _dnt_classwindow_h

#include "../gui/farso.h"
#include "../classes/classes.h"
#include "../classes/feats.h"

#define CLASSW_CANCEL  0
#define CLASSW_CONFIRM 1
#define CLASSW_OTHER   2

/*! Races Window */
class classWindow
{
   public:
      /*! Constructor 
       * \param inter -> pointer to the GUI
       * \param ftl -> pointer to the featsList
       * \param retClass -> pointer to the choosed class */
      classWindow(Farso::GuiInterface* inter, classe** retClass);

      /*! Destructor */
      ~classWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Farso::GuiObject* object, int eventInfo, 
            Farso::GuiInterface* inter); 

      Farso::Window*      intWindow;  /**< Pointer to the internal window */

   private:
      int           curClass;         /**< Pointer to current Class */
      classe**      classesOrder;     /**< Alphabetical ordered classes */
      classe**      choosedClass;     /**< Pointer to choosed pointer class */
      int           totalClasses;     /**< Total Avaible classes */

      Farso::Button* buttonConfirm;
      Farso::Button* buttonCancel;

      Farso::Button* buttonPrevious;
      Farso::Button* buttonNext;

      Farso::TextBox* textName;
      Farso::TextBox* textDescTitle;
      Farso::RolBar* textDesc;
      Farso::RolBar* textCharac;
      Farso::TextBox* textCharacTitle;
      Farso::TextBox* textCitation;

      Farso::Picture* classImage;

      /*! Draw the characteritics */
      void setCharacteristics();
      /*! Draw the descriptions */
      void setDescription();

};

#endif

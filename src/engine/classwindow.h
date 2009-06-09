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
      classWindow(guiInterface* inter, classe** retClass);

      /*! Destructor */
      ~classWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo,guiInterface* inter); 

      window*      intWindow;           /**< Pointer to the internal window */

   private:
      int           curClass;         /**< Pointer to current Class */
      classe**      classesOrder;     /**< Alphabetical ordered classes */
      classe**      choosedClass;     /**< Pointer to choosed pointer class */
      int           totalClasses;     /**< Total Avaible classes */

      button* buttonConfirm;
      button* buttonCancel;

      button* buttonPrevious;
      button* buttonNext;

      textBox* textName;
      textBox* textDescTitle;
      rolBar* textDesc;
      rolBar* textCharac;
      textBox* textCharacTitle;
      textBox* textCitation;

      picture* classImage;

      /*! Draw the characteritics */
      void setCharacteristics();
      /*! Draw the descriptions */
      void setDescription();

};

#endif

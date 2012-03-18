/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_map_editor_grass_window_h
#define _dnt_map_editor_grass_window_h

//#include "../../particle/grass.h"
#include "../../gui/farso.h"
#include "../../particle/partcontroller.h"

/*! The Grass Controller Window */
class grassWindow
{
   public:
      /*! Constructor
       * \param actualGui -> the actual GUI interface*/
      grassWindow(Farso::GuiInterface* actualGui);
      /*! Destructor */
      ~grassWindow();
#if 0      
      /*! Set the current grass (and open the window, if it is closed)
       * \param gr -> grass to show params. */
      void setGrass(grass* gr);

      /*! Draw the temporary things. */
      void drawTemporary();

      /*! Set the used particle system contoller
       * \param pS -> the particle system controller */
      void setPartSystem(partController* pS);

      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, guiObject* object);
   protected:
      /*! Open the Window */
      void openWindow();
      
      /*! Define all the values on the window */
      void defineValues();

      grass* actualGrass;     /**< the actual on edition grass */
      partController* pSystem;/**< actual particle system controller */
      guiInterface* gui;      /**< the actual used GUI */
     
      int lastTime;           /**< Last time checked ON_PRESS events */

      window* actualWindow;         /**< the actual opened window pointer */
      textBox* textPovoation;       /**< the povoation text */
      textBox* textSize;            /**< The size text */
      button* buttonSizeMore;       /**< More size button */
      button* buttonSizeLess;       /**< Less size button */
      button* buttonPovoationMore;  /**< More povoation button */
      button* buttonPovoationLess;  /**< Less povoation button */
      tabButton* navigationTab;     /**< The navigation Table */
      oneTabButton* buttonNext;     /**< Next Button */
      oneTabButton* buttonPrevious; /**< Previous Button */
      oneTabButton* buttonDestroy;  /**< Destroy Button */
#endif
};

#endif


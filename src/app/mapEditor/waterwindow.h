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

#ifndef _dnt_map_editor_waterwindow_h
#define _dnt_map_editor_waterwindow_h

#include "../../particle/partcontroller.h"
#include "../../gui/farso.h"

/* FIXME: broken. */

/*! The water controller window */
class waterWindow
{
   public:
      /*! Constructor
       * \param actualGui -> actual used interface */
      waterWindow(guiInterface* actualGui);

      /*! Destructor */
      ~waterWindow();

      /*! Set the active Water
       * \param water -> new active water */
      void setWater(particleSystem* water);

#if 0
      /*! Set the active plane
       * \param pl -> new active Plane */
      void setPlane(interPlane* pl);
#endif

      /*! Set the used particle system contoller
       * \param pS -> the particle system controller */
      void setPartSystem(partController* pS);

      /*! Verify if some event is related to the window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the window */
      bool eventGot(int type, guiObject* object);

      /*! Draw the temporary things to the screen */
      void drawTemporary();
      
   protected:

      /*! Open the water window */
      void openWindow();

      /*! Define the Values amd put on the boxes */
      void defineValues();

      particleSystem* activeWater;        /**< The active Water */
      //interPlane* activePlane;            /**< The active Plane */
      partController* pSystem;            /**< The active Controller */
      guiInterface* gui;                  /**< The used Interface */
   

      window* intWindow;                  /**< The internal window pointer */

      tabButton* waterTab;                /**< Water Tab */
      oneTabButton* buttonDestroyWater;   /**< Destroy active water */
      oneTabButton* buttonPreviousWater;  /**< Previous Water */
      oneTabButton* buttonNextWater;      /**< Next Water */

      tabButton* waterMoveTab;            /**< Water Move tab */
      oneTabButton* buttonMoveWaterUp;    /**< Water Move Up */
      oneTabButton* buttonMoveWaterDown;  /**< Water Move Down */
   
      tabButton* planesTab;               /**< Planes Tab */
      oneTabButton* buttonAddPlane;       /**< Add Plane */
      oneTabButton* buttonDestroyPlane;   /**< Destroy Active Plane */
      oneTabButton* buttonPreviousPlane;  /**< Previous Plane */
      oneTabButton* buttonNextPlane;      /**< Next Plane */

      tabButton* incTypeTab;              /**< Plane Inclination type Tab */
      oneTabButton* buttonNoInclination;  /**< No Inclination */
      oneTabButton* buttonInclinationX;   /**< X axys Inclination */
      oneTabButton* buttonInclinationZ;   /**< Z axys Inclination */

      tabButton* movXZTab;                /**< Move Plane on XZ axys Tab */
      oneTabButton* buttonMoveZMore;      /**< Move More on Z */
      oneTabButton* buttonMoveZLess;      /**< Move Less on Z */
      oneTabButton* buttonMoveXMore;      /**< Move More on X */
      oneTabButton* buttonMoveXLess;      /**< Move Less on X */

      tabButton* movYTab;                 /**< Move Plane on Y axys Tab */
      oneTabButton* buttonMoveYMore;      /**< Move More on Y */
      oneTabButton* buttonMoveYLess;      /**< Move Less on Y */
      oneTabButton* buttonMoveY1More;     /**< Move Y1 More */
      oneTabButton* buttonMoveY1Less;     /**< Move Y1 Less */
      oneTabButton* buttonMoveY2More;     /**< Move Y2 More */
      oneTabButton* buttonMoveY2Less;     /**< Move Y2 Less */

      tabButton* scaleTab;                /**< Plane Scale Tab */
      oneTabButton* buttonScaleXMore;     /**< Scale more on X */
      oneTabButton* buttonScaleXLess;     /**< Scale less on X */
      oneTabButton* buttonScaleZMore;     /**< Scale more on Z */
      oneTabButton* buttonScaleZLess;     /**< Scale less on Z */
      oneTabButton* buttonScaleYMore;     /**< Scale more on Y */
      oneTabButton* buttonScaleYLess;     /**< Scale less on Y */

      button* dxWaterflowMore;            /**< More dX flow */
      textBox* dxWaterflowText;           /**< dx flow text */
      button* dxWaterflowLess;            /**< Less dX flow */
      button* dzWaterflowMore;            /**< More dZ flow */
      textBox* dzWaterflowText;           /**< dz flow text */
      button* dzWaterflowLess;            /**< Less dZ flow */
};


#endif


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

#ifndef _dnt_map_editor_water_window_h
#define _dnt_map_editor_water_window_h

#include "../../particle/partcontroller.h"
#include "../../gui/farso.h"

/*! The water controller Farso::Window */
class waterWindow
{
   public:
      /*! Constructor
       * \param actualGui -> actual used interface */
      waterWindow(Farso::GuiInterface* actualGui);

      /*! Destructor */
      ~waterWindow();

      /*! Set the active Water
       * \param water -> new active water */
      void setWater(particleSystem* water);

      /*! Set the active plane
       * \param pl -> new active Plane */
      void setPlane(interPlane* pl);

      /*! Set the used particle system contoller
       * \param pS -> the particle system controller */
      void setPartSystem(partController* pS);

      /*! Verify if some event is related to the Farso::Window
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the Window */
      bool eventGot(int type, Farso::GuiObject* object);

      /*! Draw the temporary things to the screen */
      void drawTemporary();
      
   protected:

      /*! Open the water Farso::Window */
      void openWindow();

      /*! Define the Values amd put on the boxes */
      void defineValues();

      particleSystem* activeWater;        /**< The active Water */
      interPlane* activePlane;            /**< The active Plane */
      partController* pSystem;            /**< The active Controller */
      Farso::GuiInterface* gui;           /**< The used Interface */
   

      Farso::Window* intWindow;   /**< The internal Farso::Window pointer */

      Farso::TabButton* waterTab;                /**< Water Tab */
      Farso::OneTabButton* buttonDestroyWater;/**< Destroy activeWater */
      Farso::OneTabButton* buttonPreviousWater;  /**< Previous Water */
      Farso::OneTabButton* buttonNextWater;      /**< Next Water */

      Farso::TabButton* waterMoveTab;            /**< Water Move tab */
      Farso::OneTabButton* buttonMoveWaterUp;    /**< Water Move Up */
      Farso::OneTabButton* buttonMoveWaterDown;  /**< Water Move Down */
   
      Farso::TabButton* planesTab;               /**< Planes Tab */
      Farso::OneTabButton* buttonAddPlane;       /**< Add Plane */
      Farso::OneTabButton* buttonDestroyPlane;/**< Destroy Active Plane*/
      Farso::OneTabButton* buttonPreviousPlane;  /**< Previous Plane */
      Farso::OneTabButton* buttonNextPlane;      /**< Next Plane */

      Farso::TabButton* incTypeTab;           /**< Plane Inclination type Tab */
      Farso::OneTabButton* buttonNoInclination;  /**< No Inclination */
      Farso::OneTabButton* buttonInclinationX;/**< X axys Inclination */
      Farso::OneTabButton* buttonInclinationZ;/**< Z axys Inclination */

      Farso::TabButton* movXZTab;             /**< Move Plane on XZ axys Tab */
      Farso::OneTabButton* buttonMoveZMore;      /**< Move More on Z */
      Farso::OneTabButton* buttonMoveZLess;      /**< Move Less on Z */
      Farso::OneTabButton* buttonMoveXMore;      /**< Move More on X */
      Farso::OneTabButton* buttonMoveXLess;      /**< Move Less on X */

      Farso::TabButton* movYTab;              /**< Move Plane on Y axys Tab */
      Farso::OneTabButton* buttonMoveYMore;      /**< Move More on Y */
      Farso::OneTabButton* buttonMoveYLess;      /**< Move Less on Y */
      Farso::OneTabButton* buttonMoveY1More;     /**< Move Y1 More */
      Farso::OneTabButton* buttonMoveY1Less;     /**< Move Y1 Less */
      Farso::OneTabButton* buttonMoveY2More;     /**< Move Y2 More */
      Farso::OneTabButton* buttonMoveY2Less;     /**< Move Y2 Less */

      Farso::TabButton* scaleTab;                /**< Plane Scale Tab */
      Farso::OneTabButton* buttonScaleXMore;     /**< Scale more on X */
      Farso::OneTabButton* buttonScaleXLess;     /**< Scale less on X */
      Farso::OneTabButton* buttonScaleZMore;     /**< Scale more on Z */
      Farso::OneTabButton* buttonScaleZLess;     /**< Scale less on Z */
      Farso::OneTabButton* buttonScaleYMore;     /**< Scale more on Y */
      Farso::OneTabButton* buttonScaleYLess;     /**< Scale less on Y */

      Farso::Button* dxWaterflowMore;            /**< More dX flow */
      Farso::TextBox* dxWaterflowText;           /**< dx flow text */
      Farso::Button* dxWaterflowLess;            /**< Less dX flow */
      Farso::Button* dzWaterflowMore;            /**< More dZ flow */
      Farso::TextBox* dzWaterflowText;           /**< dz flow text */
      Farso::Button* dzWaterflowLess;            /**< Less dZ flow */
};


#endif


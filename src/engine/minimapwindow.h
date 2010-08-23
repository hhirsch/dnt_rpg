/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _dnt_minimap_window_h
#define _dnt_minimap_window_h

#include "../gui/farso.h"
#include "../map/map.h"

/*! The minimap window */
class miniMapWindow
{
   public:
      /*! Constructor */
      miniMapWindow();
      /*! Destructor */
      ~miniMapWindow();

      /*! Open the minimap window (if not already opened)
       * \param gui -> guiInterface where the window will open
       * \param posX -> character X position
       * \param posZ -> character Z position 
       * \param openedMap -> current opened map */
      void open(guiInterface* gui, float posX, float posZ,
                Map* openedMap);
      /*! Close the minimap window (if opened)
       * \param gui -> guiInterface where the window is */
      void close(guiInterface* gui);

      /*! Update the active character map position
       * \param posX -> character X position
       * \param posZ -> character Z position */
      void updateCharacterPosition(float posX, float posZ);

      /*! Verify if the minimap window is opened
       * \return -> true if is opened, false otherwise */
      bool isOpened();

      /*! Reopen the Window (if is opened) 
       * \param gui -> pointer to the used guiInterface 
       * \param openedMap -> current opened map */
      void reOpen(guiInterface* gui, Map* openedMap);

   protected:
      static int width;             /**< Current picture width */
      static int height;            /**< Current picture height */
      static picture* fig;          /**< The current minimap picture */
      static window* mapWindow;     /**< MiniMap Window */
      static button* charPosition;  /**< Character MiniMap Button */
      static Map* curMap;           /**< Current Opened Map */
};

#endif


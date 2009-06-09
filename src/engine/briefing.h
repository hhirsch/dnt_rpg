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

#ifndef _dnt_briefing_h
#define _dnt_briefing_h

#include "../gui/farso.h"

/*! The briefing class have the briefing lines added and a briefing window */
class briefing
{
   public:
      /*! Constructor */
      briefing();
      /*! Destructor */
      ~briefing();

      /*! Open the briefing window (if not already openned) 
       * \param gui -> interface to use */
      void openWindow(guiInterface* gui);
 
      /*! Close the briefing window (if opened) 
       * \param gui -> interface where window is opened */
      void closeWindow(guiInterface* gui);

      /*! Verify if the briefing window is opened
       * \return -> true if the window is opened */
      bool isOpened();

       /*! Add the string to the briefing (with default font and colors) 
       * \param text -> text to add */
      void addText(string text);

       /*! Add the string to the briefing 
       * \param text -> text to add
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font */
      void addText(string text, int R, int G, int B);

      /*! Add the string to the briefing 
       * \param text -> text to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font */
      void addText(string text, string font, int size, int style,
                   int align, int R, int G, int B);

      /*! Clear the briefing text */
      void clear();

   protected:
      static rolBar* briefTxt;       /**< Briefing Text */
      static window* briefWindow;    /**< ShortCuts Window */
      static Uint32 timeLastAdd;     /**< Ticks when last added text */
};

#endif



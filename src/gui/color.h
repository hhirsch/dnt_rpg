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

#ifndef _farso_color_h
#define _farso_color_h

namespace Farso
{

/*! A color Definition */
class Color
{
   public:
      int R, /**< Red [0,255] */
          G, /**< Green [0,255] */
          B, /**< Blue [0,255] */
          A; /**< Alpha [0,255] */
};

/*! The GUI Colors Definition */
class Colors
{
   public:
      Color colorCont[3],       /**< Contorn Colors */
            colorWindow,        /**< Window Color */
            colorButton,        /**< Button Color */
            colorMenu,          /**< Menu Color */
            colorText,          /**< Text Color */
            colorBar,           /**< Active Window Bar Color */
            colorSelText,       /**< Color Selected Text */
            colorFile,          /**< Color for file on file lists */
            colorDirectory,     /**< color for directory on file lists */
            colorHigh;          /**< Color for highlight */

      /*! Constructor */
      Colors();

      /*! Init the defaults colors. Automatically used at farso init. */
      void init();

      /*! Set a default color to a value */
      void setDefaultColor(int colorId, int R, int G, int B, int A);

      /*! Calling them COLOUR, instead of COLOR, due to conflicts with
       * a global defined by MS-Windows for COLOR_WINDOW! */
      enum DefaultColorId
      {
         COLOUR_CONT_0 = 0,
         COLOUR_CONT_1,
         COLOUR_CONT_2,
         COLOUR_WINDOW,
         COLOUR_BUTTON,
         COLOUR_MENU,
         COLOUR_TEXT,
         COLOUR_BAR,
         COLOUR_SEL_TEXT,
         COLOUR_FILE,
         COLOUR_DIRECTORY,
         COLOUR_HIGH
      };

   protected:

      /* Deafaults */
      static Color defCont[3];
      static Color defWindow;
      static Color defButton;
      static Color defMenu;
      static Color defText;
      static Color defBar;
      static Color defSelText;
      static Color defFile;
      static Color defDir;
      static Color defHigh;
};

}

#endif


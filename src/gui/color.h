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

#ifndef _farso_color_h
#define _farso_color_h

/*! A color Definition */
class farso_color
{
   public:
      int R, /**< Red [0,255] */
          G, /**< Green [0,255] */
          B, /**< Blue [0,255] */
          A; /**< Alpha [0,255] */
};

/*! The GUI Colors Definition */
class farso_colors
{
   public:
      farso_color colorCont[3],       /**< Contorn Colors */
                  colorWindow,        /**< Window Color */ 
                  colorButton,        /**< Button Color */
                  colorMenu,          /**< Menu Color */
                  colorText,          /**< Text Color */
                  colorBar,           /**< Bar Color */
                  colorSelText;       /**< Color Selected Text */
      
       /*! Constructor */
       farso_colors();
};



#endif

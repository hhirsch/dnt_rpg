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

#ifndef _farso_opts_h
#define _farso_opts_h

#include <string>

namespace Farso
{

class Options
{
   public:

      /*! Set if using an unicode language or not
       * \param unicode -> true if using an unicode languge */
      void setUnicodeLanguage(bool unicode){languageIsUnicode = unicode;};

      /*! Get the current screen width */
      int getScreenWidth(){return(width);};
      /*! Get the current screen height */
      int getScreenHeight(){return(height);};

      /*! Set current screen width and height
       * \note -> usually, this funcion is automatically called
       *          at Farso's init. */
      void setCurrentScreen(int w, int h){width=w; height=h;};

      /*! Get if using an unicode language or not */
      bool isLanguageUnicode(){return(languageIsUnicode);};

      /*! Get default font
       * \return -> path to the default font */
      std::string getDefaultFont(){return(defaultFont);};

      /*! Get default cursor
       * \return -> path to the default font */
      std::string getDefaultCursor(){return(defaultCursor);};

      /*! Set the default needed options
       * \param fntFileName -> file name of the default font to use
       * \param cursorFileName -> file name of the default cursor image to use
       * \param maximizeIcon -> file name of the maximize icon
       * \param closeIcon -> file name of the close icon
       * \param maximizeLabel -> text for "Maximize"
       * \param closeLabel -> text for "Close"
       * \param confirmLabel -> text for "Confirm"
       * \param cancelLabel -> text for "Cancel"
       * \note ou must set the default font previously using any FarSo widget */
      void setNeededDefaults(std::string fntFileName, 
         std::string cursorFileName, std::string maximizeIcon,
         std::string closeIcon, std::string maximizeLabel, 
         std::string closeLabel, std::string confirmLabel, 
         std::string cancelLabel);

      std::string getMaximizeIcon(){return(maxIcon);};
      std::string getMaximizeLabel(){return(maxLabel);};
      std::string getCloseIcon(){return(clIcon);};
      std::string getCloseLabel(){return(clLabel);};
      std::string getConfirmLabel(){return(confirmLabel);};
      std::string getCancelLabel(){return(cancelLabel);};

   protected:
      static bool languageIsUnicode;/**< If using an unicode language or not */
      static int width;    /**< Current width */
      static int height;   /**< Current height */
      static std::string defaultFont; /**< The default font */
      static std::string defaultCursor; /**< The default cursor */
      static std::string maxIcon; /**< The maximize icon fileName */
      static std::string maxLabel; /**< The maximize label */
      static std::string clIcon; /**< The close icon filename */
      static std::string clLabel; /**< The close label */
      static std::string confirmLabel; /**< confirm label*/
      static std::string cancelLabel;  /**< cancel label */

};

}


#endif


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

#ifndef _dnt_translate_h
#define _dnt_translate_h

#ifdef _MSC_VER
   #include "../config_win.h"
#else 
#ifdef __APPLE__
   #include "../config_mac.h"
#else
   #include "../config.h"
#endif
#endif

#if ENABLE_NLS == 1
   #include <libintl.h>
#else
   /* Define a gettext ignore function */
   inline const char* gettext(const char* s){return(s);};
#endif

#include <string>

/*! Translate a data string readed from a data file.
 * \param dataString -> translatable data string readed from a data file 
 * \return -> translated string for the input data. */
std::string translateDataString(std::string dataString);


#endif


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

#ifndef _dnt_translate_h
#define _dnt_translate_h

#ifdef HAVE_INTL
   #include <libintl.h>
#else
   /* Define a gettext ignore function */
   inline const char* gettext(const char* s){return(s);};
#endif

#include <string>
using namespace std;

/*! Translate a data string readed from a data file.
 * \param dataString -> translatable data string readed from a data file 
 * \return -> translated string for the input data. */
string translateDataString(string dataString);


#endif


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

#include "farsoopts.h"
using namespace Farso;

/***********************************************************************
 *                          setNeededDefaults                          *
 ***********************************************************************/
void Options::setNeededDefaults(std::string fntFileName, 
      std::string cursorFileName, 
      std::string maximizeIcon, std::string closeIcon, 
      std::string maximizeLabel, std::string closeLabel,
      std::string confirmLabel, std::string cancelLabel)
{
   Options::defaultFont = fntFileName;
   Options::defaultCursor = cursorFileName;
   Options::maxIcon = maximizeIcon;
   Options::maxLabel = maximizeLabel;
   Options::clIcon = closeIcon;
   Options::clLabel = closeLabel;
   Options::confirmLabel = confirmLabel;
   Options::cancelLabel = cancelLabel;
}

/***********************************************************************
 *                            staticMembers                            *
 ***********************************************************************/
bool Options::languageIsUnicode = false;
int Options::width = 0;
int Options::height = 0;
std::string Options::defaultFont = "undefined";
std::string Options::defaultCursor = "undefined";
std::string Options::maxIcon = "";
std::string Options::maxLabel = "Maximize";
std::string Options::clIcon = "";
std::string Options::clLabel = "Close";
std::string Options::confirmLabel = "Confirm";
std::string Options::cancelLabel = "Cancel";


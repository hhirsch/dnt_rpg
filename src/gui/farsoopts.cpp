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

/***********************************************************************
 *                          setNeededDefaults                          *
 ***********************************************************************/
void farsoOptions::setNeededDefaults(std::string fntFileName, 
      std::string cursorFileName, 
      std::string maximizeIcon, std::string closeIcon, 
      std::string maximizeLabel, std::string closeLabel,
      std::string confirmLabel, std::string cancelLabel)
{
   farsoOptions::defaultFont = fntFileName;
   farsoOptions::defaultCursor = cursorFileName;
   farsoOptions::maxIcon = maximizeIcon;
   farsoOptions::maxLabel = maximizeLabel;
   farsoOptions::clIcon = closeIcon;
   farsoOptions::clLabel = closeLabel;
   farsoOptions::confirmLabel = confirmLabel;
   farsoOptions::cancelLabel = cancelLabel;
}

/***********************************************************************
 *                            staticMembers                            *
 ***********************************************************************/
bool farsoOptions::languageIsUnicode = false;
int farsoOptions::width = 0;
int farsoOptions::height = 0;
std::string farsoOptions::defaultFont = "undefined";
std::string farsoOptions::defaultCursor = "undefined";
std::string farsoOptions::maxIcon = "";
std::string farsoOptions::maxLabel = "Maximize";
std::string farsoOptions::clIcon = "";
std::string farsoOptions::clLabel = "Close";
std::string farsoOptions::confirmLabel = "Confirm";
std::string farsoOptions::cancelLabel = "Cancel";


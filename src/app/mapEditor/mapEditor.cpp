/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mapEditor.h"
#include "../../etc/dirs.h"
#include "../../etc/userinfo.h"
#include "../../etc/extensions.h"
#include "../../engine/options.h"

/************************************************************************
 *                              Main                                    *
 ************************************************************************/
int main(int argc, char **argv)
{
   /* Load options and directorues */
   userInfo user;
   user.getValuesFromSystem();
   dirs dir;
   dir.findDataDirectories();
   options opt;
   opt.load();
   dntMapEditor::Editor* edt;

   edt = new dntMapEditor::Editor();
   edt->run();
   delete(edt);

   return(1);
}


 

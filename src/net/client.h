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

#ifndef _CLIENT_H
#define _CLIENT_H

#include "common.h"

int initclientdata( clientdata_p_t cd );

int startconnection( clientdata_p_t cd, char * server, int port );

int closeconnection( clientdata_p_t cd );

netevent_p_t pollnet( clientdata_p_t cd );

void entergame( clientdata_p_t cd );

int movchar( clientdata_p_t cd, int obj, double x, double y, double teta );

int createchar( clientdata_p_t cd, double x, double y, double teta );

#endif

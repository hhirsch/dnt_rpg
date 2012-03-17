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

#include "cursor.h"

#include "util.h"
#include "../etc/dirs.h"
#include "../gui/farsoopts.h"

#include <iostream>
using namespace std;

const std::string cursorFile[]=
{"cursors/Walk.png", "cursors/Attack.png", "cursors/Defend.png", 
 "cursors/MapTravel.png", "cursors/talk.png", "cursors/Get.png",
 "cursors/Inventory.png", "cursors/Door.png", "cursors/forbidden.png",
 "cursors/walk_cont.png", "cursors/use.png"};

const int cursorHotSpot[][2] =
{ { 0, 0}, { 1, 0}, { 0, 0},
  {15,15}, {15,15}, { 5, 2},
  {15,15}, { 5, 0}, {15,15},
  { 0, 0}, {15,15} };



/*****************************************************************
 *                         Constructor                           *
 *****************************************************************/
cursor::cursor()
{
}

/*****************************************************************
 *                          Destructor                           *
 *****************************************************************/
cursor::~cursor()
{
}

/*****************************************************************
 *                             init                              *
 *****************************************************************/
void cursor::init()
{
   dirs dir;
   int i;
   
   /* Load Cursors */
   for(i=0; i < CURSOR_TOTAL; i++)
   {
      mCursor.set(dir.getRealFile(cursorFile[i]), 
            cursorHotSpot[i][0], cursorHotSpot[i][1]);
   }

   currentCursor = CURSOR_WALK;
}

/*****************************************************************
 *                              end                              *
 *****************************************************************/
void cursor::finish()
{
}

/*****************************************************************
 *                               set                             *
 *****************************************************************/
void cursor::set(int nCursor)
{
   dirs dir;
   if(nCursor < CURSOR_TOTAL)
   {
      currentCursor = nCursor;
      mCursor.set(dir.getRealFile(cursorFile[nCursor]), 
      cursorHotSpot[nCursor][0], cursorHotSpot[nCursor][1]);
   }
   else if(nCursor < 0)
   {
      hide();
   }
}

/*****************************************************************
 *                               hide                            *
 *****************************************************************/
void cursor::hide()
{
   currentCursor = -1;
   mCursor.hide();
}

/*****************************************************************
 *                              set                              *
 *****************************************************************/
void cursor::set(SDL_Surface* img)
{
   mCursor.set(img);
   currentCursor = CURSOR_USER_IMAGE;
}

/*****************************************************************
 *                               get                             *
 *****************************************************************/
int cursor::get()
{
   return(currentCursor);
}

/*****************************************************************
 *                      setTextOverFont                          *
 *****************************************************************/
void cursor::setTextOverFont(std::string f)
{
   mCursor.setTextOverFont(f);
}

/*****************************************************************
 *                        setTextOver                            *
 *****************************************************************/
void cursor::setTextOver(string txt)
{
   mCursor.setTextOver(txt);
}

/*****************************************************************
 *                             Draw                              *
 *****************************************************************/
void cursor::draw(int mouseX, int mouseY, float angle,
                  float scaleX, float scaleY, float scaleZ)
{
   mCursor.draw(mouseX, mouseY, angle, scaleX, scaleY, scaleZ);
}

/*****************************************************************
 *                         Static Members                        *
 *****************************************************************/
int cursor::currentCursor=-1;
MouseCursor cursor::mCursor;


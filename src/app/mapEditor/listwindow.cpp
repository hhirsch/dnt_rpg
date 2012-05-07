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

#include "listwindow.h"
#include "../../etc/dirs.h"
using namespace dntMapEditor;


/* NPCs */
#define FILE_CHARACTERS "mapEditor/lists/characters.lst"

/* Objects */
#define FILE_BUILDING   "mapEditor/lists/building.lst"
#define FILE_CARS       "mapEditor/lists/cars.lst" 
#define FILE_COMMON     "mapEditor/lists/common.lst"
#define FILE_DOORS      "mapEditor/lists/doors.lst"
#define FILE_GUNS       "mapEditor/lists/guns.lst"
#define FILE_ICEX       "mapEditor/lists/icex.lst"
#define FILE_NATURE     "mapEditor/lists/nature.lst"
#define FILE_MACABRE    "mapEditor/lists/macabre.lst"
#define FILE_BATH       "mapEditor/lists/bath.lst"
#define FILE_STREET     "mapEditor/lists/street.lst"
#define FILE_JUNK       "mapEditor/lists/junk.lst"

/* Particles */
#define FILE_FIRE       "mapEditor/lists/fire.lst"
#define FILE_GRASS      "mapEditor/lists/grass.lst"
#define FILE_SMOKE      "mapEditor/lists/smoke.lst"
#define FILE_SNOW       "mapEditor/lists/snow.lst"
#define FILE_WATERFALL  "mapEditor/lists/waterfall.lst"

/* Musics */
#define FILE_MUSIC      "mapEditor/lists/music.lst"

/* Textures */
#define FILE_INDOOR_TEXTURE   "mapEditor/lists/floor_indoor.lst"
#define FILE_OUTDOOR_TEXTURE  "mapEditor/lists/floor_outdoor.lst"

#include <fstream>
#include <iostream>
using namespace std;

#include "../../etc/defparser.h"

/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
TupleList::TupleList():dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}
/********************************************************************
 *                              Destructor                          *
 ********************************************************************/
TupleList::~TupleList()
{
   clearList();
}
/********************************************************************
 *                             freeElement                          *
 ********************************************************************/
void TupleList::freeElement(dntListElement* obj)
{
   Tuple* t = (Tuple*)obj;
   if(t)
   {
      delete(t);
   }
}

/********************************************************************
 *                             Constructor                          *
 ********************************************************************/
ListWindow::ListWindow(Farso::GuiInterface* gui)
{
   intGui = gui;
   state = -1;
   list = NULL;
   intWindow = NULL;
}

/********************************************************************
 *                              Destructor                          *
 ********************************************************************/
ListWindow::~ListWindow()
{
   if(intWindow)
   {
      intGui->closeWindow(intWindow);
   }
}
/********************************************************************
 *                              getState                            *
 ********************************************************************/
int ListWindow::getState()
{
   return(state);
}

/********************************************************************
 *                              setState                            *
 ********************************************************************/
void ListWindow::setState(int st)
{
   dirs dir;
   string fileName = "";
   if( (state != st) || (intWindow == NULL))
   {
      if(intWindow == NULL)
      {
         open();
      }
      state = st;
      list->clear();
      /* Define the file */
      switch(state)
      {
         case STATE_CHARACTERS:
            fileName = FILE_CHARACTERS;
         break;
         case STATE_BUILDING:
            fileName = FILE_BUILDING;
         break;
         case STATE_CARS:
            fileName = FILE_CARS;
         break;
         case STATE_COMMON:
            fileName = FILE_COMMON;
         break;
         case STATE_DOORS:
            fileName = FILE_DOORS;
         break;
         case STATE_GUNS:
            fileName = FILE_GUNS;
         break;
         case STATE_ICEX:
            fileName = FILE_ICEX;
         break;
         case STATE_NATURE:
            fileName = FILE_NATURE;
         break;
         case STATE_MACABRE:
            fileName = FILE_MACABRE;
         break;
         case STATE_BATH:
            fileName = FILE_BATH;
         break;
         case STATE_STREET:
            fileName = FILE_STREET;
         break;
         case STATE_JUNK:
            fileName = FILE_JUNK;
         break;
         case STATE_FIRE:
            fileName = FILE_FIRE;
         break;
         case STATE_GRASS:
            fileName = FILE_GRASS;
         break;
         case STATE_SMOKE:
            fileName = FILE_SMOKE;
         break;
         case STATE_SNOW:
            fileName = FILE_SNOW;
         break;
         case STATE_WATERFALL:
            fileName = FILE_WATERFALL;
         break;
         case STATE_MUSIC:
            fileName = FILE_MUSIC;
         break;
         case STATE_INDOOR_TEXTURE:
            fileName = FILE_INDOOR_TEXTURE;
         break;
         case STATE_OUTDOOR_TEXTURE:
            fileName = FILE_OUTDOOR_TEXTURE;
         break;
      }
      /* Load file */
      if(!fileName.empty())
      {
         loadFromFile(dir.getRealFile(fileName));
      }
      intWindow->draw(0,0);
   }
}

/********************************************************************
 *                               open                               *
 ********************************************************************/
void ListWindow::open()
{
   intWindow = intGui->insertWindow(186,0,437,211,"Select");
   list = intWindow->getObjectsList()->insertListText(5,15,245,190);
   intWindow->setExternPointer(&intWindow);
   intGui->openWindow(intWindow);
}

/********************************************************************
 *                           loadFromFile                           *
 ********************************************************************/
void ListWindow::loadFromFile(string fileName)
{
   defParser f;
   string key="", value="";
   Tuple* t;

   /* Clear current list */
   tuples.clearList();

   /* Let's read the file */
   if(!f.load(fileName, true))
   {
      cerr << "Failed to load list file: " << fileName << endl;
      return;
   }

   while(f.getNextTuple(key, value))
   {
      t = new Tuple();
      t->title = key;
      t->fileName = value;
      list->insertText(key);
      tuples.insert(t);
   }
}

/********************************************************************
 *                           getFileName                            *
 ********************************************************************/
string ListWindow::getFileName()
{
   return(selFileName);
}

/********************************************************************
 *                       getFileNameWithTitle                       *
 ********************************************************************/
string ListWindow::getFileNameWithTitle(string title)
{
   int i;
   Tuple* t = (Tuple*)tuples.getFirst();

   for(i = 0; i < tuples.getTotal(); i++)
   {
      if(t->title == title)
      {
         return(t->fileName);
      }
      t = (Tuple*)t->getNext();
   }
   return("");
}

/********************************************************************
 *                             eventGot                             *
 ********************************************************************/
bool ListWindow::eventGot(int type, Farso::GuiObject* object)
{
   selFileName = "";
   if(type == Farso::EVENT_SELECTED_LIST_TEXT)
   {
      if(object == list)
      {
         selFileName = getFileNameWithTitle(list->getSelectedText());
         return(!selFileName.empty());
      }
   }
   return(false);
}


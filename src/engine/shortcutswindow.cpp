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

#include "shortcutswindow.h"
#include "cursor.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "util.h"
using namespace std;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
shortcutsWindow::shortcutsWindow()
{
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
shortcutsWindow::~shortcutsWindow()
{
}

/***********************************************************************
 *                                open                                 *
 ***********************************************************************/
void shortcutsWindow::open(Farso::GuiInterface* gui)
{
   dirs dir;
   int i;
   int midX=Farso::SCREEN_X/2;

   guiUsed = gui;

   /* Clear talents */
   if(!clearedTalents)
   {
      for(i=0; i < QUICK_FEATS; i++)
      {
         buttonQuickFeat[i] = 0;
         quickFeat[i] = NULL;
      }
      clearedTalents = true;
   }

   /* Open the window */
   if(shortCutsWindow == NULL)
   {
      curDefinedTalent = -1;
      curSelectedTalent = -1;
 
      /* Open Shortcuts Window */
      shortCutsWindow = gui->insertWindow(midX-247,Farso::SCREEN_Y-88,
            midX+247,Farso::SCREEN_Y-1, "", true);
      Farso::Picture* pic = shortCutsWindow->getObjectsList()->insertPicture(
            80,0,0,0,
            dir.getRealFile("texturas/shortcutsw/shortcuts.png").c_str());
      Farso::TabButton* tb;
      tb = shortCutsWindow->getObjectsList()->insertTabButton(80,0,334,43,NULL);
      tb->setObjectBelow(pic);
      tb->setStyle(Farso::TabButton::STYLE_HIGH);
      buttonAttackMode = tb->insertButton(11,5,44,38);/* Attack Mode */
      buttonJournal = tb->insertButton(51,5,84,38);/* Journal Window */
      buttonJournal->setMouseHint(gettext("Open Quests Window"));
      buttonInventory = tb->insertButton(91,5,124,38);/* Inventory */
      buttonInventory->setMouseHint(gettext("Open Inventory Window"));
      buttonMap = tb->insertButton(131,5,164,38);/* Map */
      buttonMap->setMouseHint(gettext("Open Map Window"));
      buttonGroup = tb->insertButton(171,5,204,38);/* Party/Group */
      buttonGroup->setMouseHint(gettext("Open Group/Party Window"));
      buttonRest = tb->insertButton(211,5,244,38);/* Rest */
      buttonRest->setMouseHint(gettext("Rest"));
      buttonCharacter = tb->insertButton(251,5,284,38);/* Character */
      buttonCharacter->setMouseHint(gettext("View Character Informations"));
      buttonEndTurn = tb->insertButton(291,5,324,38);/* End Turn */


      int i;
      tb = shortCutsWindow->getObjectsList()->insertTabButton(0,44,494,87,
            NULL);

      tb->setStyle(Farso::TabButton::STYLE_HIGH);
      
      /* Load / Save / Menu */
      buttonMenu = tb->insertButton(11, 5, 44, 38);
      buttonMenu->setMouseHint(gettext("Menu"));
      buttonLoad = tb->insertButton(50, 5, 84, 38);
      buttonLoad->setMouseHint(gettext("Load"));
      buttonSave = tb->insertButton(90, 5, 124, 38);
      buttonSave->setMouseHint(gettext("Save"));

      /* Quick Attacks  */
      for(i=0; i<QUICK_FEATS; i++)
      {
         buttonQuickFeat[i] = tb->insertButton(130+i*40,5,164+i*40,38);
         picQuickFeat[i] = shortCutsWindow->getObjectsList()->insertPicture(
                     131+i*40,49,0,0, NULL);
         picQuickFeat[i]->setSurfaceDeletion(false);
         tb->setObjectBelow(picQuickFeat[i]);
      }
      pic = shortCutsWindow->getObjectsList()->insertPicture(0,44,0,0,
            dir.getRealFile("texturas/shortcutsw/quickattacks.png").c_str());
      tb->setObjectBelow(pic);


      /* Finally, open the window */
      shortCutsWindow->setExternPointer(&shortCutsWindow);
      gui->openWindow(shortCutsWindow);
   }

   /* Redefine images */
   for(i=0; i < QUICK_FEATS; i++)
   {
      if(quickFeat[i])
      {
         picQuickFeat[i]->set(quickFeat[i]->info->image);
      }
   }
   shortCutsWindow->draw(0,0);
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void shortcutsWindow::close(Farso::GuiInterface* gui)
{
   if(shortCutsWindow != NULL)
   {
      gui->closeWindow(shortCutsWindow);
      shortCutsWindow = NULL;

      if(talentWindow)
      {
         delete(talentWindow);
         talentWindow = NULL;
         curDefinedTalent = -1;
      }
   }
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool shortcutsWindow::isOpened()
{
   return(shortCutsWindow != NULL);
}

/***********************************************************************
 *                              reOpen                                 *
 ***********************************************************************/
void shortcutsWindow::reOpen(Farso::GuiInterface* gui)
{
   int x1=0, y1=0, x2=0, y2=0;
   if(isOpened())
   {
      /* Retrieve current postion */
      x1 = shortCutsWindow->getX1();
      y1 = shortCutsWindow->getY1();
      x2 = shortCutsWindow->getX2();
      y2 = shortCutsWindow->getY2();
      
      /* Close the window */
      close(gui);
      /* Open the window */
      open(gui);

      /* Reset the position */
      shortCutsWindow->setCoordinate(x1, y1, x2, y2);
   }
}

/***********************************************************************
 *                          setClearedTalents                          *
 ***********************************************************************/
void shortcutsWindow::setClearedTalents(bool b)
{
   clearedTalents = b;
}

/***********************************************************************
 *                         getSelectedTalent                           *
 ***********************************************************************/
feat* shortcutsWindow::getSelectedTalent()
{
   if((curSelectedTalent >=0) && (curSelectedTalent < QUICK_FEATS))
   {
      return(quickFeat[curSelectedTalent]);
   }

   return(NULL);
}

/***********************************************************************
 *                        clearSelectedTalent                          *
 ***********************************************************************/
void shortcutsWindow::clearSelectedTalent()
{
   curSelectedTalent = -1;
}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
int shortcutsWindow::treat(Farso::GuiObject* object, int eventInfo, int engineMode,
      character* activeCharacter)
{
   int i, res;
   cursor mouseCursor;

   /* No opened window => No Events */
   if(shortCutsWindow == NULL)
   {
      if(talentWindow)
      {
         delete(talentWindow);
         talentWindow = NULL;
      }
      return(SHORTCUTS_WINDOW_NONE);
   }

   /* Treat inner talent window */
   if(talentWindow)
   {
      res = talentWindow->treat(object, eventInfo);
      if(res == TALENT_WINDOW_CANCEL)
      {
         /* Canceled selection */
         curDefinedTalent = -1;
         delete(talentWindow);
         talentWindow = NULL;
      }
      else if(res == TALENT_WINDOW_CONFIRM)
      {
         /* Set the current */
         quickFeat[curDefinedTalent] = talentWindow->getLastSelectedFeat();
         picQuickFeat[curDefinedTalent]->set(
               quickFeat[curDefinedTalent]->info->image);
         shortCutsWindow->draw(0,0);
         delete(talentWindow);
         talentWindow = NULL;
      }
   }
   
   /* Verify if event occur at the shortcuts window */
   switch(eventInfo)
   {
      
      /* Show Information about state-relative shortcuts */
      case Farso::EVENT_ON_FOCUS_TAB_BUTTON:
      {
         if(object == (Farso::GuiObject*) buttonAttackMode)
         {
            if(engineMode != ENGINE_MODE_TURN_BATTLE)
             {
                mouseCursor.setTextOver(gettext("Enter Battle Mode"));
             }
             else
             {
                mouseCursor.setTextOver(gettext("Select Normal Attack"));
             }  
            return(SHORTCUTS_WINDOW_OTHER); 
         }
         else if(object == (Farso::GuiObject*) buttonEndTurn)
         {
            if(engineMode == ENGINE_MODE_TURN_BATTLE)
            {
               mouseCursor.setTextOver(gettext("End Character's Turn"));
            }
            else
            {
               mouseCursor.setTextOver(
                     gettext("Only Available in Battle Mode"));
            }
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else
         {
            /* Let's see if is under a quick feat */
            for(i=0; i < QUICK_FEATS; i++)
            {
               if(object == (Farso::GuiObject*)buttonQuickFeat[i])
               {
                  if(quickFeat[i] != NULL)
                  {
                     mouseCursor.setTextOver(quickFeat[i]->info->name);
                     return(SHORTCUTS_WINDOW_OTHER);
                  }
                  else
                  {
                     mouseCursor.setTextOver(
                           gettext("Click to select a talent"));
                     return(SHORTCUTS_WINDOW_OTHER);
                  }
               }
            }
         }
      }
      break;

      /* Treat Shortcuts pressing */
      case Farso::EVENT_PRESSED_TAB_BUTTON:
      {
         if(object == (Farso::GuiObject*) buttonAttackMode)
         {
            if( engineMode != ENGINE_MODE_TURN_BATTLE )
            {
               return(SHORTCUTS_WINDOW_ATTACK_MODE);
            }
         }
         else if( object == (Farso::GuiObject*) buttonMap)
         {
            return(SHORTCUTS_WINDOW_MAP);
         } 
         else if(object == (Farso::GuiObject*) buttonEndTurn)
         {
            return(SHORTCUTS_WINDOW_END_TURN);
         }
         else if(object == (Farso::GuiObject*) buttonInventory)
         {
            return(SHORTCUTS_WINDOW_INVENTORY);
         }
         else if(object == (Farso::GuiObject*) buttonJournal)
         {
            return(SHORTCUTS_WINDOW_JOURNAL);
         }
         else if(object == (Farso::GuiObject*) buttonRest)
         {
            return(SHORTCUTS_WINDOW_REST);
         }
         else if(object == (Farso::GuiObject*) buttonCharacter)
         {
            return(SHORTCUTS_WINDOW_CHARACTER);
         }
         else if(object == (Farso::GuiObject*) buttonMenu)
         {
            return(SHORTCUTS_WINDOW_MENU);
         }
         else if(object == (Farso::GuiObject*) buttonSave)
         {
            return(SHORTCUTS_WINDOW_SAVE);
         }
         else if(object == (Farso::GuiObject*) buttonLoad)
         {
            return(SHORTCUTS_WINDOW_LOAD);
         }
         else
         {
            /* Let's see if selectd a quick feat */
            for(i=0; i < QUICK_FEATS; i++)
            {
               if(object == (Farso::GuiObject*)buttonQuickFeat[i])
               {
                  if(quickFeat[i] == NULL)
                  {
                     /* Must define it */
                     if(!talentWindow)
                     {
                        talentWindow = new featsWindow(guiUsed);
                     }
                     else
                     {
                        talentWindow->close();
                     }
                     talentWindow->open(activeCharacter, true);

                     curDefinedTalent = i;
                     return(SHORTCUTS_WINDOW_OTHER);
                  }
                  else
                  {
                     /* Selected a feat to use */
                     curSelectedTalent = i;
                     return(SHORTCUTS_WINDOW_SELECTED_TALENT);
                  }
               }
            }
         }
         break;
      }

      /* Right press tabbutton */
      case Farso::EVENT_RIGHT_PRESSED_TAB_BUTTON:
      {
         /* Let's see if selectd a quick feat */
         for(i=0; i < QUICK_FEATS; i++)
         {
            if(object == (Farso::GuiObject*)buttonQuickFeat[i])
            {
               /* call to define it */
               if(!talentWindow)
               {
                  talentWindow = new featsWindow(guiUsed);
               }
               else
               {
                  talentWindow->close();
               }
               talentWindow->open(activeCharacter, true);

               curDefinedTalent = i;
               curSelectedTalent = -1;
               return(SHORTCUTS_WINDOW_OTHER);
            }
         }
      }
      break;
   }
   return(SHORTCUTS_WINDOW_NONE);
}

/***********************************************************************
 *                       loadQuickTalents                              *
 ***********************************************************************/
bool shortcutsWindow::loadQuickTalents(string fileName, 
      character* activeCharacter)
{
   defParser parser;
   string key, value;
   int i = 0;

   if(!parser.load(fileName))
   {
      return(false);
   }
   /* No need to clear them */
   clearedTalents = true;

   /* Define each one */
   while(parser.getNextTuple(key, value))
   {
      if(key == "quickTalent")
      {
         if(value == "NONE")
         {
            quickFeat[i] = NULL;
         }
         else
         {
            quickFeat[i] = activeCharacter->actualFeats->featByString(value);
            if(shortCutsWindow)
            {
               picQuickFeat[curDefinedTalent]->set(
                     quickFeat[curDefinedTalent]->info->image);
               shortCutsWindow->draw(0,0);
            }
         }
         i++;
      }
   }

   return(true);
}

/***********************************************************************
 *                        saveQuickTalents                             *
 ***********************************************************************/
bool shortcutsWindow::saveQuickTalents(string fileName)
{
   ofstream file;
   int i;

   file.open(fileName.c_str(), ios::out | ios::binary);
   if(!file)
   {
      cerr << "Error while opening file: " << fileName << endl;
      return(false);
   }

   for(i=0; i < QUICK_FEATS; i++)
   {
      if(quickFeat[i] != NULL)
      {
         file << "quickTalent = " << quickFeat[i]->info->idString << endl;
      }
      else
      {
         file << "quickTalent = NONE" << endl;
      }
   }

   file.close();

   return(true);
}

/***********************************************************************
 *                            Static Fields                            *
 ***********************************************************************/
Farso::GuiInterface* shortcutsWindow::guiUsed = NULL;

Farso::OneTabButton* shortcutsWindow::buttonMenu = NULL;
Farso::OneTabButton* shortcutsWindow::buttonSave = NULL;
Farso::OneTabButton* shortcutsWindow::buttonLoad = NULL;

Farso::OneTabButton* shortcutsWindow::buttonAttackMode = NULL;
Farso::OneTabButton* shortcutsWindow::buttonJournal = NULL;
Farso::OneTabButton* shortcutsWindow::buttonMap = NULL;
Farso::OneTabButton* shortcutsWindow::buttonInventory = NULL;
Farso::OneTabButton* shortcutsWindow::buttonRest = NULL;
Farso::OneTabButton* shortcutsWindow::buttonGroup = NULL;
Farso::OneTabButton* shortcutsWindow::buttonCharacter = NULL;
Farso::OneTabButton* shortcutsWindow::buttonEndTurn = NULL;

Farso::OneTabButton* shortcutsWindow::buttonQuickFeat[QUICK_FEATS];
Farso::Picture* shortcutsWindow::picQuickFeat[QUICK_FEATS];
feat* shortcutsWindow::quickFeat[QUICK_FEATS];

featsWindow* shortcutsWindow::talentWindow = NULL;
int shortcutsWindow::curDefinedTalent = -1;
int shortcutsWindow::curSelectedTalent = -1;

bool shortcutsWindow::clearedTalents = false;

Farso::Window* shortcutsWindow::shortCutsWindow = NULL;


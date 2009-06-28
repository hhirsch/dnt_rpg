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

#include "shortcutswindow.h"
#include "../etc/dirs.h"
#include "util.h"

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
void shortcutsWindow::open(guiInterface* gui)
{
   dirs dir;
   if(shortCutsWindow == NULL)
   {
  
      shortCutsWindow = gui->insertWindow(0,SCREEN_Y-129,512,SCREEN_Y-1,
                                          gettext("Shortcuts"));
      thingTxt = shortCutsWindow->getObjectsList()->insertTextBox(8,20,249,35,1,
                                                            gettext("Nothing"));
      fpsTxt = shortCutsWindow->getObjectsList()->insertTextBox(8,36,128,51,1,
                                                                "FPS:");
      partTxt = shortCutsWindow->getObjectsList()->insertTextBox(129,36,
                                                                 249,51,1,
                                                                 "Part:");
      buttonSave=shortCutsWindow->getObjectsList()->insertButton(8,102,76,120,
                                                             gettext("Save"),0);
      buttonMenu=shortCutsWindow->getObjectsList()->insertButton(77,102,140,120,
                                                             gettext("Menu"),0);
      buttonLoad = shortCutsWindow->getObjectsList()->insertButton(141,102,
                                                                   209,120,
                                                             gettext("Load"),0);
      hourTxt = shortCutsWindow->getObjectsList()->insertTextBox(210,102,
                                                                 249,120,1,
                                                                 "00:00");
      hourTxt->setFont(DNT_FONT_TIMES,11,DNT_FONT_ALIGN_CENTER);

      tabButton* tb;
      tb = shortCutsWindow->getObjectsList()->insertTabButton(252,14,0,0,
                  dir.getRealFile("texturas/shortcutsw/shortcuts.png").c_str());
      buttonAttackMode = tb->insertButton(7,4,43,36);/* Attack Mode */
      tb->insertButton(7,40,43,72);/* Attack 1 */
      tb->insertButton(7,75,43,107);/* Attack 7 */

      buttonJournal = tb->insertButton(53,4,89,36);/* Journal Window */
      tb->insertButton(53,40,89,72);/* Attack 2 */
      tb->insertButton(53,75,89,107);/* Attack 8 */

      buttonInventory = tb->insertButton(99,4,135,36);/* Inventory */
      tb->insertButton(99,40,135,72);/* Attack 3 */
      tb->insertButton(99,75,135,107);/* Attack 9 */

      buttonMap = tb->insertButton(141,4,177,36);/* Map */
      tb->insertButton(141,40,177,72);/* Attack 4 */
      tb->insertButton(141,75,177,107);/* Attack 10 */

      buttonGroup = tb->insertButton(180,4,216,36);/* Party/Group */
      tb->insertButton(180,40,216,72);/* Attack 5 */
      buttonAssign = tb->insertButton(180,75,216,107);/* Assign Attacks */

      buttonCharacter = tb->insertButton(220,4,256,36);/* Character */
      tb->insertButton(220,40,256,72);/* Attack 6 */
      buttonEndTurn = tb->insertButton(220,75,256,107);/* End Turn */

      shortCutsWindow->setExternPointer(&shortCutsWindow);
      gui->openWindow(shortCutsWindow);
   }
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void shortcutsWindow::close(guiInterface* gui)
{
   if(shortCutsWindow != NULL)
   {
      gui->closeWindow(shortCutsWindow);
      shortCutsWindow = NULL;
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
void shortcutsWindow::reOpen(guiInterface* gui)
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
 *                             setThing                                *
 ***********************************************************************/
void shortcutsWindow::setThing(string thingName)
{
   if(shortCutsWindow != NULL)
   {
      thingTxt->setText(thingName);
   }
}

/***********************************************************************
 *                        setParticlesNumber                           *
 ***********************************************************************/
void shortcutsWindow::setParticlesNumber(int total)
{
   char part[32];
   if(shortCutsWindow != NULL)
   {
      sprintf(part, "%s: %d", gettext("Particles"), total);
      partTxt->setText(part);
   }
}

/***********************************************************************
 *                               setFPS                                *
 ***********************************************************************/
void shortcutsWindow::setFPS(float fps)
{
   char txt[32];
   if(shortCutsWindow != NULL)
   {
      sprintf(txt, "%s: %.2f", gettext("FPS"), fps);
      fpsTxt->setText(txt);
   }
}

/***********************************************************************
 *                              setHour                                *
 ***********************************************************************/
void shortcutsWindow::setHour(string hour)
{
   if(shortCutsWindow != NULL)
   {
      hourTxt->setText(hour);
   }
}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
int shortcutsWindow::treat(guiObject* object, int eventInfo, int engineMode)
{
   /* No opened window => No Events */
   if(shortCutsWindow == NULL)
   {
      return(SHORTCUTS_WINDOW_NONE);
   }
   
   /* Verify if event occur at the shortcuts window */
   switch(eventInfo)
   {
      
      /* Show Information about the shortcut */
      case FARSO_EVENT_ON_FOCUS_TAB_BUTTON:
      {
         if(object == (guiObject*) buttonAttackMode)
         {
            if(engineMode != ENGINE_MODE_TURN_BATTLE)
             {
                thingTxt->setText(gettext("Enter Battle Mode"));
             }
             else
             {
                thingTxt->setText(gettext("Select Normal Attack"));
             }  
            return(SHORTCUTS_WINDOW_OTHER); 
         }
         else if(object == (guiObject*) buttonJournal)
         {
            thingTxt->setText(gettext("Open Quests Window"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonMap)
         {
            thingTxt->setText(gettext("Open Map Window"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonEndTurn)
         {
            if(engineMode == ENGINE_MODE_TURN_BATTLE)
            {
               thingTxt->setText(gettext("End Character's Turn"));
            }
            else
            {
               thingTxt->setText(gettext("Only Available in Battle Mode"));
            }
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonInventory)
         {
            thingTxt->setText(gettext("Open Inventory Window"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonAssign)
         {
            thingTxt->setText(gettext("Open Assign Attacks Window"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonCharacter)
         {
            thingTxt->setText(gettext("View Character Informations"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
         else if(object == (guiObject*) buttonGroup)
         {
            thingTxt->setText(gettext("Open Group/Party Window"));
            return(SHORTCUTS_WINDOW_OTHER);
         }
      }
      break;

      /* Treat Shortcuts pressing */
      case FARSO_EVENT_PRESSED_TAB_BUTTON:
      {
         if(object == (guiObject*) buttonAttackMode)
         {
            if( engineMode != ENGINE_MODE_TURN_BATTLE )
            {
               return(SHORTCUTS_WINDOW_ATTACK_MODE);
            }
         }
         else if( object == (guiObject*) buttonMap)
         {
            return(SHORTCUTS_WINDOW_MAP);
         } 
         else if(object == (guiObject*) buttonEndTurn)
         {
            return(SHORTCUTS_WINDOW_END_TURN);
         }
         else if(object == (guiObject*) buttonInventory)
         {
            return(SHORTCUTS_WINDOW_INVENTORY);
         }
         else if(object == (guiObject*) buttonJournal)
         {
            return(SHORTCUTS_WINDOW_JOURNAL);
         }
         else if(object == (guiObject*) buttonCharacter)
         {
            return(SHORTCUTS_WINDOW_CHARACTER);
         }
         break;
      }

      /* And, finally, the tree buttons */
      case FARSO_EVENT_PRESSED_BUTTON:
      {
         if(object == (guiObject*) buttonMenu)
         {
            return(SHORTCUTS_WINDOW_MENU);
         }
         else if(object == (guiObject*) buttonSave)
         {
            return(SHORTCUTS_WINDOW_SAVE);
         }
         else if(object == (guiObject*) buttonLoad)
         {
            return(SHORTCUTS_WINDOW_LOAD);
         }
         break;
      }
   }
   return(SHORTCUTS_WINDOW_NONE);
}

/***********************************************************************
 *                            Static Fields                            *
 ***********************************************************************/
textBox* shortcutsWindow::fpsTxt = NULL;
textBox* shortcutsWindow::partTxt = NULL;
textBox* shortcutsWindow::thingTxt = NULL;
textBox* shortcutsWindow::hourTxt = NULL;

button* shortcutsWindow::buttonMenu = NULL;
button* shortcutsWindow::buttonSave = NULL;
button* shortcutsWindow::buttonLoad = NULL;

oneTabButton* shortcutsWindow::buttonAttackMode = NULL;
oneTabButton* shortcutsWindow::buttonJournal = NULL;
oneTabButton* shortcutsWindow::buttonMap = NULL;
oneTabButton* shortcutsWindow::buttonInventory = NULL;
oneTabButton* shortcutsWindow::buttonAssign = NULL;
oneTabButton* shortcutsWindow::buttonGroup = NULL;
oneTabButton* shortcutsWindow::buttonCharacter = NULL;
oneTabButton* shortcutsWindow::buttonEndTurn = NULL;

window* shortcutsWindow::shortCutsWindow = NULL;


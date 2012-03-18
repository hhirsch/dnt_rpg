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

#include "listtext.h"
#include "guilist.h"
#include "interface.h"
#include "farsoopts.h"
using namespace std;
using namespace Farso;

/**************************************************************
 *                        Constructor                         *
 **************************************************************/
ListText::ListText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
                   GuiList* list)
         : GuiObject(surface)
{
   int i;
   /* Nullify elements */
   selectedText = ""; 
   selectedPos = -1;  

   Options opt;
   Font fnt;
   fnt.defineFont(opt.getDefaultFont(), 10);

   /* Init things */
   type = Farso::OBJECT_LIST_TEXT;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;

   /* Alloc tabButtons pointers */
   maxButtons = ((yb-ya-2) / fnt.getHeight());
   listButtons = new OneTabButton*[maxButtons];


   intList = list;

   GuiList* l = (GuiList*)list;

   /* Create the rollbar */
   roll = l->insertRolBar(x1, y1, x2, y2, "");
   
   /* Create the tabButton */
   table = l->insertTabButton(x1, y1, x2-x1-12, y2-y1, NULL);
   table->setStyle(TabButton::STYLE_LIST_TEXT);
   for(i = 0; i<maxButtons; i++)
   {
      listButtons[i] = table->insertButton(1, (i*fnt.getHeight())+3,
                                              (x2-x1)-14, 
                                              ((i+1)*fnt.getHeight())+2);
   }

   table->setObjectBelow(roll);

}

/**************************************************************
 *                         Destructor                         *
 **************************************************************/
ListText::~ListText()
{
   /* Delete the pointers */
   delete [] listButtons;
   
   /* Clear the list */
   textList.clear();
}

/**************************************************************
 *                           clear                            *
 **************************************************************/
void ListText::clear()
{
   textList.clear();
   roll->setText("");

   defineTabButton();
}

/**************************************************************
 *                         insertText                         *
 **************************************************************/
void ListText::insertText(string text)
{
   /* Insert with default color */
   insertText(text, -1, -1, -1);
}

/**************************************************************
 *                         insertText                         *
 **************************************************************/
void ListText::insertText(string text, int r, int g, int b)
{
   Options opt;
   textList.push_back(text);
   if(r == -1)
   {
      roll->addText(text);
   }
   else
   {
      roll->addText(text, opt.getDefaultFont(), 10, Font::STYLE_NORMAL,
                    Font::ALIGN_LEFT, r,g,b);
   }
   roll->setFirstLine(0);
   defineTabButton();
}

/**************************************************************
 *                         removeText                         *
 **************************************************************/
void ListText::removeText(string text)
{
   std::list<std::string>::iterator it;
   for(it=textList.begin(); it != textList.end(); it++)
   {
      if((*it) == text)
      {
         textList.remove((*it));
         defineTabButton();
         return;
      }
   }
   defineTabButton();
}

/**************************************************************
 *                      defineTabButton                       *
 **************************************************************/
void ListText::defineTabButton()
{
   int curInit = roll->getFirstLine();
   int i;

   for(i = 0; i<maxButtons; i++)
   {
      listButtons[i]->setAvailable(i+curInit < (int)textList.size());
   }
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void ListText::draw()
{
   //Actually nothing to do here
}

/**************************************************************
 *                         eventGot                           *
 **************************************************************/
bool ListText::eventGot(int type, GuiObject* object)
{
   int i;
   selectedText = "";
   selectedPos = -1;

   /* Verify Events */

   /* RolBar events */
   if(type == EVENT_PRESSED_BUTTON)
   {
      if(roll->isOwner(object))
      {
         /* Must reconstruct tabbutton avaialability */
         defineTabButton();
      }
   }

   /* TabButton events */
   else if(type == EVENT_PRESSED_TAB_BUTTON)
   {
      for(i = 0; i<maxButtons; i++)
      {
         if(object == (GuiObject*)listButtons[i])
         {
            int pos = roll->getFirstLine() + i;
            if(pos < (int)textList.size())
            {
               int k;
               std::list<std::string>::iterator it = textList.begin();
               for(k = 0; k < pos; k++)
               {
                  it++;
               }
               selectedText = (*it);
               selectedPos = pos;
            }
            return(true);
         }
      }
   }
   return(false);
}

/**************************************************************
 *                      getSelectedText                       *
 **************************************************************/
string ListText::getSelectedText()
{
   return(selectedText);
}

/**************************************************************
 *                      getSelectedPos                        *
 **************************************************************/
int ListText::getSelectedPos()
{  
   return(selectedPos);
}

